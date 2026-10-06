#include "rational_beast.h"

#include "config.h"
#include "depot.h"
#include "detective.h"
#include "exceptions.h"

void RationalBeast::move(Field & field) {
    if (is_moving()) return;

    std::shared_ptr<Detective> nearest_detective = field.find_nearest<Detective>(cell_position);
    if (!nearest_detective) return;

    Vector2 detective_pos = nearest_detective->get_position();

    // Если есть оружие
    if (current_weapon && current_weapon->get_bullet_count() > 0) {
        float distance = module(cell_position - detective_pos);
        float weapon_range = static_cast<float>(current_weapon->get_distance());

        if (distance <= weapon_range) {
            // Проверяем, нет ли препятствий на линии огня
            if (has_line_of_sight(detective_pos, field) && current_weapon->get_bullet_count() >= 0) {
                strike(cell_position, detective_pos, field);

                // После выстрела проверяем, остались ли патроны
                if (current_weapon->get_bullet_count() == 0) {
                    // Если патроны закончились, ищем новое оружие
                    search_for_weapon(field);
                }
                return;
            } else {
                // Нет прямой видимости - подходим ближе
                approach_target(detective_pos, field);
            }
        } else {
            // Вне досягаемости - подходим
            approach_target(detective_pos, field);
        }
    } else {
        // Нет оружия - ищем
        search_for_weapon(field);
    }
}

bool RationalBeast::has_line_of_sight(Vector2 to, Field& field) const {
    Vector2 start = cell_position;
    Vector2 end = {roundf(to.x), roundf(to.y)};

    int dx = abs(static_cast<int>(end.x - start.x));
    int dy = abs(static_cast<int>(end.y - start.y));
    int steps = std::max(dx, dy);

    if (steps == 0) return true;

    float x_inc = static_cast<float>(end.x - start.x) / steps;
    float y_inc = static_cast<float>(end.y - start.y) / steps;

    float x = start.x;
    float y = start.y;

    for (int i = 0; i <= steps; i++) {
        Vector2 cell_coords = {roundf(x), roundf(y)};

        if (!field.include_point(cell_coords)) return false;

        auto& cell = field.get_cell(cell_coords);
        auto objects = cell.get_objects();

        for (const auto& obj : objects) {
            if (auto obst = std::dynamic_pointer_cast<Obstacle>(obj)) {
                // Любое препятствие блокирует обзор
                return false;
            }
        }

        x += x_inc;
        y += y_inc;
    }

    return true;
}

void RationalBeast::approach_target(Vector2 target, Field& field) {
    float weapon_range = current_weapon ? static_cast<float>(current_weapon->get_distance()) : 1.0f;

    // Рассчитываем позицию для остановки (на расстоянии выстрела - 1 клетка)
    Vector2 direction = normalize_coords(target - cell_position);
    Vector2 ideal_position = target - direction * (weapon_range - 1.0f);

    // Проверяем, доступна ли идеальная позиция
    if (field.include_point(ideal_position) && !field.get_cell(ideal_position).is_obstacle()) {
        Creature::move(ideal_position, field);
    } else {
        // Иначе идем просто к цели
        Creature::move(target, field);
    }
}

void RationalBeast::search_for_weapon(Field& field) {
    // Если есть пустое оружие - выбросить
    if (current_weapon && current_weapon->get_bullet_count() == 0) {
        current_weapon->set_displacement({FIELD_X, FIELD_Y});
        current_weapon->set_position(cell_position);
        field.get_cell(cell_position).add_object(current_weapon);
        current_weapon = nullptr;
    }

    // Поиск оружия в текущей клетке
    Cell &cell = field.get_cell(cell_position);
    std::vector<std::shared_ptr<Weapon>> weapons = cell.get_all<Weapon>();

    if (!weapons.empty()) {
        // Берем первое попавшееся оружие с патронами
        for (const auto& weapon : weapons) {
            if (weapon->get_bullet_count() > 0) {
                current_weapon = weapon;
                cell.remove_object(weapon);
                return;
            }
        }
    }

    // Поиск склада в текущей клетке
    std::shared_ptr<Depot> depot = cell.get_first<Depot>();
    if (depot) {
        auto content = depot->get_content();
        for (auto& item : content) {
            std::shared_ptr<Weapon> weapon = std::dynamic_pointer_cast<Weapon>(item);
            if (weapon && weapon->get_bullet_count() > 0) {
                current_weapon = weapon;
                depot->remove_item(item);
                return;
            }
        }
    }

    // Поиск ближайшего оружия или склада
    auto nearest_weapon = field.find_nearest<Weapon>(cell_position);
    auto nearest_depot = field.find_nearest<Depot>(cell_position);
    auto target = Vector2{MAXFLOAT, MAXFLOAT};
    if (nearest_depot && nearest_weapon && nearest_weapon->get_bullet_count()==0) nearest_weapon = nullptr;
    else if (!nearest_depot && nearest_weapon && nearest_weapon->get_bullet_count()==0) {
        auto all_weapons = field.get_all<Weapon>();
        for (auto& weapon : all_weapons) {
            if (weapon->get_bullet_count() != 0) {
                if (weapon->get_position() < target)
                    target = weapon->get_position();
            }
        }
        if (target == Vector2{MAXFLOAT, MAXFLOAT}) return;
    }
    if (nearest_weapon && nearest_depot) {
        float dist_to_weapon = module(cell_position - nearest_weapon->get_position());
        float dist_to_depot = module(cell_position - nearest_depot->get_position());
        target = (dist_to_weapon <= dist_to_depot) ? nearest_weapon->get_position() : nearest_depot->get_position();
    }
    else if (nearest_weapon) {
        target = nearest_weapon->get_position();
    }
    else if (nearest_depot) {
        target = nearest_depot->get_position();
    }
    else {
        // Некуда идти
        return;
    }

    Creature::move(target, field);
}
