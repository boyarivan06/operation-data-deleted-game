#include <gtest/gtest.h>
#include "beast.h"

TEST(Beast, CreateGet) {
    Beast b;
    Beast b2({0,2}, {5,6});
    EXPECT_EQ(b.get_armour(), 0);
    b2.set_armour(2);
    EXPECT_EQ(b2.get_armour(), 2);
}