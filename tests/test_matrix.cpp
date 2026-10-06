#include "../custom_containers/CustomMatrix.h"
#include <gtest/gtest.h>
TEST(CustomMatrix, Create) {
    auto m1 = CustomMatrix<int>();
    CustomVector<int> vec{};
    for (int i = 0; i < 5;i++) {
        vec.emplace_back(i*i);
    }
    m1.emplace_back(vec);
    for (int& elem : vec) {
        elem += 7;
    }
    m1.emplace_back(vec);
    CustomMatrix<std::string> m2(4, 6);
    m2[2][3] = "hello world";
    EXPECT_EQ(m2[2][2], "");
    m2[2][3] += "bebebe";
    EXPECT_EQ(m2[2][3], "hello worldbebebe");
    EXPECT_EQ(m1[1][3], 16);
    constexpr std::pair size{2,5};
    EXPECT_EQ(m1.get_matrix_size(), size);
    CustomMatrix<int> m11{{4, 5, 6}, {3, 43, 84, 75}};
    EXPECT_EQ(m11[1][2], 84);
}
TEST(CustomMatrix, Access) {
    auto word = "abcdefghijklmnopqrst";
    int i = 0;
    CustomMatrix<char> matrix(4, 5);
    for (auto& line : matrix) {
        for (auto& elem: line) {
            elem = word[i];
            i++;
        }
    }
    srand(static_cast<unsigned>(clock()));
    int x = rand() % 4;
    int y = rand() % 5;
    EXPECT_EQ(matrix[x][y], word[5*x+y]);
    int x2 = rand() % 4;
    int y2 = rand() % 5;
    EXPECT_EQ(matrix[x2][y2], word[5*x2+y2]);
    EXPECT_THROW(matrix[5][5], std::out_of_range);
}
/*TEST(CustomMatrix, Output) {
    int h = rand() % 100, w = rand() % 100;
    CustomMatrix<int> matrix(h, w);
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            matrix[i][j] = rand() % 1000;
        }
    }
    std::streambuf* old_cout_buffer = std::cout.rdbuf();
    std::stringstream captured_output;
    std::cout.rdbuf(captured_output.rdbuf());

    std::cout << matrix;
    std::cout.rdbuf(old_cout_buffer);
    std::string check;
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w-1; j++) {
            check += std::format("{}, ", matrix[i][j]);
        }
        check += std::format("{}\n", matrix[i][matrix[i].get_size()-1]);
    }
    EXPECT_EQ(captured_output.str(), check);
}*/