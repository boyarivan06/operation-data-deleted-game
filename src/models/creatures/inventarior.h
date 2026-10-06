#ifndef INVENTARIOR_H
#define INVENTARIOR_H

#include "item.h"
#include <vector>

/**
 * @brief Интерфейс: тот, кто может носить предметы
 */
class Inventarior {
protected:
    std::vector<std::shared_ptr<Item>> inventory;
    int strength = 0; ///< максимальный вес носимых предметов
    int max_strength = 0;
public:
    virtual ~Inventarior() = default; ///< Деструктор по умолчанию
    explicit Inventarior(int); ///< Конструктор с заданной силой (сколько может унести)
    virtual void take_item(const std::shared_ptr<Item> &item); ///< Взять предмет
    //virtual void lay_down(const std::shared_ptr<Item> &item);
};


#endif
