#include "bullet.h"
#include "obstacle.h"
#include "creature.h"
#include "detective.h"
#include "wild_beast.h"
#include "rational_beast.h"
#include <cmath>
#include <iostream>

Bullet::Bullet(Vector2 dir) : direction(dir){
    max_health=0;
    size = {0.2,0.2};  // Видимый размер
    set_color(Color{255, 165, 0, 255});  // Оранжевый

    // Сохраняем стартовую позицию
    start_position = get_position();
    cell_position = start_position;
}

void Bullet::move(Field& field) {
    active = true;
}

void Bullet::update_movement(float deltaTime, Field& field, const std::shared_ptr<Creature>& current_object) {
    if (!active) {
        return;
    }

    Vector2 current_pos = get_position();

    Vector2 new_pos = current_pos + direction * (speed * deltaTime);

    if (check_collision(new_pos, field, current_object)) {
        active = false;
        return;
    }

    float distance_traveled = module(new_pos - start_position);
    if (distance_traveled > max_distance) {
        std::cout << "Bullet exceeded max distance: " << distance_traveled << std::endl;
        active = false;
        return;
    }
    set_position(new_pos);

    Vector2 grid_pos = Field::convert_global_to_cell(new_pos);
    if (!field.include_point(grid_pos)) {
        std::cout << "Bullet left field at: " << grid_pos.x << ", " << grid_pos.y << std::endl;
        active = false;
    }
}

bool Bullet::check_collision(const Vector2& pos, Field& field, const std::shared_ptr<Creature> &current_object) {
    Vector2 cell_coords = Field::convert_global_to_cell(pos);

    if (!field.include_point(cell_coords)) {
        std::cout << "Bullet out of bounds at: " << cell_coords.x << ", " << cell_coords.y << std::endl;
        return true;
    }

    return check_cell_collision(cell_coords, field, current_object);
}

bool Bullet::check_cell_collision(const Vector2& cell_coords, Field& field, const std::shared_ptr<Creature>& current_object) {
    if (cell_coords == Field::convert_global_to_cell(get_position())) return false;
    //std::cout << "on check bullet coords==" << cell_coords<< ", player coords = "<< current_object->get_position() << std::endl;
    if (cell_coords == current_object->get_position()) {
        current_object->cause_damage(static_cast<int>(damage));
        return true;
    }
    auto& cell = field.get_cell(cell_coords);
    auto objects = cell.get_objects();

    for (const auto& obj : objects) {
        if (!obj) {
            continue;
        }

        if (obj.get() == this) {
            continue;
        }

        process_collision_with_object(obj, cell);

        if (!active) {
            return true;
        }
    }

    return false;
}

void Bullet::process_collision_with_object(std::shared_ptr<GameObject> obj, Cell& cell) {
    // 1. Столкновение с препятствием
    if (auto obstacle = std::dynamic_pointer_cast<Obstacle>(obj)) {
        std::cout << "Bullet hit obstacle at position: "
                  << obstacle->get_position().x << ", "
                  << obstacle->get_position().y << std::endl;

        if (obstacle->is_breakable()) {
            std::cout << "Breaking obstacle" << std::endl;
            cell.remove_object(obstacle);
        } else {
            std::cout << "Obstacle is unbreakable" << std::endl;
        }

        active = false;
        return;
    }

    // 2. Столкновение с существом
    if (auto creature = std::dynamic_pointer_cast<Creature>(obj)) {
        // Пропускаем, если это пуля (наследник Creature)
        if (std::dynamic_pointer_cast<Bullet>(creature)) {
            return;
        }

        std::cout << "Bullet hit creature at position: "
                  << creature->get_position().x << ", "
                  << creature->get_position().y << std::endl;
        std::cout << "Creature health before: " << creature->get_health() << std::endl;

        // Наносим урон
        creature->cause_damage(damage);

        std::cout << "Damage dealt: " << damage << std::endl;
        std::cout << "Creature health after: " << creature->get_health() << std::endl;

        active = false;
        return;
    }

    // 3. Столкновение с детективом
    if (auto detective = std::dynamic_pointer_cast<Detective>(obj)) {
        std::cout << "Bullet hit detective at position: "
                  << detective->get_position().x << ", "
                  << detective->get_position().y << std::endl;
        std::cout << "Detective health before: " << detective->get_health() << std::endl;

        detective->cause_damage(damage);

        std::cout << "Damage dealt: " << damage << std::endl;
        std::cout << "Detective health after: " << detective->get_health() << std::endl;

        active = false;
        return;
    }

    // 4. Столкновение с диким зверем
    if (auto wild_beast = std::dynamic_pointer_cast<WildBeast>(obj)) {
        std::cout << "Bullet hit wild beast at position: "
                  << wild_beast->get_position().x << ", "
                  << wild_beast->get_position().y << std::endl;

        wild_beast->cause_damage(damage);

        std::cout << "Damage dealt to wild beast: " << damage << std::endl;

        active = false;
        return;
    }

    // 5. Столкновение с рациональным зверем
    if (auto rational_beast = std::dynamic_pointer_cast<RationalBeast>(obj)) {
        std::cout << "Bullet hit rational beast at position: "
                  << rational_beast->get_position().x << ", "
                  << rational_beast->get_position().y << std::endl;

        rational_beast->cause_damage(damage);

        std::cout << "Damage dealt to rational beast: " << damage << std::endl;

        active = false;
        return;
    }

    // Для других типов объектов можно добавить обработку здесь
}