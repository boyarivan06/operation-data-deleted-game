#include <fstream>
#include "button.h"
#include "colors.h"
#include "config.h"
#include "controller.h"
#include "foorageer.h"
#include "Label.h"
#include "rational_beast.h"
#include "wild_beast.h"

void Controller::load_json(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open field file: "+filename);
    }
    nlohmann::json json_data;
    file >> json_data;
    int levels_count = json_data["levels_count"].get<int>();
    const auto& levels_data = json_data["levels"];
    for (int i = 0; i < levels_count; i++) {
        const auto& level_json = levels_data[i];
        auto name = level_json["name"].get<std::string>();
        auto preamble = level_json["preamble"].get<std::string>();
        Field new_field = load_field(level_json["field"]);
        int mins = level_json["minutes"];
        int secs = level_json["seconds"];
        levels.emplace_back(new_field, name, preamble, CountdownTimer(mins,secs));
    }
    file.close();
}

Field Controller::load_field(const nlohmann::json &j) {
    int width = j["width"].get<int>();
    int height = j["height"].get<int>();
    auto result_field = Field(height, width);
    const auto& cells_data = j["cells"];
    for (int i = 0; i < j["cells_count"].get<int>(); ++i) {
        const auto& cell_json = cells_data[i];
        for (int k = 0; k < cell_json["objects_count"].get<int>();k++) {
            auto& object_json = cell_json["objects"][k];
            auto type = object_json["type"].get<std::string>();
            if (type == "obstacle") {
                auto new_obst = std::make_shared<Obstacle>();
                new_obst->set_breakability(object_json["data"]["breakable"].get<bool>());
                new_obst->set_position({cell_json["x"].get<float>(), cell_json["y"].get<float>()});
                new_obst->set_displacement({FIELD_X, FIELD_Y});
                result_field.add_object(new_obst);
                new_obst->set_color(DARKBLUE);
                new_obst->resource_name = 6;
            }
            else if (type == "depot") {
                auto new_depot = std::make_shared<Depot>();
                new_depot->set_displacement({FIELD_X, FIELD_Y});
                for (auto & bot : *result_field.get_bots()) {
                    if (std::shared_ptr<Foorageer> foor = std::dynamic_pointer_cast<Foorageer>(bot); foor != nullptr) {
                        foor->add_depot(new_depot);
                    }
                }
                for (const auto& obj : object_json["data"]["content"]) {
                    if (auto _type = obj["type"].get<std::string>();_type == "weapon") {
                        auto new_weapon = std::make_shared<Weapon>();
                        new_depot->add_item(new_weapon);
                        new_weapon->set_bullet_count(obj["data"]["bullets_count"].get<int>());
                        new_weapon->set_distance(obj["data"]["distance"].get<int>());
                        new_weapon->set_damage(obj["data"]["damage"].get<int>());
                        new_weapon->set_reload_time(obj["data"]["reload_time"].get<float>());
                    }
                }
                new_depot->set_position({cell_json["x"].get<float>(), cell_json["y"].get<float>()});
                new_depot->set_color(WAX_GRAY);
                result_field.add_object(new_depot);
            }
            else if (type == "item") {
                int weight = object_json["data"]["weight"].get<int>();
                auto new_item = std::make_shared<Item>(weight);
                new_item->set_position({cell_json["x"].get<float>(), cell_json["y"].get<float>()});
                result_field.add_object(new_item);
                new_item->resource_name = 4;
                new_item->set_displacement({FIELD_X, FIELD_Y});
            }
            else if (type == "wild_beast") {
                auto wild_beast = std::make_shared<WildBeast>();
                wild_beast->set_armour(object_json["data"]["armour"].get<int>());
                wild_beast->set_position({cell_json["x"].get<float>(), cell_json["y"].get<float>()});
                result_field.add_object(wild_beast);
                wild_beast->set_color(RED);
                wild_beast->set_displacement({FIELD_X, FIELD_Y});
                result_field.get_bots()->emplace_back(wild_beast);
                wild_beast->resource_name = 2;
            }
            else if (type == "rational_beast") {
                auto rational_beast = std::make_shared<RationalBeast>();
                rational_beast->set_armour(object_json["data"]["armour"].get<int>());
                rational_beast->set_position({cell_json["x"].get<float>(), cell_json["y"].get<float>()});
                result_field.add_object(rational_beast);
                rational_beast->set_color(BROWN);
                rational_beast->set_displacement({FIELD_X, FIELD_Y});
                rational_beast->set_current_weapon(std::make_shared<Weapon>(5));
                result_field.get_bots()->emplace_back(rational_beast);
                rational_beast->resource_name=3;
            }
            else if (type == "foorageer") {
                auto strength = object_json["data"]["strength"].get<int>();
                auto foorageer = std::make_shared<Foorageer>(strength);
                foorageer->resource_name = 1;
                foorageer->set_position({cell_json["x"].get<float>(), cell_json["y"].get<float>()});
                foorageer->set_displacement({FIELD_X, FIELD_Y});
                result_field.get_bots()->emplace_back(foorageer);
                result_field.add_object(foorageer);
                auto depots = result_field.get_all<Depot>();
                for (auto& depot : depots)
                    foorageer->add_depot(depot);
            }
        }
    }
    return result_field;
}



void Controller::load_game_data_async(const std::string& filename) {
    try {
        if (loading_cancelled) return;

        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open field file: " + filename);
        }

        nlohmann::json json_data;
        file >> json_data;

        int levels_count = json_data["levels_count"].get<int>();
        const auto& levels_data = json_data["levels"];

        for (int i = 0; i < levels_count; i++) {
            if (loading_cancelled) {
                file.close();
                return;
            }

            const auto& level_json = levels_data[i];
            auto name = level_json["name"].get<std::string>();
            auto preamble = level_json["preamble"].get<std::string>();
            Field new_field = load_field(level_json["field"]);

            int mins = level_json["minutes"].get<int>();
            int secs = level_json["seconds"].get<int>();
            levels.emplace_back(new_field, name, preamble, CountdownTimer(mins, secs));
        }

        file.close();
        loading_complete = true;
    }
    catch (const std::exception& e) {
        loading_error = true;
        error_message = e.what();
        loading_complete = true;
        // TODO: обработать ошибку загрузки
    }
}

void Controller::show_loading_animation() {
    static float rotation = 0.0f;
    static int dots = 0;
    static float dot_timer = 0.0f;

    float deltaTime = GetFrameTime();
    dot_timer += deltaTime;

    if (dot_timer > 0.5f) {
        dots = (dots + 1) % 4;
        dot_timer = 0.0f;

        std::string loading_text = loading_error ? "Ошибка загрузки" : "Загрузка";
        for (int i = 0; i < dots; i++) {
            loading_text += ".";
        }

        if (!non_game_objects.empty()) {
            auto label = std::dynamic_pointer_cast<Label>(non_game_objects[0]);
            if (label) {
                label->set_name(loading_text);

                if (loading_error) {
                    label->set_color(RED);
                }
            }
        }
    }

    if (!loading_error) {
        rotation += deltaTime * 180.0f;
        if (rotation > 360.0f) rotation -= 360.0f;

        Vector2 center = {
            static_cast<float>(GetScreenWidth()) / 2,
            static_cast<float>(GetScreenHeight()) / 2 + 50
        };

        DrawCircleLines(center.x, center.y, 30, Colors::wax_gray);
        Vector2 line_end = {
            center.x + cosf(rotation * DEG2RAD) * 30,
            center.y + sinf(rotation * DEG2RAD) * 30
        };
        DrawLineEx(center, line_end, 3, Colors::custom_violet);
    }

    if (loading_error && !non_game_objects.empty() && non_game_objects.size() < 2) {
        auto error_label = std::make_shared<Label>(
            Label(error_message, {},
                  {static_cast<float>(GetScreenWidth())/2/CELL_SIZE,
                   static_cast<float>(GetScreenHeight())/2/CELL_SIZE + 2},
                  {2, 7},
                  {0, 0},
                  20,
                  RED)
        );
        non_game_objects.emplace_back(error_label);

        auto retry_button = std::make_shared<Button>(
            "Повторить", DARKGREEN,
            [this]() {
                loading_cancelled = false;
                loading_complete = false;
                loading_error = false;
                error_message.clear();
                levels.clear();
                non_game_objects.clear();

                auto load_label = std::make_shared<Label>(
                    Label("Загрузка", {},
                          {static_cast<float>(GetScreenWidth())/2/CELL_SIZE,
                           static_cast<float>(GetScreenHeight())/2/CELL_SIZE - 2},
                          {2, 1},
                          {0, 0},
                          30,
                          WHITE)
                );
                non_game_objects.emplace_back(load_label);
                loading_future = std::async(std::launch::async,
                    [this]() { this->load_game_data_async("../../game_data/levels.json"); });
            }
        );
        retry_button->set_position({
            static_cast<float>(GetScreenWidth())/2/CELL_SIZE - 2,
            static_cast<float>(GetScreenHeight())/2/CELL_SIZE + 4
        });
        non_game_objects.emplace_back(retry_button);
    }
}