#include "wild_beast.h"

#include "detective.h"

void WildBeast::move(Field & field) {
    if (is_moving()) return;

    if (current_sleep_time > 0) {
        current_sleep_time -= GetFrameTime();
        return;
    }

    auto detective = field.find_nearest<Detective>(cell_position);
    if (!detective) return;

    Vector2 target_pos = detective->get_position();
    
    Vector2 diff = cell_position - target_pos;
    if (abs(diff.x) + abs(diff.y) <= 1.0f) {
        detective->cause_damage(close_fight_damage);
        current_sleep_time = sleep_time; // Уходим в сон
        return;
    }

    Creature::move(target_pos, field);

}
void WildBeast::die(Field& field) {
    field.remove_object(shared_from_this());
}