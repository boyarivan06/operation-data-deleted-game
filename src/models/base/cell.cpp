#include "cell.h"
#include <algorithm>
#include <memory>
#include <vector>
#include "game_object.h"
#include "obstacle.h"

Cell::~Cell() = default;


std::vector<std::shared_ptr<GameObject>>& Cell::get_objects() {
    return objects;
}

void Cell::add_object(const std::shared_ptr<GameObject> &shared) {
    objects.push_back(shared);
}

void Cell::remove_object(const std::shared_ptr<GameObject> & item) {
    for (int i = 0; i < objects.size(); i++) {
        if (objects[i] == item) {
            objects.erase(objects.begin()+i);
            break;
        }
        if (i == objects.size()-1) throw std::runtime_error("This item isn't from this cell!");
    }
}

bool Cell::is_obstacle() {
    return std::ranges::any_of(objects, [](const auto& obj) {
        return std::dynamic_pointer_cast<Obstacle>(obj) != nullptr;
    });
}
