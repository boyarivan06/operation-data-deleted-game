//
// Created by Иван Бояринцев on 02.12.25.
//

#include "Label.h"

Label::Label(const std::string &name, Color color, Vector2 size, Vector2 position, Vector2 displacement, int font_size, Color font_color) : GameObject(name, color, size, position, displacement), font_color(font_color), font_size(font_size){
}

int Label::get_font_size() const {
    return font_size;
}

Color Label::get_font_color() const {
    return font_color;
}

void Label::set_font_size(int new_size) {
    font_size = new_size;
}

void Label::set_font_color(Color new_color) {
    font_color = new_color;
}
