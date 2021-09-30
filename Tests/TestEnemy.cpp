//
// Created by burth on 12.09.2021.
//
#include "../Game2D/src/Model/Enemy.h"

#include "gtest/gtest.h"
#include <memory>

struct TestEnemy : public Enemy {
    using Enemy::getProbabilityArray;
};

TEST(Enemy, ProbabilityArray) {
    std::array<float, 2> pos = {Constants::WIDTH_UNIT * 0, Constants::HEIGHT_UNIT * 0};
    std::array<float, 2> velocity = {Constants::WIDTH_UNIT * 3, Constants::HEIGHT_UNIT * 3};
    Direction direction = Direction::DOWN;
    Enemy testEnemy0 = Enemy(pos, velocity, direction, EnemyState::NONE, EnemyType::WANDERING, false);

    std::vector<Direction> testDirections0 = {Direction::UP, Direction::RIGHT, Direction::LEFT, Direction::DOWN};
    std::vector<int> expectedProbabilities0 = {90, 94, 98, 100};
    EXPECT_EQ(expectedProbabilities0, testEnemy0.getProbabilityArray(testDirections0));
}