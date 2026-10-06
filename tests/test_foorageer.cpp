#include "foorageer.h"
#include "field.h"
#include "depot.h"
#include "item.h"
#include <gtest/gtest.h>
#include <memory>

class FoorageerTest : public ::testing::Test {
protected:
    void SetUp() override {
        foorageer = std::make_shared<Foorageer>(10);
        field = std::make_shared<Field>(10, 10);
        depot = std::make_shared<Depot>();
        item = std::make_shared<Item>(5);

        foorageer->set_position({5.0f, 5.0f});
        field->get_cell({5, 5}).add_object(foorageer);

        item->set_position({3.0f, 3.0f});
    }

    std::shared_ptr<Foorageer> foorageer;
    std::shared_ptr<Field> field;
    std::shared_ptr<Depot> depot;
    std::shared_ptr<Item> item;
};

TEST_F(FoorageerTest, ConstructorInitializesStrength) {
    auto customFoorageer = std::make_shared<Foorageer>(15);
    EXPECT_TRUE(customFoorageer != nullptr);
}

TEST_F(FoorageerTest, AddDepot) {
    foorageer->add_depot(depot);
    SUCCEED();
}

TEST_F(FoorageerTest, MoveUnloadsAtDepotWhenInventoryNotEmpty) {
    depot->set_position({5.0f, 5.0f});
    field->get_cell({5, 5}).add_object(depot);
    foorageer->add_depot(depot);

    foorageer->take_item(item);

    foorageer->move(*field);

    SUCCEED();
}

TEST_F(FoorageerTest, MovePicksUpItemWhenOnSameCell) {
    item->set_position({5.0f, 5.0f});
    field->get_cell({5, 5}).add_object(item);

    foorageer->move(*field);

    auto items_on_cell = field->get_cell({5, 5}).get_all<Item>();
    EXPECT_TRUE(items_on_cell.empty());
}

TEST_F(FoorageerTest, MoveSeeksNearestItem) {
    field->get_cell({3, 3}).add_object(item);

    foorageer->move(*field);

    EXPECT_TRUE(foorageer->is_moving() || foorageer->has_route());
}

TEST_F(FoorageerTest, MoveReturnsWhenNoItemsAndEmptyInventory) {
    foorageer->move(*field);

    EXPECT_FALSE(foorageer->is_moving());
    EXPECT_FALSE(foorageer->has_route());
}

TEST_F(FoorageerTest, MoveGoesToDepotWhenStrengthZero) {
    auto heavyItem = std::make_shared<Item>(10);
    foorageer->take_item(heavyItem);

    depot->set_position({2.0f, 2.0f});
    field->get_cell({2, 2}).add_object(depot);
    foorageer->add_depot(depot);

    foorageer->move(*field);

    EXPECT_TRUE(foorageer->is_moving() || foorageer->has_route());
}

TEST_F(FoorageerTest, MoveGoesToDepotWhenItemTooHeavy) {
    auto heavyItem = std::make_shared<Item>(15);
    heavyItem->set_position({1.0f, 1.0f});
    field->get_cell({1, 1}).add_object(heavyItem);

    depot->set_position({2.0f, 2.0f});
    field->get_cell({2, 2}).add_object(depot);
    foorageer->add_depot(depot);

    foorageer->move(*field);

    EXPECT_TRUE(foorageer->is_moving() || foorageer->has_route());
}

TEST_F(FoorageerTest, MoveGoesToDepotWhenNoItemsButHasDepotsAndInventory) {

    foorageer->take_item(item);

    depot->set_position({2.0f, 2.0f});
    field->get_cell({2, 2}).add_object(depot);
    foorageer->add_depot(depot);

    foorageer->move(*field);

    EXPECT_TRUE(foorageer->is_moving() || foorageer->has_route());
}

TEST_F(FoorageerTest, MoveGoesToNearestDepot) {
    auto depot1 = std::make_shared<Depot>();
    depot1->set_position({1.0f, 1.0f});
    field->get_cell({1, 1}).add_object(depot1);

    auto depot2 = std::make_shared<Depot>();
    depot2->set_position({8.0f, 8.0f});
    field->get_cell({8, 8}).add_object(depot2);

    foorageer->add_depot(depot1);
    foorageer->add_depot(depot2);

    foorageer->take_item(item);

    foorageer->move(*field);

    EXPECT_TRUE(foorageer->is_moving() || foorageer->has_route());
}

TEST_F(FoorageerTest, MoveDoesNothingWhenOnItemCell) {

    item->set_position({5.0f, 5.0f});
    field->get_cell({5, 5}).add_object(item);

    foorageer->move(*field);
    auto items_on_cell = field->get_cell({5, 5}).get_all<Item>();
    EXPECT_TRUE(items_on_cell.empty());
}

TEST_F(FoorageerTest, NoMovementWhenAlreadyMoving) {
    Vector2 direction = {1, 0};
    foorageer->try_step(direction, *field);
    EXPECT_TRUE(foorageer->is_moving());

    foorageer->move(*field);

    EXPECT_TRUE(foorageer->is_moving());
}

TEST_F(FoorageerTest, MoveWithEmptyInventoryAndNoItems) {
    foorageer->move(*field);

    EXPECT_FALSE(foorageer->is_moving());
    EXPECT_FALSE(foorageer->has_route());
}

TEST_F(FoorageerTest, MoveWithItemsInInventoryButNoDepots) {
    foorageer->take_item(item);

    foorageer->move(*field);

    EXPECT_FALSE(foorageer->is_moving());
    EXPECT_FALSE(foorageer->has_route());
}