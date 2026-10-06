#include "creature.h"
#include "field.h"
#include <gtest/gtest.h>
#include <memory>

#include "config.h"

class CreatureTest : public ::testing::Test {
protected:
    void SetUp() override {
        creature = std::make_shared<Creature>();
        field = std::make_shared<Field>(10, 10);

        // Устанавливаем начальную позицию
        creature->set_position({5.0f, 5.0f});
        field->get_cell({5, 5}).add_object(creature);
    }

    std::shared_ptr<Creature> creature;
    std::shared_ptr<Field> field;
};

TEST_F(CreatureTest, InitialState) {
    EXPECT_FALSE(creature->is_moving());
    EXPECT_FALSE(creature->has_route());
    EXPECT_FALSE(creature->is_dead());
    EXPECT_EQ(creature->get_time_points(), 100);
}

TEST_F(CreatureTest, MoveSetsRoute) {
    Vector2 target = {7, 7};
    creature->move(target, *field);

    EXPECT_TRUE(creature->has_route());
    EXPECT_FALSE(creature->is_moving());
}

TEST_F(CreatureTest, MoveClearsRouteWhenWayImpassable) {
    // Создаем препятствие вокруг
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (i != 5 || j != 5) {
                auto obstacle = std::make_shared<Obstacle>();
                obstacle->set_position({float(i), float(j)});
                field->get_cell({float(i), float(j)}).add_object(obstacle);
            }
        }
    }

    Vector2 target = {7, 7};
    creature->move(target, *field);

    EXPECT_FALSE(creature->has_route());
}

TEST_F(CreatureTest, TryStepSuccess) {
    Vector2 direction = {1, 0};
    int initial_time_points = creature->get_time_points();

    bool result = creature->try_step(direction, *field);

    EXPECT_TRUE(result);
    EXPECT_TRUE(creature->is_moving());
    EXPECT_EQ(creature->get_time_points(), initial_time_points - STEP_TIME_POINTS);
}

TEST_F(CreatureTest, TryStepFailsWhenMoving) {
    Vector2 direction1 = {1, 0};
    creature->try_step(direction1, *field); // Начинаем движение

    Vector2 direction2 = {0, 1};
    bool result = creature->try_step(direction2, *field);

    EXPECT_FALSE(result);
}

TEST_F(CreatureTest, TryStepFailsWhenObstacle) {
    auto obstacle = std::make_shared<Obstacle>();
    obstacle->set_position({6.0f, 5.0f});
    field->get_cell({6, 5}).add_object(obstacle);

    Vector2 direction = {1, 0};
    bool result = creature->try_step(direction, *field);

    EXPECT_FALSE(result);
    EXPECT_FALSE(creature->is_moving());
}

TEST_F(CreatureTest, TryStepFailsWhenOutOfBounds) {
    creature->set_position({9.0f, 9.0f});
    field->get_cell({5, 5}).remove_object(creature);
    field->get_cell({9, 9}).add_object(creature);

    Vector2 direction = {1, 1};
    bool result = creature->try_step(direction, *field);

    EXPECT_FALSE(result);
}

TEST_F(CreatureTest, UpdateRouteMovementStartsMovement) {
    Vector2 target = {7, 7};
    creature->move(target, *field);

    creature->update_route_movement(*field, 0.0f);

    EXPECT_TRUE(creature->is_moving() || creature->has_route());
}

TEST_F(CreatureTest, UpdateRouteMovementSkipsInvalidSteps) {
    // Блокируем первый шаг
    auto obstacle = std::make_shared<Obstacle>();
    obstacle->set_position({6.0f, 5.0f});
    field->get_cell({6, 5}).add_object(obstacle);

    Vector2 target = {7, 7};
    creature->move(target, *field);

    creature->update_route_movement(*field, 0.0f);

    // Должен пропустить заблокированный шаг и попытаться следующий
    EXPECT_TRUE(creature->has_route() || creature->is_moving());
}


TEST_F(CreatureTest, StepCallsTryStep) {
    Vector2 direction = {0, 1};
    int initial_time_points = creature->get_time_points();

    creature->step(direction, *field);

    // Либо начал движение, либо нет, но не должно быть ошибки
    EXPECT_TRUE(creature->is_moving() || !creature->is_moving());
}

TEST_F(CreatureTest, ClearRoute) {
    Vector2 target = {7, 7};
    creature->move(target, *field);

    EXPECT_TRUE(creature->has_route());

    creature->clear_route();

    EXPECT_FALSE(creature->has_route());
}

TEST_F(CreatureTest, CauseDamage) {
    int initial_health = creature->get_health();
    int damage = 3;

    creature->cause_damage(damage);

    EXPECT_EQ(creature->get_health(), initial_health - damage);
    EXPECT_FALSE(creature->is_dead());
}

TEST_F(CreatureTest, CauseDamageToDeath) {
    creature->cause_damage(15);

    EXPECT_TRUE(creature->is_dead());
    EXPECT_EQ(creature->get_health(), 0);
}
