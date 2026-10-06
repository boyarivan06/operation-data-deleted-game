#include "medicine.h"

Medicine::Medicine(int _health, int _treat_time) : Item(_health) {
    health = _health;
    treat_time = _treat_time;
}

void Medicine::heal(int & creature_health, int max_creature_health) {
    int needed = max_creature_health-creature_health;
    if (health <= needed) {
        creature_health += health;
        health = 0;
    }
    else {
        health -= needed;
        creature_health = max_creature_health;
    }
}

int Medicine::get_remaining_health() const {
    return health;
}

int Medicine::get_treat_time() const {
    return treat_time;
}


