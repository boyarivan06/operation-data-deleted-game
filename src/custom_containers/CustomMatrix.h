#ifndef MATRIX_H
#define MATRIX_H
#include <iostream>

#include "custom_vector.h"


/**
 * @brief Класс матрицы
 * @tparam T Тип элементов
 */
template<typename T>
class CustomMatrix : public CustomVector<CustomVector<T>> {
public:
    CustomMatrix() = default;
    /**
     * @brief Создание через список инициализации
     * @param args Список векторов (списков инициализации)
     */
    CustomMatrix(std::initializer_list<CustomVector<T>> args) {
        this->reserve(args.size());
        for (auto vec : args) {
            this->emplace_back(vec);
        }
    }
    /**
     * @brief Создать матрицу с заданным размеров
     * @param height Высота
     * @param width Ширина
     */
    CustomMatrix(const int height, const int width) {
        this->reserve(height);
        for (int i = 0; i < height; i++) {
            CustomVector<T> vec;
            for (int j = 0; j < width; j++) {
                vec.emplace_back(T());
            }
            this->emplace_back(std::move(vec));
        }
    }
    /**
     * @brief Узнать размер матрицы
     * @return Пару - высота и ширина
     */
    std::pair<int, int>get_matrix_size() {
        //if (!this[0]) return {this->get_size(), 0};
        return {this->get_size(), (*this)[0].get_size()};
    }
};

template<typename T>
std::ostream& operator<< (std::ostream& out, CustomMatrix<T> matrix) {
    for (int i = 0; i < matrix.get_size(); i++) {
        out << matrix[i] << std::endl;
    }
    return out;
}

#endif
