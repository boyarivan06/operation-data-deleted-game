#include "button.h"
#include "config.h"
#include "controller.h"
#include "detective.h"
#include "Label.h"

void Controller::set_current_object(std::shared_ptr<Creature> detective) {
    current_object = std::move(detective);
}

void Controller::control() {

}

void Controller::set_current_level(const Level &level) {
    current_level = std::make_shared<Level>(level);
}

Controller::Controller() : current_object(nullptr), current_level(nullptr) {
    game_state = GameState::LOADING;
    controller_state = GameState::LOADING;
    view.init_window("GAME");
    view.set_window_size({30, 15});

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
        [this] { this->load_game_data_async("../../game_data/levels.json"); });
}

Controller::~Controller() {
    loading_cancelled = true;
    if (loading_future.valid()) {
        loading_future.wait();
    }
}

void Controller::change_state(GameState game_state) {
    switch (game_state) {
        case GameState::LOADING: {
            non_game_objects.clear();
            auto load_label = std::make_shared<Label>(
                Label("Загрузка", {},
                      {static_cast<float>(GetScreenWidth())/2/CELL_SIZE,
                       static_cast<float>(GetScreenHeight())/2/CELL_SIZE - 2},
                      {2, 1},
                      {0, 0},
                      30,
                      YELLOW)
            );
            non_game_objects.emplace_back(load_label);
            View::set_window_title("Загрузка");
            break;
        }
        case GameState::GAME_OVER: {
            if (current_level) {
                clear_level();
            }
            non_game_objects.clear();
            auto menu_button = std::make_shared<Button>("В меню", DARKGREEN, [this](){
                this->game_state = GameState::MENU;
            });
            menu_button->set_position({4, 4});
            non_game_objects.emplace_back(menu_button);

            auto game_over_label = std::make_shared<Label>(
                Label("Игра окончена!", {},
                      {static_cast<float>(GetScreenWidth())/2/CELL_SIZE - 3,
                       static_cast<float>(GetScreenHeight())/2/CELL_SIZE - 3},
                      {6, 1},
                      {0, 0},
                      40,
                      RED)
            );
            non_game_objects.emplace_back(game_over_label);
            break;
        }
        case GameState::LEVEL_FIELD: {
            if (!current_level) {
                game_state = GameState::MENU;
                return;
            }
            std::pair<int, int> field_size = current_level->get_field().get_size();
            auto hero = std::make_shared<Detective>(Detective());
            hero->set_displacement({FIELD_X, FIELD_Y});
            hero->resource_name = 0;
            hero->set_position({2, 8});
            current_object = hero;
            current_level->get_field().add_object(hero);
            view.set_window_size(field_size);
            current_level->timer.start();
            break;
        }
        case GameState::VICTORY: {
            if (current_level) {
                clear_level();  // Очищаем уровень
            }

            // Создаем кнопку "В меню"
            auto menu_button = std::make_shared<Button>("В меню", DARKGREEN, [this](){
                this->game_state = GameState::MENU;
                this->clear_level();
            });
            menu_button->set_position({4, 4});
            non_game_objects.emplace_back(menu_button);

            // Добавляем надпись "Победа!"
            auto victory_label = std::make_shared<Label>(
                Label("Победа!", {},
                      {static_cast<float>(GetScreenWidth())/2/CELL_SIZE - 2,
                       static_cast<float>(GetScreenHeight())/2/CELL_SIZE - 3},
                      {4, 1},
                      {0, 0},
                      40,
                      GREEN)
            );
            non_game_objects.emplace_back(victory_label);
            break;
        }
        case GameState::MENU: {
            non_game_objects.clear();
            view.set_window_size({30, 15});
            View::set_window_title("Menu");
            for (int i = 0; i < levels.size(); i++) {
                non_game_objects.emplace_back(std::make_shared<Button>(
                    levels[i].get_name(),
                    DARKGREEN,
                    [this, level_copy = levels[i]]() mutable {
                        this->current_level = nullptr;
                        this->current_level = std::make_shared<Level>();
                        *this->current_level = std::move(level_copy);
                        this->game_state = GameState::LEVEL_FIELD;
                    }
                ));
                non_game_objects[i]->set_position({4, 1 + static_cast<float>(i)*3});
            }
            break;
        }
    }
    controller_state = game_state;
}