#include <gtest/gtest.h>
#include "ammo.h"

TEST(Ammo, CreateGet) {
    srand(time(nullptr));
    for (int i = 0; i < 100; i++) {
        int frst = rand() % 100, scnd = rand()%50;
        auto ammo = Ammo(frst);
        EXPECT_EQ(ammo.get_bullet_cnt(), frst);
        ammo.remove_bullets(scnd);
        EXPECT_EQ(ammo.get_bullet_cnt(), frst-scnd > 0? frst-scnd : 0);
    }
}