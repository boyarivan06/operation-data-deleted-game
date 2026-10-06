#include "vector2_operations.h"
#include <gtest/gtest.h>

#include "exceptions.h"

TEST(Vector2OperationsTest, EqualityOperator) {
    Vector2 v1 = {1.0f, 2.0f};
    Vector2 v2 = {1.0f, 2.0f};
    Vector2 v3 = {1.0f, 3.0f};

    EXPECT_TRUE(v1 == v2);
    EXPECT_FALSE(v1 == v3);
}

TEST(Vector2OperationsTest, AdditionOperator) {
    Vector2 v1 = {1.0f, 2.0f};
    Vector2 v2 = {3.0f, 4.0f};
    Vector2 result = v1 + v2;

    EXPECT_FLOAT_EQ(result.x, 4.0f);
    EXPECT_FLOAT_EQ(result.y, 6.0f);
}

TEST(Vector2OperationsTest, SubtractionOperator) {
    Vector2 v1 = {5.0f, 6.0f};
    Vector2 v2 = {2.0f, 3.0f};
    Vector2 result = v1 - v2;

    EXPECT_FLOAT_EQ(result.x, 3.0f);
    EXPECT_FLOAT_EQ(result.y, 3.0f);
}

TEST(Vector2OperationsTest, CompoundAddition) {
    Vector2 v1 = {1.0f, 2.0f};
    Vector2 v2 = {3.0f, 4.0f};
    v1 += v2;

    EXPECT_FLOAT_EQ(v1.x, 4.0f);
    EXPECT_FLOAT_EQ(v1.y, 6.0f);
}

TEST(Vector2OperationsTest, CompoundSubtraction) {
    Vector2 v1 = {5.0f, 6.0f};
    Vector2 v2 = {2.0f, 3.0f};
    v1 -= v2;

    EXPECT_FLOAT_EQ(v1.x, 3.0f);
    EXPECT_FLOAT_EQ(v1.y, 3.0f);
}

TEST(Vector2OperationsTest, ScalarMultiplication) {
    Vector2 v = {2.0f, 3.0f};
    Vector2 result1 = v * 2;
    Vector2 result2 = 3 * v;

    EXPECT_FLOAT_EQ(result1.x, 4.0f);
    EXPECT_FLOAT_EQ(result1.y, 6.0f);
    EXPECT_FLOAT_EQ(result2.x, 6.0f);
    EXPECT_FLOAT_EQ(result2.y, 9.0f);
}

TEST(Vector2OperationsTest, CompoundScalarMultiplication) {
    Vector2 v = {2.0f, 3.0f};
    v *= 2;

    EXPECT_FLOAT_EQ(v.x, 4.0f);
    EXPECT_FLOAT_EQ(v.y, 6.0f);
}

TEST(Vector2OperationsTest, NormalizeCoords) {
    Vector2 v1 = {10.0f, 0.0f};
    Vector2 result1 = normalize_coords(v1);
    EXPECT_FLOAT_EQ(result1.x, 1.0f);
    EXPECT_FLOAT_EQ(result1.y, 0.0f);

    Vector2 v2 = {0.0f, -10.0f};
    Vector2 result2 = normalize_coords(v2);
    EXPECT_FLOAT_EQ(result2.x, 0.0f);
    EXPECT_FLOAT_EQ(result2.y, -1.0f);

    Vector2 v3 = {7.0f, 7.0f};
    Vector2 result3 = normalize_coords(v3);
    EXPECT_FLOAT_EQ(result3.x, 1.0f);
    EXPECT_FLOAT_EQ(result3.y, 1.0f);
}

TEST(Vector2OperationsTest, MakeInteger) {
    Vector2 v1 = {2.3f, 4.7f};
    Vector2 result1 = make_integer(v1);
    EXPECT_FLOAT_EQ(result1.x, 2.0f);
    EXPECT_FLOAT_EQ(result1.y, 5.0f);

    Vector2 v2 = {2.6f, 4.2f};
    Vector2 result2 = make_integer(v2);
    EXPECT_FLOAT_EQ(result2.x, 3.0f);
    EXPECT_FLOAT_EQ(result2.y, 4.0f);
}

TEST(Vector2OperationsTest, Module) {
    Vector2 v = {3.0f, 4.0f};
    float result = module(v);
    EXPECT_FLOAT_EQ(result, 5.0f);
}

TEST(Vector2OperationsTest, RotateDestinationValid) {
    Vector2 dest = {1, 0};
    Vector2 result_left = rotate_destination(dest, true, 1);
    EXPECT_FLOAT_EQ(result_left.x, 1.0f);
    EXPECT_FLOAT_EQ(result_left.y, -1.0f);

    Vector2 result_right = rotate_destination(dest, false, 1);
    EXPECT_FLOAT_EQ(result_right.x, 1.0f);
    EXPECT_FLOAT_EQ(result_right.y, 1.0f);
}

TEST(Vector2OperationsTest, RotateDestinationInvalidThrows) {
    Vector2 invalid = {2, 0};
    EXPECT_THROW(rotate_destination(invalid, true, 1), ObjectParameterException);

    Vector2 invalid2 = {0.5f, 0.5f};
    EXPECT_THROW(rotate_destination(invalid2, true, 1), ObjectParameterException);
}