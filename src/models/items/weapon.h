#ifndef WEAPON_H
#define WEAPON_H
#include "field.h"
#include "item.h"

/**
 * @brief Класс оружия
 */
class Weapon : public Item {
    int damage = 1;
    int bullets_count = 5;
    int distance = 3;
    int reload_time = 1; /// seconds?
    float wait_time = 0;
    float mistake = 1;
    int max_bullets_count = 5;

public:
    bool strike(); ///< Выстрел

    void set_bullet_count(int n); ///< Поменять количество пуль

    void set_distance(int i); ///< Поменять дистанцию поражения
    void set_damage(int d);

    void set_reload_time(float get);

    Weapon(int, int, int, int); ///< Конструктор: урон, количество пуль, дистанция поражения, время перезарядки
    [[nodiscard]] int get_damage() const; ///< узнать урон
    [[nodiscard]] int get_distance() const; ///< узнать дистанцию поражения
    [[nodiscard]] int get_bullet_count() const; ///< узнать количество пуль
    explicit Weapon(int bullet_count); ///< Конструктор с заданным количеством пуль
    Weapon();///< Конструктор по умолчанию
};

#endif
