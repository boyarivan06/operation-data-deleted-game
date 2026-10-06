#ifndef FOORAGEER_H
#define FOORAGEER_H
#include <vector>

#include "beast.h"
#include "creature.h"
#include "depot.h"
#include "inventarior.h"
#include "../items/item.h"

/**
 * @brief Монстр, который может носить предметы. Данный монстр пассивный и не атакует игрока, но может, если захотеть.
 */
class Foorageer : public Beast, public Inventarior {
    std::vector<std::shared_ptr<Depot>> depots;
public:
    explicit Foorageer(int); ///< Конструктор с заданной силой
    void move(Field &) override; ///< Перемещение (собирает с пола предметы и несёт на ближайший известный склад)
    void add_depot(std::shared_ptr<Depot>&); ///< Добавить склад в список известных складов

};
#endif
