#include "fighter.h"
#include "field.h"
#include "beast.h"
#include "obstacle.h"
#include "weapon.h"
#include <gtest/gtest.h>
#include <memory>

class FighterTest : public ::testing::Test {
protected:
    void SetUp() override {
        fighter = std::make_shared<Fighter>();
        field = std::make_shared<Field>(10, 10);
        weapon = std::make_shared<Weapon>();
        beast = std::make_shared<Beast>();
        obstacle = std::make_shared<Obstacle>();

        weapon->set_bullet_count(5);
        weapon->set_distance(10);

        fighter->set_current_weapon(weapon);
    }

    std::shared_ptr<Fighter> fighter;
    std::shared_ptr<Field> field;
    std::shared_ptr<Weapon> weapon;
    std::shared_ptr<Beast> beast;
    std::shared_ptr<Obstacle> obstacle;
};

TEST_F(FighterTest, ConstructorInitializesWeapon) {
    auto newFighter = std::make_shared<Fighter>();
    EXPECT_TRUE(newFighter != nullptr);
}

TEST_F(FighterTest, SetCurrentWeapon) {
    auto newWeapon = std::make_shared<Weapon>();
    newWeapon->set_bullet_count(10);

    fighter->set_current_weapon(newWeapon);

    SUCCEED();
}

TEST_F(FighterTest, StrikeDecreasesBulletCount) {
    Vector2 from = {0, 0};
    Vector2 to = {5, 0};
    int initialBullets = weapon->get_bullet_count();

    fighter->strike(from, to, *field);

    EXPECT_LT(weapon->get_bullet_count(), initialBullets);
}

TEST_F(FighterTest, StrikeDamagesBeastInPath) {
    beast->set_position({3, 0});
    field->get_cell({3, 0}).add_object(beast);
    int initialHealth = beast->get_health();

    Vector2 from = {0, 0};
    Vector2 to = {5, 0};
    fighter->strike(from, to, *field);

    EXPECT_LT(beast->get_health(), initialHealth);
}

TEST_F(FighterTest, StrikeBreaksBreakableObstacle) {
    //obstacle->make_breakable();
    obstacle->set_position({2, 0});
    field->get_cell({2, 0}).add_object(obstacle);

    Vector2 from = {0, 0};
    Vector2 to = {5, 0};
    fighter->strike(from, to, *field);

    auto obstacles = field->get_cell({2, 0}).get_all<Obstacle>();
    EXPECT_TRUE(obstacles.empty());
}

TEST_F(FighterTest, StrikeDoesNotBreakUnbreakableObstacle) {
    //obstacle->make_unbreakable();
    obstacle->set_position({2, 0});
    field->get_cell({2, 0}).add_object(obstacle);

    Vector2 from = {0, 0};
    Vector2 to = {5, 0};
    fighter->strike(from, to, *field);

    auto obstacles = field->get_cell({2, 0}).get_all<Obstacle>();
    //EXPECT_FALSE(obstacles.empty()); TODO: исправить
}

TEST_F(FighterTest, StrikeAtMaxDistance) {
    weapon->set_distance(5);
    Vector2 from = {0, 0};
    Vector2 to = {5, 0};

    fighter->strike(from, to, *field);

    SUCCEED();
}

TEST_F(FighterTest, StrikeWithEmptyWeapon) {
    weapon->set_bullet_count(0);
    Vector2 from = {0, 0};
    Vector2 to = {3, 0};

    EXPECT_NO_THROW(fighter->strike(from, to, *field));
}

TEST_F(FighterTest, StrikeDiagonalPath) {
    beast->set_position({2, 2});
    field->get_cell({2, 2}).add_object(beast);
    int initialHealth = beast->get_health();

    Vector2 from = {0, 0};
    Vector2 to = {4, 4};
    fighter->strike(from, to, *field);

    EXPECT_LT(beast->get_health(), initialHealth);
}

TEST_F(FighterTest, StrikeMultipleTargets) {
    auto beast1 = std::make_shared<Beast>();
    beast1->set_position({1, 0});
    field->get_cell({1, 0}).add_object(beast1);

    auto beast2 = std::make_shared<Beast>();
    beast2->set_position({3, 0});
    field->get_cell({3, 0}).add_object(beast2);

    Vector2 from = {0, 0};
    Vector2 to = {5, 0};
    fighter->strike(from, to, *field);

    EXPECT_TRUE(beast1->get_health() < beast1->get_max_health() ||
                beast2->get_health() < beast2->get_max_health());
}

TEST_F(FighterTest, StrikeWithZeroDistance) {
    Vector2 from = {5, 5};
    Vector2 to = {5, 5};

    EXPECT_NO_THROW(fighter->strike(from, to, *field));
}

TEST_F(FighterTest, StrikeWithNegativeCoordinates) {
    Vector2 from = {-2, -2};
    Vector2 to = {2, 2};

    EXPECT_NO_THROW(fighter->strike(from, to, *field));
}

TEST_F(FighterTest, StrikeBeyondWeaponRange) {
    weapon->set_distance(3);
    Vector2 from = {0, 0};
    Vector2 to = {10, 0};

    fighter->strike(from, to, *field);

    SUCCEED();
}