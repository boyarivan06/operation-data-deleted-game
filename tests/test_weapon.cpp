#include <exceptions.h>
#include <gtest/gtest.h>
#include "weapon.h"

TEST(Weapon, CreateGet) {
    Weapon w1{};
    Weapon w2(10, 20, 10, 1);
    EXPECT_EQ(w1.get_bullet_count(), 10);
    EXPECT_EQ(w2.get_damage(), 10);
    EXPECT_EQ(w2.get_distance(), 10);
}

TEST(Weapon, Strike) {
    Weapon w(1, 1, 1, 1);
    EXPECT_THROW(w.strike(), WeaponEmptyError);
}