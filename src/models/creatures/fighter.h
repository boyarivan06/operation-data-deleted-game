#ifndef FIGHTER_H
#define FIGHTER_H
#include "weapon.h"

/**
 * @brief Класс интерфейса - тот, кто может держать в руках оружие и стрелять
 */
class Fighter {
protected:
    std::shared_ptr<Weapon> current_weapon; ///< Текущее оружие

public:
    Fighter(); ///< Конструктор по умолчанию
    ~Fighter() = default; ///< Деструктор по умолчанию
    void strike(Vector2 from, Vector2 to, Field &) const; ///< Стрельба: from - откуда (целые), to - куда (целые), field - на каком поле
    void set_current_weapon(const std::shared_ptr<Weapon> &); ///< Поменять текущее оружие

    virtual std::shared_ptr<Weapon> get_current_weapon(); ///< Получить текущее оружие
};



#endif
