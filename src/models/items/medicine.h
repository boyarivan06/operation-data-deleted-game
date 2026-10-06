#ifndef MEDICINE_H
#define MEDICINE_H
#include "item.h"

/**
 * @brief Класс аптечки
 */
class Medicine : public Item {
    int treat_time = 0;
    int health = 0;
public:
    Medicine() = delete;
    Medicine(int, int); ///< Конструктор с заданным количеством HP и временем исцеления
    void heal(int&, int); ///< Исцелить тварь (передавать указатель на Creature::health) - используется в методе Creature::treat()
    [[nodiscard]] int get_treat_time() const; ///< Узнать время исцеления
    [[nodiscard]] int get_remaining_health() const; ///< Узнать оставшееся время
};


#endif