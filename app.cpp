#include "src/controller/controller.h"


int main() {
    std::locale::global(std::locale("en_US.UTF-8"));
    Controller game;
    game.run();
}


