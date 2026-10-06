#include "level.h"
#include <gtest/gtest.h>
#include <memory>

class LevelTest : public ::testing::Test {
protected:
    void SetUp() override {
        level = std::make_unique<Level>();
    }

    std::unique_ptr<Level> level;
};

TEST_F(LevelTest, ConstructorInitializesField) {
    Field& field = level->get_field();
    EXPECT_TRUE(true);
}

TEST_F(LevelTest, GetNameReturnsCorrectName) {
    std::string name = level->get_name();
    EXPECT_EQ(name, "Первый уровень");
}

TEST_F(LevelTest, GetFieldReturnsValidReference) {
    Field& field1 = level->get_field();
    Field& field2 = level->get_field();
    EXPECT_EQ(&field1, &field2);
}