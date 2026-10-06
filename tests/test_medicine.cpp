#include <gtest/gtest.h>
#include "medicine.h"

TEST(Medicine, CreateGet) {
    Medicine m(5, 2);
    EXPECT_EQ(m.get_treat_time(), 2);
    EXPECT_EQ(m.get_remaining_health(), 5);
    int creature_health = 50;
    m.heal(creature_health, 100);
    EXPECT_EQ(creature_health, 55);
    EXPECT_EQ(m.get_remaining_health(), 0);
}