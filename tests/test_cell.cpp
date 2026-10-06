#include <gtest/gtest.h>
#include "cell.h"
#include "medicine.h"
#include "weapon.h"

TEST(Cell, CreateGet) {
    Cell cell;
    auto weapon = std::make_shared<Weapon>(Weapon());
    cell.add_object(weapon);
    cell.add_object(std::make_shared<Obstacle>(Obstacle()));
    EXPECT_EQ(cell.get_objects()[0], weapon);
    cell.remove_object(weapon);
    auto med = std::make_shared<Medicine>(Medicine(5, 6));
    cell.add_object(med);
    EXPECT_EQ(cell.get_first<Medicine>(), med);
    EXPECT_EQ(cell.get_first<Weapon>(), nullptr);
    EXPECT_EQ(cell.get_all<Medicine>().size(), 1);
    EXPECT_EQ(cell.is_obstacle(), true);
}