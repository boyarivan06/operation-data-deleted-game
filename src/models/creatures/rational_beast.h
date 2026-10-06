#ifndef RATIONAL_BEAST_H
#define RATIONAL_BEAST_H
#include "beast.h"
#include "fighter.h"
#include "../items/weapon.h"

/**
 * @brief Класс для разумного монстра - может поднимать оружие и стрелять в игрока.
 */
class RationalBeast: public Beast, public Fighter {
public:
    void move(Field &) override; ///< Перемещение - подходит на расстояние стрельбы и стреляет

    bool has_line_of_sight(Vector2 to, Field &field) const; ///< Может ли стрелять в точку

    void approach_target(Vector2 target, Field &field); ///< Выбор точки для стрельбы (куда идти)

    void search_for_weapon(Field &field); ///< Искать оружие на земле вокруг

};
#endif
