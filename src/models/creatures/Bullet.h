#pragma once

#include "beast.h"
#include "vector2_operations.h"
#include <iostream>

#include "detective.h"

/**
 * @brief Класс пули
 */
class Bullet : public Beast {
    Vector2 direction;
    float speed = 15.0f;
    float damage = 10.0f;
    bool active = true;
    Vector2 start_position{};  // Начальная позиция для расчета дистанции
    float max_distance = 20.0f;  // Максимальная дистанция полета

public:
    explicit Bullet(Vector2 dir); ///< Конструктор с заданным направлением
    void move(Field &) override;
    void update_movement(float deltaTime, Field &field, const std::shared_ptr<Creature> &current_object); ///< Покадровое обновление положения пули на карте
    bool check_collision(const Vector2 &pos, Field &field, const std::shared_ptr<Creature> &current_object); ///< Проверка на столкновение с объектами в клетке
    bool has_hit() const { return !active; } ///< Проверка, долетела ли пуля
    bool is_active() const { return active; } ///< Проверка, летит ли пуля

    void set_damage(float d) { damage = d; } ///< Поменять урон пули
    void set_speed(float s) { speed = s; } ///< Поменять скорость пули
    void set_max_distance(float dist) { max_distance = dist; } ///< Поменять дальность полёта

private:
    bool check_cell_collision(const Vector2 &cell_coords, Field &field, const std::shared_ptr<Creature> &current_object);
    void process_collision_with_object(std::shared_ptr<GameObject> obj, Cell& cell);
};