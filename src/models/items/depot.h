#ifndef DEPOT_H
#define DEPOT_H
#include "item.h"
#include <vector>

/**
 * @brief Класс склада предметов
 */
class Depot : public GameObject {
    std::vector<std::shared_ptr<Item>> content;
public:
    std::vector<std::shared_ptr<Item>> get_content(); ///< Получить содержимое склада
    void add_item(const std::shared_ptr<Item>&); ///< Добавить предмет
    void add_items(const std::vector<std::shared_ptr<Item>>&); ///< Добавить предметы
    void remove_item(std::shared_ptr<Item> item); ///< Удалить предмет
    Depot(); ///< Конструктор по умолчанию
};

#endif
