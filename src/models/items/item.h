#ifndef ITEM_H
#define ITEM_H
#include "../base/game_object.h"


/**
 * @brief Класс предмета
 */
class Item : public GameObject {
    int weight = 0;
public:
    explicit Item(int); ///< Конструктор с заданным весом
    void set_weight(int); ///< Изменить вес
    [[nodiscard]] int get_weight() const; ///< Узнать вес
    ~Item() override = default; ///< Деструктор по умолчанию
};

#endif