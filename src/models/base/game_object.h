#ifndef BASE_H
#define BASE_H
#include <iostream>
#include <string>
#include <raylib.h>
#include <vector>



/**
 * @brief Класс игрового объекта
 */
class GameObject {
protected:
    std::string name;

    Vector2 cell_position = {0, 0};
    Vector2 real_position = {0,0};
    Vector2 size = {1,1};
    Vector2 displacement = {0,0};
    Color color = BLACK;

public:
    int resource_name=-1; // macro, ex: EMPTY_CELL_RESOURCE

    virtual ~GameObject() = default; ///< Деструктор по умолчанию

    Color get_color(); ///< Цвет объекта

    [[nodiscard]] Vector2 get_size() const; ///< Размер объекта

    std::string get_name(); ///< Название объекта

    [[nodiscard]] int get_resource() const; ///< Номер текстуры объекта

    std::vector<Vector2> route; ///< маршрут движения объекта
    GameObject() = default; ///< Конструктор по умолчанию
    explicit GameObject(Vector2 pos); ///< Конструктор с заданной позицией
    GameObject(std::string name, Color color, Vector2 size); ///< Конструктор с заданными названием, цветом и размером
    GameObject(const std::string& name, Color color, Vector2 size, Vector2 position); ///< Конструктор с заданными названием, цветом, размером и позицией
    GameObject(const std::string& name, Color color, Vector2 size, Vector2 position, Vector2 displacement); ///< Конструктор с заданными названием, цветом, размером, позицией и сдвигом отрисовки
    virtual void set_position(Vector2 new_pos); ///< Поменять позицию (без проверок на корректность)
    void set_name(std::string text); ///< Поменять название
    void set_displacement(Vector2 new_disp); ///< Поменять сдвиг отрисовки

    [[nodiscard]] Vector2 get_position() const; ///< Узнать позицию
    [[nodiscard]] Vector2 get_displacement() const; ///< Узнать сдвиг отрисовки
    void set_color(Color); ///< Поменять цвет

};


#endif
