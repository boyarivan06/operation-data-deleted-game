#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <future>

#include "beast.h"
#include "../creatures/creature.h"
#include "game_state.h"
#include "level.h"
#include "view.h"
#include "game_object.h"


/**
 * @brief Класс контроллера - управление всей игрой
 */
class Controller {
    std::shared_ptr<Creature> current_object;
    std::vector<std::shared_ptr<Creature>> managed;
    //std::vector<std::shared_ptr<GameObject>> objects;
    //GameObject* mouse_pointer;
    std::vector<Level> levels;
    std::shared_ptr<Level> current_level;
    View view;
    GameState game_state = GameState::MENU;
    GameState controller_state = GameState::DEFAULT;
    std::vector<std::shared_ptr<GameObject>> non_game_objects;

    std::atomic<bool> loading_complete{false};
    std::atomic<bool> loading_cancelled{false};
    std::atomic<bool> loading_error{false};
    std::string error_message;
    std::future<void> loading_future;

    void load_game_data_async(const std::string& filename);
    void show_loading_animation();

    void update_projectiles();

public:
    void handle_keys() const; ///< Обработка нажатий клавиш
    void set_current_object(std::shared_ptr<Creature> detective); ///< Поменять главный управляемый объект

    void run_bots() const; ///< Управлять ботами
    void run_bots_parallel() const;

    //std::vector<std::shared_ptr<GameObject>> &get_objects(){return objects;}
    void control(); ///< Общее управление
    void set_current_level(const Level & level);

    Controller();
    ~Controller();
    void change_state(GameState game_state); ///< Изменение игрового состояния

    void menu_control(); ///< Контроль меню

    std::vector<std::shared_ptr<GameObject>> collect_objects(); ///< Сбор объектов для отрисовки

    void run(); ///< Запуск
    void load_json(const std::string& filename); ///< Загрузить уровни из json-файла

    Field load_field(const nlohmann::json &j); ///< Загрузить поле из json-объекта

    void clear_level();
};

#endif
