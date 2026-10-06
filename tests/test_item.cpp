#include <gtest/gtest.h>
#include "item.h"

TEST(Item, CreateGetSet) {
    Item i = Item(3);
    EXPECT_EQ(i.get_weight(), 3);
    i.set_weight(2);
    EXPECT_EQ(i.get_weight(), 2);
    EXPECT_THROW(i.set_weight(-2), std::runtime_error);
}