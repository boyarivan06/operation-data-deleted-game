#include "wild_beast.h"
#include "field.h"
#include "detective.h"
#include <gtest/gtest.h>
#include <memory>

class WildBeastTest : public ::testing::Test {
protected:
    void SetUp() override {
        wildBeast = std::make_shared<WildBeast>();
        field = std::make_shared<Field>(10, 10);
        detective = std::make_shared<Detective>();
        field->add_object(detective);
        field->add_object(wildBeast);
    }

    std::shared_ptr<WildBeast> wildBeast;
    std::shared_ptr<Field> field;
    std::shared_ptr<Detective> detective;
};

TEST_F(WildBeastTest, MoveTowardsDetective) {
    wildBeast->set_position({0, 0});
    detective->set_position({5, 5});
    field->get_cell({5, 5}).add_object(detective);

    wildBeast->move(*field);

    EXPECT_TRUE(wildBeast->is_moving() || wildBeast->has_route());
}

TEST_F(WildBeastTest, NoMoveWhenNoDetective) {
    wildBeast->set_position({0, 0});

    wildBeast->move(*field);

    EXPECT_FALSE(wildBeast->is_moving());
}

TEST_F(WildBeastTest, NoMoveWhenSamePosition) {
    wildBeast->set_position({3, 3});
    detective->set_position({3, 3});
    field->get_cell({3, 3}).add_object(detective);

    wildBeast->move(*field);

    EXPECT_FALSE(wildBeast->is_moving());
}

TEST_F(WildBeastTest, MoveToDetectivePosition) {
    wildBeast->set_position({1, 1});
    detective->set_position({4, 4});
    field->get_cell({4, 4}).add_object(detective);

    wildBeast->move(*field);

    EXPECT_TRUE(wildBeast->has_route());
}