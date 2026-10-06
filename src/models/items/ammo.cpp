#include "ammo.h"

Ammo::Ammo(int bc): Item(bc) {
    bullet_count = bc;
}

void Ammo::remove_bullets(int cnt) {
    if (cnt >= bullet_count) bullet_count = 0;
    else bullet_count -= cnt;
}

int Ammo::get_bullet_cnt() const {
    return bullet_count;
}
