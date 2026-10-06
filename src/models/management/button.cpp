//
// Created by Иван Бояринцев on 28.10.25.
//

#include "button.h"

#include <utility>

Button::Button(std::string _name, Color _color, std::function<void()> _action) {
    name = std::move(_name);
    color = _color;
    action = std::move(_action);
    size = {4, 2};
}

void Button::operator()() const {
    action();
}
