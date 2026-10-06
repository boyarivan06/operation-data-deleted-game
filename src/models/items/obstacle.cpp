#include "obstacle.h"

bool Obstacle::is_breakable() const {
    return breakable;
}

void Obstacle::set_breakability(const bool b_able) {
    breakable = b_able;
}
