#ifndef AMMO_H
#define AMMO_H
#include "item.h"

/**
 * @brief Класс набора патронов
 */
class Ammo : public Item {
    int bullet_count{};

public:
    explicit Ammo(int); ///< Конструктор с заданным количеством пуль
    void remove_bullets(int); ///< Удалить заданное количество пуль
    [[nodiscard]] int get_bullet_cnt() const; ///< Узнать количество пуль
};

#endif
