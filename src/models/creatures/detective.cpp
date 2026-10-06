#include "detective.h"
#include "../../service/exceptions.h"


void Detective::treat(std::shared_ptr<Medicine>& medicine) {
    if (this->health >= max_health) throw ObjectParameterException("Creature health is 100%");
    if (medicine->get_remaining_health() <= 0) throw ObjectParameterException("Medicine is used");
    if (time_points < medicine->get_treat_time()) throw ObjectParameterException("Creature hasn't enough time points");
    // TODO: anything else?
    medicine->heal(health, max_health);
    time_points -= medicine->get_treat_time();
}

Detective::Detective(int _max_health, int _max_time_points, int _distance_of_view, float _accuracy, int _close_combat_strength, float _speed, int strength): Inventarior(strength) {
    max_health = _max_health;
    max_time_points = _max_time_points;
    distance_of_view = _distance_of_view;
    accuracy = _accuracy;
    close_combat_strength = _close_combat_strength;
    speed = _speed;
}

Detective::Detective(): Inventarior(11) {
    max_health = 10;
    max_time_points = 10;
    distance_of_view = 10;
    accuracy = 1;
    close_combat_strength = 10;
    speed = 1;
}
