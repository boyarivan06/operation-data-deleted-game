#include "controller.h"
#include <fstream>
#include <memory>
#include <utility>
#include <raylib.h>
#include "beast.h"
#include "button.h"
#include "config.h"
#include "exceptions.h"
#include "foorageer.h"
#include "vector2_operations.h"
#include "Label.h"
#include <thread>
#include <format>
#include "Bullet.h"
#include "colors.h"

Vector2 make_grid(Vector2 vector2) {
    return {
        static_cast<float>(static_cast<int>(roundf((vector2.x - CELL_SIZE / 2) / CELL_SIZE))),
        static_cast<float>(static_cast<int>(roundf((vector2.y - CELL_SIZE / 2) / CELL_SIZE)))
    };
}

void Controller::handle_keys() const {
    switch (controller_state) {
        case GameState::MENU:
        case GameState::GAME_OVER: {
            for (const auto& btn : non_game_objects) {
                if (!std::dynamic_pointer_cast<Button>(btn)) continue;
                auto btn_pos = (btn->get_position())*CELL_SIZE;
                auto btn_size = btn->get_size() * CELL_SIZE;
                Vector2 mouse_pos = GetMousePosition();
                /*if (controller_state == GameState::GAME_OVER) {
                    mouse_pos = mouse_pos - current_object->get_displacement();
                }*/
                if (CheckCollisionPointRec(mouse_pos, Rectangle{btn_pos.x, btn_pos.y, btn_size.x, btn_size.y})) {
                    btn->set_color(RED);
                    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                        (*std::dynamic_pointer_cast<Button>(btn))();
                    }
                }
                else btn->set_color(DARKGREEN);
            }
            break;
        }
        case GameState::LEVEL_FIELD: {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                Vector2 mouse_pos = GetMousePosition();
                Vector2 grid_pos = {
                    roundf((mouse_pos.x - CELL_SIZE/2) / CELL_SIZE),
                    roundf((mouse_pos.y - CELL_SIZE/2) / CELL_SIZE)
                };
                current_object->move(grid_pos-current_object->get_displacement(), current_level->get_field());
            }
            current_object->update_route_movement(current_level->get_field(), GetFrameTime());
            break;
        }
        default:
            break;
    }
}



void Controller::run_bots() const {
    const auto deltaTime = GetFrameTime();
    auto& bots = *current_level->get_field().get_bots();

    if (bots.empty()) return;
    std::vector<std::shared_ptr<Creature>> to_remove;

    for (size_t i = 0;  i < bots.size(); i++) {

        if (auto bullet = std::dynamic_pointer_cast<Bullet>(bots[i])) {
            bullet->update_movement(deltaTime, current_level->get_field(), current_object);

            if (bullet->has_hit()) {
                to_remove.push_back(bullet);
            }
        } else {
            if (bots[i]->get_health()<=0) {
                bots[i]->die(current_level->get_field());
                bots.erase(bots.begin()+i);
            }

            if (!bots[i]->is_moving() && !bots[i]->has_route()) {
                bots[i]->move(current_level->get_field());
            }
            bots[i]->update_route_movement(current_level->get_field(), deltaTime);
        }
    }


    for (const auto& obj : to_remove) {
        if (auto bullet = std::dynamic_pointer_cast<Bullet>(obj)) {
            current_level->get_field().remove_projectile(bullet);
        }
        auto it = std::find(bots.begin(), bots.end(), obj);
        if (it != bots.end()) {
            bots.erase(it);
        }
    }
}


void Controller::run_bots_parallel() const {
    auto deltaTime = GetFrameTime();
    auto bots = current_level->get_field().get_bots();
    std::vector<std::shared_ptr<Bullet>> bullets{};
    auto process_chunk = [bots, this, deltaTime](const size_t start, const size_t end, std::vector<std::shared_ptr<Bullet>> _bullets, std::shared_ptr<std::vector<std::shared_ptr<Beast>>> _bots) {
        for (size_t i = start; i <= end && i <= _bots->size(); i++) {
            if (auto bullet = std::dynamic_pointer_cast<Bullet>(_bots->operator[](i));bullet) {
                _bullets.emplace_back(bullet);
                continue;
            }
            if (_bots->operator[](i)->get_health()<=0) {
                _bots->operator[](i)->die(current_level->get_field());
                _bots->erase(_bots->begin()+i);
            }

            if (!_bots->operator[](i)->is_moving() && !_bots->operator[](i)->has_route()) {
                _bots->operator[](i)->move(current_level->get_field());
            }
            _bots->operator[](i)->update_route_movement(current_level->get_field(), deltaTime);
        }
    };

    if (bots->empty()) return;
    constexpr size_t num_threads = 4;
    const size_t chunk_size = (bots->size() + num_threads - 1) / num_threads;
    std::vector<std::thread> threads;

    for (size_t t = 0; t < num_threads; t++) {
        size_t start = t * chunk_size;
        size_t end = start + chunk_size;
        threads.emplace_back(process_chunk, start, end, bullets, bots);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    std::vector<std::shared_ptr<Creature>> to_remove;

    for (auto bullet : bullets) {
        bullet->update_movement(deltaTime, current_level->get_field(), current_object);

        if (bullet->has_hit()) {
            to_remove.push_back(bullet);
        }
    }

    for (const auto& obj : to_remove) {
        if (auto bullet = std::dynamic_pointer_cast<Bullet>(obj)) {
            current_level->get_field().remove_projectile(bullet);
        }
        auto it = std::find(bots->begin(), bots->end(), obj);
        if (it != bots->end()) {
            bots->erase(it);
        }
    }

}

std::vector<std::shared_ptr<GameObject>> Controller::collect_objects() {
    auto res = std::vector<std::shared_ptr<GameObject>>();


    if (!current_level || !current_object) {
        return res;
    }

    auto& field = current_level->get_field();
    for (int i = 0; i < field.get_size().first; i++) {
        for (int j = 0; j < field.get_size().second; j++) {
            auto& cell = field[i][j];
            res.insert(res.end(), cell.get_objects().begin(), cell.get_objects().end());
        }
    }
    res.emplace_back(std::make_shared<Label>(Label(std::format("HP: {}", current_object->get_health()), {0,0,0,0}, {5,1}, {0,0},{0,0}, CELL_SIZE-4,WHITE)));
    res.insert(res.end(), field.get_projectiles().begin(), field.get_projectiles().end());
    res.emplace_back(std::make_shared<Label>(Label(current_level->timer.to_string(), BLACK, {2,1}, {8,0},{0,0},CELL_SIZE,WAX_GRAY)));
    return res;
}

void Controller::run() {
    while (!WindowShouldClose()) {
        if (WindowShouldClose() && controller_state == GameState::LOADING) {
            loading_cancelled = true;
        }

        if (game_state != controller_state) change_state(game_state);

        switch (controller_state) {
            case GameState::LOADING: {
                if (loading_complete) {
                    if (loading_future.valid()) {
                        loading_future.get();
                    }
                    if (loading_error) {
                        show_loading_animation();
                    } else {
                        game_state = GameState::MENU;
                    }
                } else {
                    show_loading_animation();
                }
                view.draw_view(non_game_objects);
                break;
            }

            case GameState::LEVEL_FIELD: {
                if (current_object && current_object->get_health() <= 0) {
                    game_state = GameState::GAME_OVER;
                }
                current_level->timer.update();
                if (current_level->timer.is_finished()) {
                    game_state = GameState::VICTORY;
                }
                handle_keys();
                run_bots();
                view.draw_view(collect_objects());
                break;
            }
            case GameState::MENU: {
                handle_keys();
                view.draw_view(non_game_objects);
                break;
            }
            case GameState::VICTORY: {
                handle_keys();
                view.draw_view(non_game_objects);
                break;
            }
            case GameState::GAME_OVER: {
                handle_keys();
                view.draw_view(non_game_objects);
                break;
            }
        }
    }
}


void Controller::update_projectiles() {
    auto projectiles = current_level->get_field().get_projectiles();

    for (auto& projectile : projectiles) {
        if (auto bullet = std::dynamic_pointer_cast<Bullet>(projectile)) {
            bullet->update_movement(GetFrameTime(), current_level->get_field(), current_object);

            if (!bullet->is_active()) {
                current_level->get_field().remove_object(bullet);
            }
        }
    }
}

void Controller::clear_level() {
    if (!current_level) return;

    auto& field = current_level->get_field();
    auto& bots = *field.get_bots();

    for (auto& bot : bots) {
        if (!bot) continue;
        Vector2 bot_pos = bot->get_position();
        if (field.include_point(bot_pos)) {
            auto& cell = field.get_cell(bot_pos);
            auto& objects = cell.get_objects();
            auto it = std::find(objects.begin(), objects.end(), bot);
            if (it != objects.end()) {
                objects.erase(it);
            }
        }
    }
    bots.clear();

    field.get_projectiles().clear();

    current_object = nullptr;
    non_game_objects.clear();

    view.set_window_size({30, 15});
    View::set_window_title("Меню");
}

