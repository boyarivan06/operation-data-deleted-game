#include "foorageer.h"
#include "depot.h"
#include "vector2_operations.h"

Foorageer::Foorageer(int n) : Inventarior(n){
}

void Foorageer::move(Field & field) {
    if (!is_moving()) {
        /// выгружаемся на склад
        if (field.get_cell(cell_position).get_first<Depot>() != nullptr && !inventory.empty()) {
            auto depot = field.get_cell(cell_position).get_first<Depot>();
            depot->add_items(inventory);
            strength = max_strength;
            inventory.clear();
        }

        auto& cell = field.get_cell(cell_position);
        /// пришли на клетку с предметом, можем взять
        if (cell.get_first<Item>() != nullptr) {
            auto item = cell.get_first<Item>();
            this->take_item(item);
            cell.remove_object(item);
        }

        std::shared_ptr<Item> nearest_item = field.find_nearest<Item>(cell_position);

        // Если нет предметов и инвентарь пуст - некуда идти
        if (!nearest_item && inventory.empty()) {
            return;
        }

        /// переполнились и есть склад
        if (strength == 0 ||
            (nearest_item && nearest_item->get_weight() > strength) ||
            (!nearest_item && !depots.empty())) {

            if (!depots.empty()) {
                const auto nearest_depot_iter = std::ranges::min_element(
                    depots,
                    [this](const auto& a, const auto& b) {
                        return module(a->get_position() - cell_position) <
                               module(b->get_position() - cell_position);
                    });
                Creature::move((*nearest_depot_iter)->get_position(), field);
            }
            }
        /// есть куда идти (предмет существует и не на текущей клетке)
        else if (nearest_item && nearest_item->get_position() != cell_position) {
            Creature::move(nearest_item->get_position(), field);
        }
        // Если предмет уже на текущей клетке, он будет взят в начале следующего move
    }
}

void Foorageer::add_depot(std::shared_ptr<Depot> & depot) {
    depots.emplace_back(depot);
}
