#include <exceptions.h>
#include <field.h>
#include <queue>
#include <raylib.h>
#include <vector>
#include "vector2_operations.h"
#include <unordered_map>
#include <unordered_set>

struct Node {
    Vector2 pos;        // Позиция на карте
    float g_cost;       // Стоимость пути ОТ СТАРТА до этой точки
    float h_cost;       // Оценочная стоимость ОТ этой точки ДО ЦЕЛИ
    Node* parent;       // Откуда пришли в эту точку

    [[nodiscard]] float f_cost() const { return g_cost + h_cost; } // Общий приоритет
};

struct Vector2Hash {
    std::size_t operator()(const Vector2& v) const {
        return std::hash<int>{}(v.x) ^ (std::hash<int>{}(v.y) << 1);
    }
};

struct Vector2Equal {
    bool operator()(const Vector2& a, const Vector2& b) const {
        return a.x == b.x && a.y == b.y;
    }
};

// Эвристика - евклидово расстояние (точнее для диагоналей)
float heuristic(const Vector2& a, const Vector2& b) {
    float dx = abs(a.x - b.x);
    float dy = abs(a.y - b.y);
    return sqrt(dx * dx + dy * dy); // Евклидово расстояние
}

// Все 8 направлений движения
std::vector<Vector2> get_all_directions() {
    return {
        // Ортогональные
                    {1, 0}, {-1, 0}, {0, 1}, {0, -1},
                    // Диагональные
                    {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
    };
}


// Восстановление пути (без изменений)
// Восстановление пути - возвращаем ПОЗИЦИИ, а не направления
std::vector<Vector2> build_path(Node* end_node) {
    std::vector<Vector2> path;
    std::vector<Vector2> temp_path;

    // Собираем все позиции пути
    for (Node* node = end_node; node != nullptr; node = node->parent) {
        temp_path.push_back(node->pos);
    }

    // Разворачиваем путь (от начала к концу)
    std::reverse(temp_path.begin(), temp_path.end());

    // Конвертируем позиции в направления движения
    for (size_t i = 1; i < temp_path.size(); i++) {
        Vector2 direction = temp_path[i] - temp_path[i-1];
        path.push_back(direction);
    }

    return path;
}

std::vector<Vector2> Field::a_star_route(Vector2 from, Vector2 to) {
    if (!include_point(from) || !include_point(to)) 
        return{}; //throw FieldException("Точки не принадлежат полю");
    if (from == to) return {};
    if (cells[to.x][to.y].is_obstacle()) 
        throw WayImpassableException();

    auto cmp = [](Node* a, Node* b) { return a->f_cost() > b->f_cost(); };
    std::priority_queue<Node*, std::vector<Node*>, decltype(cmp)> open_list(cmp);

    std::unordered_set<Vector2, Vector2Hash, Vector2Equal> closed;
    std::unordered_map<Vector2, Node, Vector2Hash, Vector2Equal> nodes;

    nodes[from] = {from, 0, heuristic(from, to), nullptr};
    open_list.push(&nodes[from]);

    auto [width, height] = get_size();

    while (!open_list.empty()) {
        Node* current = open_list.top();
        open_list.pop();

        Vector2 current_pos = {current->pos.x, current->pos.y};

        if (current_pos == to) {
            return build_path(current);
        }

        closed.insert(current_pos);

        for (auto& dir : get_all_directions()) {
            Vector2 neighbor_pos = current_pos + dir;

            if (neighbor_pos.x < 0 || neighbor_pos.y < 0 ||
                neighbor_pos.x >= width || neighbor_pos.y >= height) {
                continue;
            }

            Vector2 int_pos = {neighbor_pos.x, neighbor_pos.y};
            if (cells[int_pos.x][int_pos.y].is_obstacle() || closed.count(int_pos)) {
                continue;
            }

            float move_cost = (abs(dir.x) + abs(dir.y) == 2) ? 1.414f : 1.0f;
            float new_g_cost = current->g_cost + move_cost;


            if (!nodes.count(int_pos) || new_g_cost < nodes[int_pos].g_cost) {
                nodes[int_pos] = {int_pos, new_g_cost, heuristic(int_pos, to), current};
                open_list.push(&nodes[int_pos]);
            }
        }
    }
    
    throw WayImpassableException("Маршрут непроходим");
}