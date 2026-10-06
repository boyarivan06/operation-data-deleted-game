#ifndef CREATURE_H
#define CREATURE_H

#include "field.h"
#include "../base/game_object.h"


/**
 * @brief Класс твари - объекта, обладающего возможностью перемещения, нанесения и получения урона.
 * Основное свойство живого существа - возможность умереть.
 */
class Creature : public GameObject, public std::enable_shared_from_this<Creature> {
protected:
    int health = 10;
    int max_health = 10;
    int max_time_points = 100;
    int distance_of_view = 10;
    float accuracy = 1;
    int close_combat_strength = 10;
    float speed = 1;
    int time_points = 100;
    bool isMoving = false;
    float moveProgress = 0.0f;
    Vector2 moveStart = {};
    Vector2 targetPosition = {};
    std::vector<Vector2> route;
    void start_move(Vector2 destination);
    void update_move(float deltaTime);

public:
    [[nodiscard]] bool is_moving() const { return isMoving; } ///< Двигается ли тварь
    [[nodiscard]] bool has_route() const { return !route.empty(); } ///< Есть ли у твари маршрут

    void move(Vector2 target, Field& field); ///< Подвинуть тварь (запускает процесс перемещения до цели, если это возможно)
    virtual void die(Field&){} ///< Умереть (это надо уметь)
    void update_route_movement(Field& field, float deltaTime = 0.0f); ///< Обновить положение в соответствии с маршрутом

    bool try_step(Vector2 direction, Field &field); ///< Попытка переместиться в соседнюю клетку в указанном направлении (true, если удаётся)

    void clear_route() { route.clear(); } ///< Очистить маршрут твари

    void step(Vector2 direction, Field &field); ///< Переместиться в соседнюю клетку (использует try_step)
    [[nodiscard]] bool is_dead() const { return health <= 0; } ///< Тварь умерла?
    void cause_damage(int damage) { health = std::max(0, health - damage); } ///< Нанести твари урон

    int get_health() const; ///< Узнать здоровье твари

    int get_time_points(); ///< Узнать, сколько энергии на движение осталось у твари


    int get_max_health() const; ///< Узнать максимальное здоровье твари
};


struct BotCommand {
    std::shared_ptr<Creature> bot;
    enum { MOVE, DIE, REMOVE } action;
    std::shared_ptr<Creature> target; // для атак
    float damage;
};

#endif