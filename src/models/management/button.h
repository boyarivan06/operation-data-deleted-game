//
// Created by Иван Бояринцев on 28.10.25.

#ifndef BUTTON_H
#define BUTTON_H
#include "game_object.h"
#include <functional>


/**
 * @brief Класс кнопки
 */
class Button : public GameObject {
    std::function<void()> action;
public:
    Button(std::string, Color, std::function<void()>); ///< Конструктор с названием и функуией
    void operator()() const; ///< Исполнить функуию кнопки
};



#endif //BUTTON_H
