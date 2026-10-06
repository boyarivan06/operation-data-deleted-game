#include "game_object.h"

#include <utility>
#include "vector2_operations.h"
#include "config.h"

Color GameObject::get_color() {
    return color;
}

Vector2 GameObject::get_size() const {
    return size;
}

std::string GameObject::get_name() {
    return name;
}

int GameObject::get_resource() const {
    return resource_name;
}

GameObject::GameObject(Vector2 pos) {
    cell_position = pos;
}

GameObject::GameObject(std::string _name, Color _color, Vector2 _size) {
    color = _color;
    name = std::move(_name);
    size = _size;
}

GameObject::GameObject(const std::string& _name, Color _color, Vector2 _size, Vector2 _pos) {
    color = _color;
    name = _name;
    size = _size;
    cell_position = _pos;
}

GameObject::GameObject(const std::string& _name, Color _color, Vector2 _size, Vector2 _pos, Vector2 _displ) {
    color = _color;
    name = _name;
    size = _size;
    cell_position = _pos;
    displacement = _displ;
}

void GameObject::set_position(Vector2 pos) {
    cell_position = pos;
    real_position = pos * CELL_SIZE;
}

void GameObject::set_name(std::string text) {
    name = std::move(text);
}

void GameObject::set_displacement(Vector2 new_disp) {
    displacement = new_disp;
}

Vector2 GameObject::get_position() const {
    return cell_position;
}

Vector2 GameObject::get_displacement() const {
    return displacement;
}

void GameObject::set_color(Color _color) {
    color = _color;
}
