/**
 * @file custom_vector.h
 * @brief Реализация шаблонного класса собственного вектора
 * @author Ivan Boiarintsev
 * @version 1.1
 * @date 2025
*/
#ifndef CUSTOM_VECTOR_H
#define CUSTOM_VECTOR_H

#include <stdexcept>
#include <utility>
#include <algorithm>
#include <format>
#include <ostream>
#include <memory>
#include <ranges>
#define DEFAULT_CAPACITY 16

/**
 * @brief Класс вектора (шаблон)
 * @tparam T Тип элемента вектора
 */
template<typename T>
class CustomVector {
    T* data = nullptr; ///< Массив элементов
    int capacity = 0; ///< Ёмкость массива (размер выделенного блока памяти)
    int size = 0; ///< Количество элементов

    /**
     * @brief Уничтожает все элементы в массиве
     */
    void destroy_elements() {
        for (int i = 0; i < size; ++i) {
            data[i].~T();
        }
    }

    /**
     * @brief Освобождает память массива
     */
    void deallocate() {
        if (data) {
            ::operator delete(data);
            data = nullptr;
        }
    }

    /**
     * @brief Увеличение выделенного под массив блока памяти
     * @param new_capacity Новый размер блока памяти
     */
    void realloc(int new_capacity) {
        if (new_capacity <= 0) {
            new_capacity = 1;
        }
        if (new_capacity < size) {
            new_capacity = size;
        }
        if (new_capacity == capacity) {
            return;
        }

        // Выделяем новую сырую память
        T* new_data = static_cast<T*>(::operator new(new_capacity * sizeof(T)));

        // Перемещаем существующие элементы
        for (int i = 0; i < size; ++i) {
            new(&new_data[i]) T(std::move(data[i]));  // Placement new с перемещением
            data[i].~T();  // Уничтожаем старый объект
        }

        // Освобождаем старую память
        ::operator delete(data);

        // Обновляем указатели и емкость
        data = new_data;
        capacity = new_capacity;
    }

    /**
     * @brief Копирует элементы из другого вектора
     * @param other Исходный вектор
     */
    void copy_from(const CustomVector& other) {
        capacity = other.capacity;
        size = other.size;
        data = static_cast<T*>(::operator new(capacity * sizeof(T)));

        for (int i = 0; i < size; ++i) {
            new(&data[i]) T(other.data[i]);  // Placement new с копированием
        }
    }

public:
    /**
     * @brief Конструктор по умолчанию
     */
    CustomVector() : capacity(DEFAULT_CAPACITY), size(0) {
        data = static_cast<T*>(::operator new(capacity * sizeof(T)));
    }

    /**
     * @brief Конструктор с начальной емкостью
     * @param initial_capacity Начальная емкость вектора
     */
    explicit CustomVector(int initial_capacity) : capacity(initial_capacity), size(0) {
        if (capacity <= 0) capacity = DEFAULT_CAPACITY;
        data = static_cast<T*>(::operator new(capacity * sizeof(T)));
    }

    /**
     * @brief Конструктор из списка инициализации
     */
    CustomVector(std::initializer_list<T> args) : capacity(std::max(static_cast<int>(args.size()), DEFAULT_CAPACITY)), size(0) {
        data = static_cast<T*>(::operator new(capacity * sizeof(T)));
        for (const T& arg : args) {
            new(&data[size]) T(arg);  // Placement new
            ++size;
        }
    }

    /**
     * @brief Конструктор копирования
     * @param other Исходный вектор
     */
    CustomVector(const CustomVector& other) {
        copy_from(other);
    }

    /**
     * @brief Оператор присваивания копированием
     * @param other Исходный вектор
     * @return Ссылку на этот вектор
     */
    CustomVector& operator=(const CustomVector& other) {
        if (this != &other) {
            // Освобождаем текущие ресурсы
            destroy_elements();
            deallocate();

            // Копируем новые
            copy_from(other);
        }
        return *this;
    }

    /**
     * @brief Перемещающий конструктор
     * @param other Исходный вектор (rvalue reference)
     */
    CustomVector(CustomVector&& other) noexcept
        : data(other.data), capacity(other.capacity), size(other.size) {
        other.data = nullptr;
        other.capacity = 0;
        other.size = 0;
    }

    /**
     * @brief Перемещающий оператор присваивания
     * @param other Исходный вектор (rvalue reference)
     * @return Ссылку на этот вектор
     */
    CustomVector& operator=(CustomVector&& other) noexcept {
        if (this != &other) {
            // Освобождаем текущие ресурсы
            destroy_elements();
            deallocate();

            // Забираем ресурсы у other
            data = other.data;
            capacity = other.capacity;
            size = other.size;

            // Обнуляем other
            other.data = nullptr;
            other.capacity = 0;
            other.size = 0;
        }
        return *this;
    }

    /**
     * @brief Деструктор
     */
    ~CustomVector() {
        destroy_elements();
        deallocate();
    }

    /**
     * @brief Добавить элемент в конец вектора (копированием)
     * @param value Новый элемент
     */
    void push_back(const T& value) {
        if (size >= capacity) {
            realloc(capacity * 2);
        }
        new(&data[size]) T(value);  // Placement new с копированием
        ++size;
    }

    /**
     * @brief Добавить элемент в конец вектора (перемещением)
     * @param value Новый элемент
     */
    void push_back(T&& value) {
        if (size >= capacity) {
            realloc(capacity * 2);
        }
        new(&data[size]) T(std::move(value));  // Placement new с перемещением
        ++size;
    }

    /**
     * @brief Создает элемент на месте в конце вектора
     * @tparam Args Типы аргументов конструктора
     * @param args Аргументы для конструктора T
     * @return Ссылку на созданный элемент
     */
    template<typename... Args>
    T& emplace_back(Args&&... args) {
        if (size >= capacity) {
            realloc(capacity * 2);
        }
        new(&data[size]) T(std::forward<Args>(args)...);  // Perfect forwarding
        return data[size++];
    }

    /**
     * @brief Увеличить ёмкость вектора
     * @param n - новый размер
     */
    void reserve(int n) {
        if (n <= capacity) return;
        realloc(n);
    }

    /**
     * @brief Изменить размер вектора
     * @param new_size Новый размер
     * @param value Значение для новых элементов (если увеличиваем размер)
     */
    void resize(int new_size, const T& value = T()) {
        if (new_size < 0) return;

        if (new_size > capacity) {
            realloc(std::max(new_size, capacity * 2));
        }

        // Уничтожаем лишние элементы если уменьшаем размер
        if (new_size < size) {
            for (int i = new_size; i < size; ++i) {
                data[i].~T();
            }
        }
        // Добавляем новые элементы если увеличиваем размер
        else if (new_size > size) {
            for (int i = size; i < new_size; ++i) {
                new(&data[i]) T(value);
            }
        }

        size = new_size;
    }

    /**
     * @brief Проверка на отсутствие элементов
     * @return true/false
     */
    [[nodiscard]] bool empty() const {
        return size == 0;
    }

    /**
     * @brief Итератор начала массива (используется в методах класса)
     * @return Указатель на первый элемент вектора
     */
    T* begin() {
        return data;
    }

    /**
     * @brief Константный итератор начала массива
     * @return Указатель на первый элемент вектора
     */
    const T* begin() const {
        return data;
    }

    /**
     * @brief Итератор конца массива (используется в методах класса)
     * @return Указатель за последний элемент вектора
     */
    T* end() {
        return data + size;
    }

    /**
     * @brief Константный итератор конца массива
     * @return Указатель за последний элемент вектора
     */
    const T* end() const {
        return data + size;
    }

    /**
     * @brief Вставка элемента в конкретную позицию
     * @param pos Куда вставлять
     * @param value Элемент для вставки
     */
    void insert(T* pos, const T& value) {
        int index = pos - data;
        if (index < 0 || index > size) {
            throw std::out_of_range("Index Error in insert");
        }

        if (size >= capacity) {
            realloc(capacity * 2);
        }

        // Сдвигаем элементы вправо
        for (int i = size; i > index; --i) {
            new(&data[i]) T(std::move(data[i - 1]));
            data[i - 1].~T();
        }

        // Вставляем новый элемент
        new(&data[index]) T(value);
        ++size;
    }

    /**
     * @brief Очистка массива (сохраняет емкость)
     */
    void clear() {
        destroy_elements();
        size = 0;
    }

    /**
     * @brief Удаление элемента из вектора
     * @param pos Позиция удаляемого элемента
     */
    void erase(T* pos) {
        int index = pos - data;
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index Error in erase");
        }

        // Уничтожаем удаляемый элемент
        data[index].~T();

        // Сдвигаем оставшиеся элементы влево
        for (int i = index; i < size - 1; ++i) {
            new(&data[i]) T(std::move(data[i + 1]));
            data[i + 1].~T();
        }
        --size;
    }

    /**
     * @brief Доступ к элементу по индексу
     * @param index Индекс элемента
     * @return Элемент, если он существует
     */
    T& operator[](int index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index Error");
        }
        return data[index];
    }

    /**
     * @brief Константный доступ к элементу по индексу
     * @param index Индекс элемента
     * @return Элемент, если он существует
     */
    const T& operator[](int index) const {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index Error");
        }
        return data[index];
    }

    /**
     * @brief Доступ к элементу по индексу с проверкой границ
     * @param index Индекс элемента
     * @return Элемент, если он существует
     */
    T& at(int index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index Error");
        }
        return data[index];
    }

    /**
     * @brief Константный доступ к элементу по индексу с проверкой границ
     * @param index Индекс элемента
     * @return Элемент, если он существует
     */
    const T& at(int index) const {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index Error");
        }
        return data[index];
    }

    /**
     * @brief Доступ к первому элементу
     * @return Первый элемент
     */
    T& front() {
        if (size == 0) throw std::out_of_range("Vector is empty");
        return data[0];
    }

    /**
     * @brief Доступ к последнему элементу
     * @return Последний элемент
     */
    T& back() {
        if (size == 0) throw std::out_of_range("Vector is empty");
        return data[size - 1];
    }

    /**
     * @brief Геттер для количества элементов в векторе
     * @return Количество элементов
     */
    [[nodiscard]] int get_size() const {
        return size;
    }

    /**
     * @brief Геттер для ёмкости вектора
     * @return Ёмкость
     */
    [[nodiscard]] int get_capacity() const {
        return capacity;
    }

    /**
     * @brief Освобождает неиспользуемую память
     */
    void shrink_to_fit() {
        if (size < capacity) {
            realloc(size);
        }
    }
};

/**
 * @brief Оператор вывода для CustomVector
 */
template<typename T>
std::ostream& operator<<(std::ostream& out, const CustomVector<T>& vector) {
    out << "[";
    for (int i = 0; i < vector.get_size(); ++i) {
        if (i > 0) out << ", ";
        out << vector[i];
    }
    out << "]";
    return out;
}

#endif // CUSTOM_VECTOR_H