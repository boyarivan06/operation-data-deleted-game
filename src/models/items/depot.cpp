#include "depot.h"
std::vector<std::shared_ptr<Item>> Depot::get_content() {
    return content;
}
void Depot::add_item(const std::shared_ptr<Item>& item) {
    content.emplace_back(item);
}

void Depot::add_items(const std::vector<std::shared_ptr<Item>>& items) {
    for (const auto& item : items) add_item(item);
}

void Depot::remove_item(std::shared_ptr<Item> item) {
    for (int i = 0; i < content.size(); i++) {
        if (content[i] == item) {
            content.erase(content.begin()+i);
            break;
        }
        if (i == content.size()-1) throw std::runtime_error("There is no such item in this depot!");
    }
}

Depot::Depot()= default;



