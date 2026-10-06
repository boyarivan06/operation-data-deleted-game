#ifndef CELL_H
#define CELL_H
#include <vector>

#include "item.h"

class GameObject;

/**
 * @brief Класс клетки поля
 */
class Cell{

    std::vector<std::shared_ptr<GameObject>> objects;
public:
    ~Cell(); ///< Деструктор (по умолчанию)
    std::vector<std::shared_ptr<GameObject>>& get_objects(); ///< Получить объекты из клетки
    void add_object(const std::shared_ptr<GameObject> & shared); ///< Добавить объект в клетку
    void remove_object(const std::shared_ptr<GameObject> &); ///< Удалить объект из клетки
    bool is_obstacle(); ///< Есть ли в клетке препятствие


    /**
     * @brief Получить первый объект запрашиваемого типа из клетки, если есть
     * @tparam T Тип объекта (должен наследоваться от GameObject)
     * @return Указатель на объект или nullptr
     */
    template <typename T>
    requires std::derived_from<T, GameObject>
    std::shared_ptr<T> get_first() {
        if (!objects.empty()) {
            for (auto& obj : objects) {
                if (std::dynamic_pointer_cast<T>(obj) != nullptr) return std::dynamic_pointer_cast<T>(obj);
            }
        }
        return nullptr;
    }

    /**
     * @brief Получить все объекты определённого типа из клетки
     * @tparam T Тип объектов (должен наследоваться от GameObject)
     * @return Вектор объектов (может быть пустым)
     */
    template <typename T>
    requires std::derived_from<T, GameObject>
    std::vector<std::shared_ptr<T>> get_all() const {
        std::vector<std::shared_ptr<T>> result;
        for (const auto& obj : objects) {
            auto casted = std::dynamic_pointer_cast<T>(obj);
            if (casted != nullptr) {
                result.emplace_back(casted);
            }
        }
        return result;
    }
};

#endif