#include <gtest/gtest.h>
#include "obstacle.h"


TEST(Obstacle, CreateGet) {
    Obstacle obstacle;
    EXPECT_EQ(obstacle.is_breakable(), false);
    Obstacle obstacle2({5,6});
    EXPECT_EQ(obstacle2.get_position().x, 5);
    Obstacle obstacle3("wall", GREEN, {1,1}, {4, 5}, {2,1});
    EXPECT_EQ(obstacle3.get_displacement().x, 2);
}