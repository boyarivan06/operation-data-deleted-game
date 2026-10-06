#include <medicine.h>
#include <weapon.h>
#include <gtest/gtest.h>
#include "depot.h"

TEST(Depot, CreateGet) {
    auto depot = Depot();
    auto frst_item = std::make_shared<Item> (Item(3));
    auto sec_item = std::make_shared<Item>(Item(34));
    depot.add_item(std::make_shared<Item>(Weapon()));
    depot.add_item(std::make_shared<Item>(Medicine(5, 6)));
    depot.add_items({frst_item, sec_item});
    EXPECT_EQ(depot.get_content()[2], frst_item);
    depot.remove_item(depot.get_content()[2]);
    EXPECT_EQ(depot.get_content()[2], sec_item);
}