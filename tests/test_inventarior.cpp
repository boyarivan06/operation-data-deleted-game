#include "inventarior.h"
#include <gtest/gtest.h>
#include <memory>

class InventariorTest : public ::testing::Test {
protected:
    void SetUp() override {
        inventarior = std::make_shared<Inventarior>(10);
        item = std::make_shared<Item>(10);
    }

    std::shared_ptr<Inventarior> inventarior;
    std::shared_ptr<Item> item;
};

TEST_F(InventariorTest, Constructor) {
    EXPECT_TRUE(inventarior != nullptr);
}

TEST_F(InventariorTest, TakeItem) {
    inventarior->take_item(item);
    SUCCEED();
}
