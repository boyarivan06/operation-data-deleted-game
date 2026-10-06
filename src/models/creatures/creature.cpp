#include "creature.h"
#include "config.h"
#include "exceptions.h"
#include "rational_beast.h"

void Creature::move(Vector2 target, Field& field) {
    Vector2 current_pos = {roundf(cell_position.x), roundf(cell_position.y)};

    try {
        route = field.a_star_route(current_pos, target);
    } catch (const WayImpassableException&) {
        route.clear();
        return;
    }
}

void Creature::update_route_movement(Field& field, float deltaTime) {
    if (is_moving()) {
        update_move(deltaTime);
        return;
    }

    // Пытаемся сделать шаг по маршруту
    while (!route.empty()) {
        Vector2 next_cell = route[0];

        if (try_step(next_cell, field)) {
            // Шаг успешно начат
            route.erase(route.begin());  // Удаляем точку из маршрута
            return;
        } else {
            // Не удалось сделать шаг
            route.clear();  // Очищаем маршрут
            break;
        }
    }
}

bool Creature::try_step(Vector2 direction, Field& field) {
    if (isMoving) return false;

    Vector2 int_pos = {roundf(cell_position.x), roundf(cell_position.y)};
    Vector2 target_pos = {int_pos.x + direction.x, int_pos.y + direction.y};

    if (field.include_point(target_pos) && !field.get_cell(target_pos).is_obstacle()) {
        field.get_cell(cell_position).remove_object(shared_from_this());
        field.get_cell(target_pos).add_object(shared_from_this());
        start_move(target_pos);
        time_points -= STEP_TIME_POINTS;
        return true;
    }
    return false;
}

void Creature::step(Vector2 direction, Field& field) {
    // Устаревший метод, используйте try_step
    try_step(direction, field);
}

int Creature::get_health() const {
    return health;
}

int Creature::get_time_points() {
    return time_points;
}

void Creature::start_move(Vector2 destination) {
    moveStart = cell_position;
    targetPosition = destination;
    isMoving = true;
    moveProgress = 0.0f;
}

void Creature::update_move(float deltaTime) {
    moveProgress += deltaTime / 0.3f; // Длительность 0.3 секунды

    cell_position = {
        moveStart.x + (targetPosition.x - moveStart.x) * moveProgress,
        moveStart.y + (targetPosition.y - moveStart.y) * moveProgress
    };

    if (moveProgress >= 1.0f) {
        cell_position = targetPosition;
        isMoving = false;
    }
}


int Creature::get_max_health() const {
    return max_health;
}
