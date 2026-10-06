#include <gtest/gtest.h>
#include <gtest/internal/custom/gtest.h>

#include "vector2_operations.h"
#include "models/base/game_object.h"
#include "service/config.h"


TEST(GameObject, CreateAngGet) {
    auto gb1 = GameObject({1, 8});
    auto gb2 = GameObject("object number 2", BLUE, {4, 6}, {3, 4});
    auto gb3 = GameObject("object number 4", GREEN, {8, 2}, {2, 1},
        {FIELD_X, FIELD_Y});
    auto field_disp = Vector2{FIELD_X, FIELD_Y};
    EXPECT_EQ(gb1.get_size() == Vector2({   1, 1}), true);
    EXPECT_EQ(gb2.get_name(), "object number 2");
    EXPECT_EQ(gb2.get_size(), Vector2({4,6}));
    EXPECT_EQ(gb3.get_displacement(), field_disp);
}

TEST(GameObject, GetSet) {
    auto gb = GameObject();
    gb.set_displacement({56, 34});
    EXPECT_EQ(gb.get_displacement(), Vector2({56, 34}));
    gb.set_position({44, 5});
    EXPECT_EQ(gb.get_position(), Vector2({44, 5}));
}