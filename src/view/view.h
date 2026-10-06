#ifndef VIEW_H
#define VIEW_H
#include <vector>
#include <string>

#include "field.h"
#include "game_object.h"

inline std::vector resources {
    std::string("/Users/ivanboarincev/CLionProjects/b24506_boiarintsev.is/3/resources/img/hero.png"),
    std::string("/Users/ivanboarincev/CLionProjects/b24506_boiarintsev.is/3/resources/img/foor.png"),
    std::string("/Users/ivanboarincev/CLionProjects/b24506_boiarintsev.is/3/resources/img/black_monster.png"),
    std::string("/Users/ivanboarincev/CLionProjects/b24506_boiarintsev.is/3/resources/img/gun_monster.png"),
    std::string("/Users/ivanboarincev/CLionProjects/b24506_boiarintsev.is/3/resources/img/item.png"),
    std::string("/Users/ivanboarincev/CLionProjects/b24506_boiarintsev.is/3/resources/img/gun.png"),
    std::string("/Users/ivanboarincev/CLionProjects/b24506_boiarintsev.is/3/resources/img/rock.png")
};

/**
 * @brief Класс управления представлением
 */
class View {
    std::vector<Texture2D> textures;
    int screen_height = 500;
    int screen_width = 500;
    std::string window_title{};
    Font font{};
public:
    View(); ///< Конструктор (загрузка текстур)
    ~View(); ///< Деструктор (выгрузка текстур)
    void draw_view(const std::vector<std::shared_ptr<GameObject>> &objects) const; ///< Нарисовать объекты

    //void draw_view(Field &field);


    void init_window(const std::string&); ///< Инициализировать окно

    void init_font(); ///< Загрузка шрифта

    static void set_window_title(const std::string&); ///< Поменять подпись окна

    void draw_object(GameObject &game_object) const; ///< Нарисовать объект
    void set_window_size(const std::pair<int, int> &); ///< Поменять размер окна
};



#endif
