#ifndef WILD_BEAST_H
#define WILD_BEAST_H
#include "beast.h"

/**
 * @brief Класс дикого монстра - втупую гонится за игроком
 */
class WildBeast : public Beast {
    int close_fight_damage=2;
    float sleep_time = 1;
    float current_sleep_time = 0;
public:
    void move(Field &field) override; ///< Перемещение - просто идёт в клетку к игроку и атакует в лоб
    void die(Field& field) override;
};

#endif
