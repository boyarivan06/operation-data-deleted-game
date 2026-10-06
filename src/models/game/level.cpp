#include "level.h"

#include <utility>
Level::Level() {
    field = Field();
    name = "Первый уровень";
    preamble = "Давайте начнём";
}

Level::Level(Field field, std::string name, std::string preamble, CountdownTimer timer) : field(std::move(field)), name(std::move(name)), preamble(std::move(preamble)), timer(timer) {
}

Field& Level::get_field() {
    return field;
}

std::string Level::get_name() {
    return name;
}
