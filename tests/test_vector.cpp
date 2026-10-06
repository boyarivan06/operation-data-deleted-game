#include <format>
#include <custom_vector.h>
#include <gtest/gtest.h>

TEST(CustomVector, CreateCopyAndEmplace) {
    CustomVector<int> vec;
    auto vec2 = vec;
    vec.emplace_back(5);
    vec.emplace_back(3);
    vec2.emplace_back(1);
    vec2.emplace_back(55);
    auto vec3(vec2);
    vec3.emplace_back(111);
    EXPECT_EQ(std::format("{} {}", vec[0], vec[1]), "5 3");
    EXPECT_EQ(std::format("{} {}", vec2[0], vec2[1]), "1 55");
    EXPECT_EQ(std::format("{} {} {}", vec3[0], vec3[1], vec3[2]), "1 55 111");
    EXPECT_THROW(vec[10], std::out_of_range);
}

TEST(CustomVector, EmptyReserveBeginEnd) {
    CustomVector<char> vec;
    EXPECT_EQ(vec.empty(), true);
    vec.emplace_back('a');
    vec.emplace_back('b');
    vec.emplace_back('c');
    EXPECT_EQ(*vec.begin(), 'a');
    EXPECT_EQ(*(vec.end()-1), 'c');
    vec.reserve(40);
    EXPECT_EQ(vec.get_capacity(), 40);
}

TEST(CustomVector, InsertClearErase) {
    CustomVector<std::string> vec;
    vec.insert(vec.end(), "bububu");
    vec.insert(vec.begin(), "bababa");
    vec.insert(vec.begin()+1, "one more");
    EXPECT_EQ(vec.get_size(), 3);
    EXPECT_EQ(vec.get_capacity(), DEFAULT_CAPACITY);
    for (int i = 0; i < 100; i++) {
        vec.emplace_back("mmmmm");
    }
    EXPECT_EQ(vec.get_size(), 103);
    EXPECT_EQ(vec.get_capacity(), 128);
    EXPECT_EQ(vec[2], "bububu");
    for (int i = 0; i < 100; i++) {
        vec.insert(vec.end(), "ooo");
    }
    EXPECT_THROW(vec[1000], std::out_of_range);
    vec.erase(vec.begin()+2);
    const auto obj = vec[2];
    EXPECT_EQ(obj, "mmmmm");
    EXPECT_THROW(vec.erase(vec.begin()+1000), std::out_of_range);
}


TEST(CustomVectorMoveConstructor, MoveEmptyVector) {
    CustomVector<int> empty_vec;
    CustomVector<int> moved_vec(std::move(empty_vec));

    EXPECT_EQ(moved_vec.get_size(), 0);
    EXPECT_EQ(moved_vec.get_capacity(), DEFAULT_CAPACITY);
    EXPECT_TRUE(moved_vec.empty());

    // Original vector should be in valid but empty state
    EXPECT_EQ(empty_vec.get_size(), 0);
    EXPECT_EQ(empty_vec.get_capacity(), 0);
    EXPECT_TRUE(empty_vec.empty());
}

TEST(CustomVectorMoveConstructor, MoveVectorWithElements) {
    CustomVector<int> original;
    original.emplace_back(1);
    original.emplace_back(2);
    original.emplace_back(3);

    int original_capacity = original.get_capacity();
    int original_size = original.get_size();

    CustomVector<int> moved_vec(std::move(original));

    // Check that moved vector has all elements
    EXPECT_EQ(moved_vec.get_size(), original_size);
    EXPECT_EQ(moved_vec.get_capacity(), original_capacity);
    EXPECT_EQ(moved_vec[0], 1);
    EXPECT_EQ(moved_vec[1], 2);
    EXPECT_EQ(moved_vec[2], 3);

    // Original vector should be empty
    EXPECT_EQ(original.get_size(), 0);
    EXPECT_EQ(original.get_capacity(), 0);
}



TEST(CustomVectorMoveConstructor, MovePreservesCapacity) {
    CustomVector<int> original;
    original.reserve(100);
    original.emplace_back(42);

    int original_capacity = original.get_capacity();

    CustomVector<int> moved_vec(std::move(original));

    EXPECT_EQ(moved_vec.get_capacity(), original_capacity);
    EXPECT_EQ(moved_vec[0], 42);
    EXPECT_EQ(original.get_capacity(), 0);
}

TEST(CustomVectorMoveConstructor, CanUseMovedVector) {
    CustomVector<std::string> original;
    original.emplace_back("hello");
    original.emplace_back("world");

    CustomVector<std::string> moved_vec(std::move(original));

    // Test that we can still use the moved vector
    moved_vec.emplace_back("test");
    EXPECT_EQ(moved_vec.get_size(), 3);
    EXPECT_EQ(moved_vec[0], "hello");
    EXPECT_EQ(moved_vec[1], "world");
    EXPECT_EQ(moved_vec[2], "test");

    // Original should not be usable for element access
    // But shouldn't crash either
    EXPECT_EQ(original.get_size(), 0);
}