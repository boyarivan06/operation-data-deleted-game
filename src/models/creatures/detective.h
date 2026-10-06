#ifndef DETECTIVE_H
#define DETECTIVE_H
#include "creature.h"
#include "inventarior.h"
#include "fighter.h"
#include "../items/medicine.h"

/**
 * @brief Класс оперативника - обладает свойствами и возможностями всех типов тварей. За него играет пользователь
 */
class Detective : public Inventarior, public Fighter, public Creature {

public:
    void treat(std::shared_ptr<Medicine> &medicine); ///< Полечиться аптечкой (если возможно)

    Detective(int _max_health, int _max_time_points, int _distance_of_view, float _accuracy, int _close_combat_strength,
              float _speed, int strength); ///< Конструктор с указанием максимального здоровья, максимальной энергии, дистанции обзора, точности стрельбы и силы в ближнем бою

    Detective(); ///< Конструктор по умолчанию
};

#endif
