#include "field.h"

#include "../ext/vector2_operations.h"
#include <nlohmann/json.hpp>
#include <ostream>

#include "config.h"
#include "wild_beast.h"

Field::Field() {
    cells = CustomMatrix<Cell>(20, 10);
    auto _bots = std::vector<std::shared_ptr<Beast>>();
    bots = std::make_shared<std::vector<std::shared_ptr<Beast>>>(_bots);
}

Field::Field(int h, int w) {
    if (h <= 0 || w <= 0) throw std::invalid_argument("Invalid height or width");
    cells = CustomMatrix<Cell>(h, w);
    auto _bots = std::vector<std::shared_ptr<Beast>>();
    bots = std::make_shared<std::vector<std::shared_ptr<Beast>>>(_bots);
}

CustomVector<Cell>& Field::operator[](int i) {
    return cells[i];
}
bool Field::is_valid_cell(Vector2 pos) {
    /*pos.x = roundf(pos.x) + pos.x > roundf(pos.x) ? 1 : 0;
    pos.y = roundf(pos.y) + pos.y > roundf(pos.y) ? 1 : 0;*/
    return include_point(pos) && !cells[static_cast<int>(pos.x)][static_cast<int>(pos.y)].is_obstacle();
}

bool Field::include_point(Vector2 point) {

    return 0 <= point.x && point.x < get_size().first && 0 <= point.y && point.y < get_size().second;
}

std::pair<int, int> Field::get_size() {
    return cells.get_matrix_size();
}

void Field::add_object(const std::shared_ptr<GameObject> &shared) {
    if (include_point(shared->get_position()))
        cells[shared->get_position().x][shared->get_position().y].add_object(shared);
}

void Field::remove_object(const std::shared_ptr<GameObject>& obj) {
    get_cell(obj->get_position()).remove_object(obj);
    remove_projectile(obj);
}

Cell & Field::get_cell(Vector2 vector2) {
    return cells[static_cast<int>(vector2.x)][static_cast<int>(vector2.y)];
}

Vector2 Field::convert_global_to_cell(Vector2 vec) {
    Vector2 res;
    res.x = static_cast<float>(static_cast<int>(vec.x));
    res.y = static_cast<float>(static_cast<int>(vec.y));
    return res;
}

