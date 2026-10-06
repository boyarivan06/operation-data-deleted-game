#ifndef BEAST_H
#define BEAST_H
#include "creature.h"

/**
 * @brief Класс монстра
 */
class Beast : public Creature {
protected:
    int armour = 0;
public:
    int get_armour() const; ///< Узнать броню
    void set_armour(int value); ///< Поменять броню
    virtual void move(Field&){} ///< Движение (виртуальная, not implemented)
    Beast() = default; ///< Конструктор по умолчанию
    Beast(Vector2, Vector2); ///< Конструктор со случайным размещением в заданном двумя точками прямоугольнике
};

#endif
