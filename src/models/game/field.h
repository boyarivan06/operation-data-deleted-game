#ifndef FIELD_H
#define FIELD_H
#include "../base/cell.h"
#include "../custom_containers/CustomMatrix.h"
#include <utility>
#include "vector2_operations.h"
#include "obstacle.h"
#include <nlohmann/json.hpp>

class Beast;

/**
 * @brief Класс для игрового поля
 */
class Field {
    CustomMatrix<Cell> cells; ///< Клетки поля
    std::shared_ptr<std::vector<std::shared_ptr<Beast>>> bots = std::make_shared<std::vector<std::shared_ptr<Beast>>>(); ///< Управляемые игрой объекты
    std::vector<std::shared_ptr<GameObject>> projectiles; ///< Специальные объекты, не хранящиеся в клетках
public:
    std::pair<int, int> get_size(); ///< Узнать размер поля

    void add_object(const std::shared_ptr<GameObject> & shared); ///< Добавить объект на поле (в ячейку по его координатам)
    void remove_object(const std::shared_ptr<GameObject>& obj); ///< Удалить объект (из ячейки по координатам)
    Cell& get_cell(Vector2 vector2); ///< Получить клетку по координатам

    static Vector2 convert_global_to_cell(Vector2); ///< Конвертировать дробные координаты в целые (в какую клетку они попадают)

    Field(); ///< Конструктор по умолчанию
    Field(int h, int w); ///< Конструктор с заданными размерами
    CustomVector<Cell>& operator[](int); ///< Оператор [] для получения клетки по координатам через [i][j] - для любителей
    std::vector<Vector2> a_star_route(Vector2, Vector2);  ///< Создаёт маршрут, реализация в src/ext/a_star.cpp, возвращает вектор направлений {v1,v2,v3}, |vk|=1
    bool include_point(Vector2); ///< Содержит ли клетку по координатам
    bool is_valid_cell(Vector2 pos); ///< Клетка существует и не содержит препятствия

    [[nodiscard]] std::shared_ptr<std::vector<std::shared_ptr<Beast>>> get_bots() const {return bots;} ///< Получить ботов на поле

    //---- TEMPLATES ----//
    /*
     * @brief Функция для поиска объекта данного типа на заданном расстоянии от центра
     * @tparam T Тип объекта для поиска (наследуется от GameObject)
     * @param n Номер квадрата (1 - сторона 3, 2 - 5, etc.)
     * @param start Точка начала
     * @return Найденный объект или nullptr
     */
    template <typename T>
    requires std::derived_from<T, GameObject>
    std::shared_ptr<T> check_square(int n, const Vector2 start) {
        Vector2 current = start + Vector2{static_cast<float>(n),static_cast<float>(n)};
        std::vector<Vector2> displacements = {{-1, 0},{0, -1}, {1, 0}, {0,1}};
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 2*n + 1; j++) {
                if (include_point(current)) {
                    for (auto objects = get_cell(current).get_objects(); auto & object : objects) {
                        if (std::dynamic_pointer_cast<T>(object) != nullptr) return std::dynamic_pointer_cast<T>(object);
                    }
                }
                current += displacements[i];
            }
        }
        return nullptr;
    }

    /**
     * @brief Найти ближайший к точке объект заданного типа
     * @tparam T Тип объекта (наследуется от GameObject)
     * @param start Точка начала поиска
     * @return Найденный объект или nullptr
     */
    template <typename T>
    requires std::derived_from<T, GameObject>
    std::shared_ptr<T> find_nearest(Vector2 start) {
        int max_n = std::ranges::max({start.x, start.y, get_size().first-start.x, get_size().second-start.y}, {}, {});
        for (int i = 0; i < max_n; i++) {
            auto item = check_square<T>(i + 1, start);
            if (item != nullptr) return item;
        }
        return nullptr;
    }


    /**
     * @brief Добавить спецобъект
     * @param projectile Объект
     */
    void add_projectile(std::shared_ptr<GameObject> projectile) {
        projectiles.push_back(projectile);
    }
    /**
         * @brief Удалить спецобъект
         * @param projectile Объект
    */
    void remove_projectile(std::shared_ptr<GameObject> projectile) {
        projectiles.erase(std::remove(projectiles.begin(), projectiles.end(), projectile), projectiles.end());
    }
    /**
     * @brief Получить спецобъекты поля
     */
    std::vector<std::shared_ptr<GameObject>>& get_projectiles() {
        return projectiles;
    }

    template <typename T>
    std::vector<std::shared_ptr<T>>get_all() {
        std::vector<std::shared_ptr<T>> result{};
        for (int i = 0; i < get_size().first;i++) {
            for (int j = 0; j < get_size().second;j++) {
                auto data = get_cell(Vector2{static_cast<float>(i),static_cast<float>(j)}).get_all<T>();
                result.insert(result.end(), data.begin(), data.end());
            }
        }
        return result;
    }
};

#endif
