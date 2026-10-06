#include "fighter.h"
#include <iostream>
#include <cmath>
#include "beast.h"
#include "Bullet.h"
#include "vector2_operations.h"
#include "config.h"
#include "exceptions.h"

Fighter::Fighter() {
    current_weapon = nullptr;
}

void Fighter::strike(Vector2 from, Vector2 to, Field& field) const {
    if (!current_weapon) return;


    float distance = module(to - from);
    if (distance > static_cast<float>(current_weapon->get_distance())) {
        return;
    }

    if (!current_weapon->strike())
        return;

    auto bullet = std::make_shared<Bullet>(to - from);
    bullet->set_position(from+Vector2{0.5,0.5});
    bullet->set_color(ORANGE);
    bullet->set_displacement({FIELD_X, FIELD_Y});
    bullet->set_damage(static_cast<float>(current_weapon->get_damage()));
    bullet->set_speed(2.0f);  // Быстрая пуля

    field.add_projectile(bullet);

    field.get_bots()->emplace_back(bullet);
    bullet->move(field);
}

void Fighter::set_current_weapon(const std::shared_ptr<Weapon> & w) {
    current_weapon=w;
}

std::shared_ptr<Weapon> Fighter::get_current_weapon() {
    return current_weapon;
}
