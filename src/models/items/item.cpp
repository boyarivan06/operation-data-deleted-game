#include "item.h"
Item::Item(int w) {
    weight = w;
}
int Item::get_weight() const {
    return weight;
}
void Item::set_weight(int w) {
    if (w < 0) throw std::runtime_error("Weight cannot be < 0");
    weight = w;
}


