#include "beast.h"
#include "vector2_operations.h"
#include <iostream>


int Beast::get_armour() const {
    return armour;
}

void Beast::set_armour(int value) {
    armour = value;
}

Beast::Beast(Vector2 area_start, Vector2 area_end) {
    this->GameObject::set_position({area_start.x + rand() % static_cast<int>(area_end.x - area_start.x), area_start.y + rand() % static_cast<int>(area_end.y - area_start.y)});
}
