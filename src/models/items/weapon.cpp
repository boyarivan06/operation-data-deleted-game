#include "weapon.h"

#include "exceptions.h"

bool Weapon::strike() {
    if (wait_time > 0) {
        wait_time -= GetFrameTime();
        if (wait_time > 0) return false;
    }
    if (bullets_count >= 1) {
        bullets_count--;
        wait_time = static_cast<float>(reload_time);
        return true;
    }
    return false;
}

void Weapon::set_bullet_count(int n) {
    bullets_count = n;
}

void Weapon::set_distance(int i) {
    distance = i;
}

void Weapon::set_damage(int d) {
    damage = d;
}

void Weapon::set_reload_time(float get) {
    reload_time = get;
}

Weapon::Weapon(int dmg, int bullets_cnt, int dist, int rld_time): Item(bullets_cnt/2) {
    damage = dmg;
    bullets_count = bullets_cnt;
    distance = dist;
    reload_time = rld_time;
    resource_name=5;
}

int Weapon::get_damage() const {
    return damage;
}

int Weapon::get_distance() const {
    return distance;
}

int Weapon::get_bullet_count() const {
    return bullets_count;
}

Weapon::Weapon(int bc): Item(bc/2) {
    bullets_count = bc;
    resource_name=5;
}

Weapon::Weapon(): Item(4) {
    damage = 10;
    bullets_count = 10;
    reload_time = 5;
    distance = 5; ///< расстояние в клетках
    resource_name=5;
}
