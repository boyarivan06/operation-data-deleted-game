#include "inventarior.h"
class Item;

Inventarior::Inventarior(int max_str) {
    max_strength = max_str;
    strength = max_strength;
}

void Inventarior::take_item(const std::shared_ptr<Item>& item) {
    strength -= item->get_weight();
    inventory.push_back(item);
}
// void Inventarior::lay_down(const std::shared_ptr<Item>& item) {
//     for (int i = 0; i < inventory.size(); i++) {
//         if (inventory[i] == item) {
//             inventory.erase(inventory.begin()+i);
//             break;
//         }
//         if (i == inventory.size()-1) throw std::runtime_error("This item isn't from this inventory!");
//     }
// }
