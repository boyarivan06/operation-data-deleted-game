#include "rational_beast.h"
#include "field.h"
#include "detective.h"
#include "depot.h"
#include <gtest/gtest.h>
#include <memory>

class RationalBeastTest : public ::testing::Test {
protected:
    void SetUp() override {
        rationalBeast = std::make_shared<RationalBeast>();
        field = std::make_shared<Field>(10, 10);
        weapon = std::make_shared<Weapon>();
        detective = std::make_shared<Detective>();
        depot = std::make_shared<Depot>();

        weapon->set_bullet_count(5);
        weapon->set_distance(3);
    }

    std::shared_ptr<RationalBeast> rationalBeast;
    std::shared_ptr<Field> field;
    std::shared_ptr<Weapon> weapon;
    std::shared_ptr<Detective> detective;
    std::shared_ptr<Depot> depot;
};

TEST_F(RationalBeastTest, MoveWithoutWeaponNoTargets) {
    rationalBeast->set_position({0, 0});
    rationalBeast->move(*field);

    EXPECT_FALSE(rationalBeast->is_moving());
    EXPECT_FALSE(rationalBeast->has_route());
}

TEST_F(RationalBeastTest, DropsEmptyWeapon) {
    weapon->set_bullet_count(0);
    rationalBeast->set_current_weapon(weapon);
    rationalBeast->set_position({5, 5});

    rationalBeast->move(*field);

    //EXPECT_EQ(rationalBeast->get_current_weapon(), nullptr);
}

/*TEST_F(RationalBeastTest, SeeksDetectiveWithLoadedWeapon) {
    rationalBeast->set_current_weapon(weapon);
    rationalBeast->set_position({0.0f, 0.0f});
    detective->set_position({2.0f, 2.0f});
    field->get_cell({2, 2}).add_object(detective);

    rationalBeast->move(*field);

    EXPECT_TRUE(rationalBeast->is_moving());
}

TEST_F(RationalBeastTest, PrefersCloserWeaponSource) {
    auto closeWeapon = std::make_shared<Weapon>();
    closeWeapon->set_bullet_count(5);
    closeWeapon->set_position({1.0f, 1.0f});
    field->get_cell({1, 1}).add_object(closeWeapon);

    auto farDepot = std::make_shared<Depot>();
    farDepot->set_position({8.0f, 8.0f});
    field->get_cell({8, 8}).add_object(farDepot);

    rationalBeast->set_position({0.0f, 0.0f});
    rationalBeast->move(*field);

    EXPECT_TRUE(rationalBeast->is_moving());
}*/

TEST_F(RationalBeastTest, PicksUpWeaponFromCell) {
    rationalBeast->set_position({0, 0});
    field->get_cell({0, 0}).add_object(weapon);

    rationalBeast->move(*field);

    EXPECT_NE(rationalBeast->get_current_weapon(), nullptr);
}

TEST_F(RationalBeastTest, PicksUpWeaponFromDepot) {
    rationalBeast->set_position({0, 0});
    depot->add_item(weapon);
    field->get_cell({0, 0}).add_object(depot);

    rationalBeast->move(*field);

    EXPECT_NE(rationalBeast->get_current_weapon(), nullptr);
    EXPECT_TRUE(depot->get_content().empty());
}



TEST_F(RationalBeastTest, NoMovementWhenNoWeaponsAvailable) {
    rationalBeast->set_position({5, 5});

    rationalBeast->move(*field);

    EXPECT_FALSE(rationalBeast->is_moving());
    EXPECT_FALSE(rationalBeast->has_route());
}

TEST_F(RationalBeastTest, AttacksWhenInRange) {
    rationalBeast->set_current_weapon(weapon);
    rationalBeast->set_position({0, 0});
    detective->set_position({1, 1});
    field->get_cell({1, 1}).add_object(detective);

    int initialBullets = weapon->get_bullet_count();
    rationalBeast->move(*field);

    EXPECT_LT(weapon->get_bullet_count(), initialBullets);
}

TEST_F(RationalBeastTest, ChoosesMostLoadedWeapon) {
    auto weakWeapon = std::make_shared<Weapon>();
    weakWeapon->set_bullet_count(2);
    auto strongWeapon = std::make_shared<Weapon>();
    strongWeapon->set_bullet_count(10);

    field->get_cell({0, 0}).add_object(weakWeapon);
    field->get_cell({0, 0}).add_object(strongWeapon);
    rationalBeast->set_position({0, 0});

    rationalBeast->move(*field);

    EXPECT_EQ(rationalBeast->get_current_weapon()->get_bullet_count(), 10);
}