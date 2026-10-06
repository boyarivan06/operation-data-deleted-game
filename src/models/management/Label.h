//
// Created by Иван Бояринцев on 02.12.25.

#ifndef LABEL_H
#define LABEL_H
#include "game_object.h"


/**
 * @brief Класс подписи
 */
class Label : public GameObject {
    Color font_color;
    int font_size;
public:
    Label(const std::string& name, Color color, Vector2 size, Vector2 position, Vector2 displacement, int font_size, Color font_color);
    [[nodiscard]] int get_font_size() const; ///< узнать размер шрифта
    [[nodiscard]] Color get_font_color() const; ///< Узнать цвет шрифта
    void set_font_size(int); ///< узнать размер шрифта
    void set_font_color(Color new_color); ///< узнать цвет шрифта

};



#endif //LABEL_H
