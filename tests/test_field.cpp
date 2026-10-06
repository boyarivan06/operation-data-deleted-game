#include <gtest/gtest.h>
#include "field.h"

class FieldTest : public ::testing::Test {
protected:
    void SetUp() override {
        field = std::make_shared<Field>(10, 10);
    }

    std::shared_ptr<Field> field;
};

TEST_F(FieldTest, ConstructorInitialization) {
    auto size = field->get_size();
    EXPECT_EQ(size.first, 10);
    EXPECT_EQ(size.second, 10);
}

TEST_F(FieldTest, GetCellValidPosition) {
    Vector2 pos = {0, 0};
    EXPECT_NO_THROW(field->get_cell(pos));
}

TEST_F(FieldTest, GetCellInvalidPosition) {
    Vector2 pos = {15, 15};
    EXPECT_THROW(field->get_cell(pos), std::out_of_range);
}

TEST_F(FieldTest, IncludePointValid) {
    Vector2 pos = {5, 5};
    EXPECT_TRUE(field->include_point(pos));
}

TEST_F(FieldTest, IncludePointInvalid) {
    Vector2 pos = {15, 15};
    EXPECT_FALSE(field->include_point(pos));
}

TEST_F(FieldTest, IsValidCellValid) {
    Vector2 pos = {3, 3};
    EXPECT_TRUE(field->is_valid_cell(pos));
}

TEST_F(FieldTest, IsValidCellInvalid) {
    Vector2 pos = {-1, -1};
    EXPECT_FALSE(field->is_valid_cell(pos));
}

TEST_F(FieldTest, ConvertGlobalToCell) {
    Vector2 global = {41, 8.3f};
    Vector2 cell = Field::convert_global_to_cell(global);
    EXPECT_EQ(cell.x, 2);
    EXPECT_EQ(cell.y, 1);
}

TEST_F(FieldTest, AStarRouteValid) {
    Vector2 start = {0, 0};
    Vector2 end = {2, 2};
    auto route = field->a_star_route(start, end);
    EXPECT_FALSE(route.empty());
}

TEST_F(FieldTest, CheckSquareTemplateNoObjects) {
    Vector2 start = {5, 5};
    auto result = field->check_square<GameObject>(1, start);
    EXPECT_EQ(result, nullptr);
}

TEST_F(FieldTest, FindNearestTemplateNoObjects) {
    Vector2 start = {5, 5};
    auto result = field->find_nearest<GameObject>(start);
    EXPECT_EQ(result, nullptr);
}

TEST_F(FieldTest, OperatorBracketValid) {
    EXPECT_NO_THROW((*field)[0]);
}

TEST_F(FieldTest, OperatorBracketInvalid) {
    EXPECT_THROW((*field)[15], std::out_of_range);
}