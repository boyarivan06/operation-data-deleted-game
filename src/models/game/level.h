#ifndef LEVEL_H
#define LEVEL_H
#include "field.h"
#include <string>

#include "GameTimer.h"

/**
 * @brief Класс игрового уровня
 */
class Level {
    Field field; ///< Поле
    std::string name; ///< Название уровня
    std::string preamble; ///< Текст перед уровнем
public:
    CountdownTimer timer;
    Level(); ///< Конструктор по умолчанию
    Level(Field , std::string , std::string , CountdownTimer); ///< Конструктор с заданным полем, названием и текстом
    Field& get_field(); ///< Получить поле
    std::string get_name(); ///< Получить название
};

#endif
