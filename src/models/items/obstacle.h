#ifndef OBSTACLE_H
#define OBSTACLE_H
#include "game_object.h"

/**
 * @brief Класс препятствия
 */
class Obstacle : public GameObject {
protected:
    bool breakable = false;
public:
    //virtual void destroy();
    Obstacle() = default;
    explicit Obstacle(Vector2 pos) : GameObject(pos){color=RED;} ///< Конструктор с заданной позицией
    [[nodiscard]] bool is_breakable() const; ///< Узнать, можно ли сломать препятствие

    void set_breakability(bool); ///< Поменять прочность

    Obstacle(const std::string& _name, Color _color, Vector2 _size, Vector2 _pos, Vector2 _disp) : GameObject(_name, _color, _size, _pos, _disp){}
};


#endif
