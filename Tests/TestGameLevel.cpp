//
// Created by burth on 07.09.2021.
//
#include "../Game2D/src/Model/GameLevel.h"

#include "gtest/gtest.h"
#include <memory>

struct TestGameLevel : public GameLevel {
    using GameLevel::blockTouchesWall;

    using GameLevel::checkWallCollision;
    using GameLevel::checkWallCollisionPrecise;

    using GameLevel::checkBlockCollision;

    using GameLevel::checkCollisionPrecise;
    using GameLevel::checkCollisions;

    using GameLevel::calculateStepRange;

    using GameLevel::checkThreeDiamonds;

    using GameLevel::spawnEnemy;

    using GameLevel::getInitialDirections;

    using GameLevel::getDirectionIndex;

    using GameLevel::trySettingBoxer;

    using GameLevel::boxerExists;
};

TEST(GameLevel, BlockTouchesWall) {
    TestGameLevel testGameLevel = TestGameLevel();

    Block testBlock0 = Block({0, 0}, false, nullptr, BlockState::SOLID);
    Block testBlock1 = Block({Constants::WIDTH_UNIT, Constants::HEIGHT_UNIT}, false, nullptr, BlockState::SOLID);
    Block testBlock2 =
            Block({Constants::WIDTH_UNIT * 12, Constants::HEIGHT_UNIT * 14}, false, nullptr, BlockState::SOLID);
    Block testBlock3 = Block({Constants::WIDTH_UNIT, Constants::HEIGHT_UNIT * 12}, false, nullptr, BlockState::SOLID);


    EXPECT_EQ(true, testGameLevel.blockTouchesWall(testBlock0));
    EXPECT_EQ(false, testGameLevel.blockTouchesWall(testBlock1));
    EXPECT_EQ(true, testGameLevel.blockTouchesWall(testBlock2));
    EXPECT_EQ(false, testGameLevel.blockTouchesWall(testBlock3));
}

TEST(GameLevel, CheckWallCollision) {
    TestGameLevel testGameLevel = TestGameLevel();

    GameObject testObject0 = GameObject({0, 0});
    GameObject testObject1 = GameObject({Constants::WIDTH_UNIT, Constants::HEIGHT_UNIT});
    GameObject testObject2 = GameObject({Constants::WIDTH_UNIT * 12, Constants::HEIGHT_UNIT * 14});
    GameObject testObject3 = GameObject({Constants::WIDTH_UNIT, Constants::HEIGHT_UNIT * 12});
    GameObject testObject4 = GameObject({Constants::WIDTH_UNIT * 11, 0});
    GameObject testObject5 = GameObject({Constants::WIDTH_UNIT * 11.5f, 0});

    EXPECT_EQ(false, testGameLevel.checkWallCollision(testObject0, Direction::RIGHT));
    EXPECT_EQ(false, testGameLevel.checkWallCollision(testObject0, Direction::UP));
    EXPECT_EQ(true, testGameLevel.checkWallCollision(testObject0, Direction::LEFT));
    EXPECT_EQ(true, testGameLevel.checkWallCollision(testObject0, Direction::DOWN));

    EXPECT_EQ(false, testGameLevel.checkWallCollision(testObject1, Direction::RIGHT));
    EXPECT_EQ(false, testGameLevel.checkWallCollision(testObject1, Direction::UP));
    EXPECT_EQ(false, testGameLevel.checkWallCollision(testObject1, Direction::LEFT));
    EXPECT_EQ(false, testGameLevel.checkWallCollision(testObject1, Direction::DOWN));

    EXPECT_EQ(false, testGameLevel.checkWallCollisionPrecise(testObject0, Direction::RIGHT));
    EXPECT_EQ(false, testGameLevel.checkWallCollisionPrecise(testObject0, Direction::UP));
    EXPECT_EQ(true, testGameLevel.checkWallCollisionPrecise(testObject0, Direction::LEFT));
    EXPECT_EQ(true, testGameLevel.checkWallCollisionPrecise(testObject0, Direction::DOWN));

    EXPECT_EQ(false, testGameLevel.checkWallCollisionPrecise(testObject1, Direction::RIGHT));
    EXPECT_EQ(false, testGameLevel.checkWallCollisionPrecise(testObject1, Direction::UP));
    EXPECT_EQ(false, testGameLevel.checkWallCollisionPrecise(testObject1, Direction::LEFT));
    EXPECT_EQ(false, testGameLevel.checkWallCollisionPrecise(testObject1, Direction::DOWN));

    // This shows the relevant difference between the two methods
    EXPECT_EQ(true, testGameLevel.checkWallCollision(testObject5, Direction::RIGHT));
    EXPECT_EQ(false, testGameLevel.checkWallCollisionPrecise(testObject5, Direction::RIGHT));
}

TEST(GameLevel, CheckBlockCollision) {
    TestGameLevel testGameLevel = TestGameLevel();

    // This object is in the bottom left corner
    GameObject testObject0 = GameObject({0, 0});
    // This object is one unit to the right of testObject0 and at the same height
    GameObject testObject1 = GameObject({Constants::WIDTH_UNIT, 0});
    // This object is two units to the right of testObject0 and at the same height
    GameObject testObject2 = GameObject({Constants::WIDTH_UNIT * 2, 0});

    EXPECT_EQ(true, testGameLevel.checkBlockCollision(testObject0, testObject1, Direction::RIGHT));
    EXPECT_EQ(false, testGameLevel.checkBlockCollision(testObject0, testObject2, Direction::RIGHT));

    EXPECT_EQ(true, testGameLevel.checkBlockCollision(testObject1, testObject0, Direction::LEFT));
    EXPECT_EQ(false, testGameLevel.checkBlockCollision(testObject2, testObject0, Direction::LEFT));


    // This object is in the bottom left corner
    GameObject testObject3 = GameObject({0, 0});
    // This object is one unit above testObject3
    GameObject testObject4 = GameObject({0, Constants::HEIGHT_UNIT});
    // This object is two units above testObject3
    GameObject testObject5 = GameObject({0, Constants::HEIGHT_UNIT * 2});

    EXPECT_EQ(true, testGameLevel.checkBlockCollision(testObject3, testObject4, Direction::UP));
    EXPECT_EQ(false, testGameLevel.checkBlockCollision(testObject3, testObject5, Direction::UP));

    EXPECT_EQ(true, testGameLevel.checkBlockCollision(testObject4, testObject3, Direction::DOWN));
    EXPECT_EQ(false, testGameLevel.checkBlockCollision(testObject5, testObject3, Direction::DOWN));
}

TEST(GameLevel, CheckCollision) {
    TestGameLevel testGameLevel = TestGameLevel();

    GameObject testObject0 = GameObject({Constants::WIDTH_UNIT * 2, Constants::HEIGHT_UNIT * 2});
    GameObject testObject1 = GameObject({0, 0});

    Block testBlock0 =
            Block({Constants::WIDTH_UNIT * 2, Constants::HEIGHT_UNIT * 3}, false, nullptr, BlockState::SOLID);
    Block testBlock1 =
            Block({Constants::WIDTH_UNIT * 3, Constants::HEIGHT_UNIT * 2}, false, nullptr, BlockState::SOLID);
    Block testBlock2 = Block({0, Constants::HEIGHT_UNIT * 2}, false, nullptr, BlockState::SOLID);
    Block testBlock3 = Block({Constants::WIDTH_UNIT * 2, 0}, false, nullptr, BlockState::SOLID);
    Block testBlock4 =
            Block({Constants::WIDTH_UNIT * 1.5f, Constants::HEIGHT_UNIT * 2}, false, nullptr, BlockState::SOLID);
    Block testBlock5 =
            Block({Constants::WIDTH_UNIT * 0.5f, Constants::HEIGHT_UNIT * 2}, false, nullptr, BlockState::SOLID);

    testGameLevel.Blocks.push_back(testBlock0);
    testGameLevel.Blocks.push_back(testBlock1);
    testGameLevel.Blocks.push_back(testBlock2);
    testGameLevel.Blocks.push_back(testBlock3);

    EXPECT_EQ(true, testGameLevel.checkCollisions(testObject0, Direction::RIGHT));
    EXPECT_EQ(true, testGameLevel.checkCollisions(testObject0, Direction::UP));
    EXPECT_EQ(false, testGameLevel.checkCollisions(testObject0, Direction::DOWN));
    EXPECT_EQ(false, testGameLevel.checkCollisions(testObject0, Direction::LEFT));
    EXPECT_EQ(true, testGameLevel.checkCollisions(testObject1, Direction::LEFT));

    EXPECT_EQ(false, testGameLevel.checkCollisionPrecise(testObject0, testBlock0));
    EXPECT_EQ(false, testGameLevel.checkCollisionPrecise(testObject0, testBlock5));
    EXPECT_EQ(true, testGameLevel.checkCollisionPrecise(testObject0, testBlock4));
}

TEST(GameLevel, StepRange) {
    TestGameLevel testGameLevel = TestGameLevel();

    Block testBlock0 =
            Block({Constants::WIDTH_UNIT * 2, Constants::HEIGHT_UNIT * 2}, false, nullptr, BlockState::SOLID);


    Block testBlock1 =
            Block({Constants::WIDTH_UNIT * 3, Constants::HEIGHT_UNIT * 2}, false, nullptr, BlockState::SOLID);
    Block testBlock2 =
            Block({Constants::WIDTH_UNIT * 2, Constants::HEIGHT_UNIT * 10}, false, nullptr, BlockState::SOLID);
    Block testBlock3 = Block({Constants::WIDTH_UNIT * 2, 0}, false, nullptr, BlockState::SOLID);

    testGameLevel.Blocks.push_back(testBlock0);
    testGameLevel.Blocks.push_back(testBlock1);
    testGameLevel.Blocks.push_back(testBlock2);
    testGameLevel.Blocks.push_back(testBlock3);

    EXPECT_EQ(2, testGameLevel.calculateStepRange(testBlock0, Direction::LEFT));
    EXPECT_EQ(0, testGameLevel.calculateStepRange(testBlock0, Direction::RIGHT));
    EXPECT_EQ(7, testGameLevel.calculateStepRange(testBlock0, Direction::UP));
    EXPECT_EQ(1, testGameLevel.calculateStepRange(testBlock0, Direction::DOWN));
}

TEST(GameLevel, CheckThreeDiamonds) {
    TestGameLevel testGameLevel = TestGameLevel();

    Block testBlock0 = Block({0, Constants::HEIGHT_UNIT * 2}, true, nullptr, BlockState::SOLID);
    Block testBlock1 = Block({Constants::WIDTH_UNIT * 1, Constants::HEIGHT_UNIT * 2}, true, nullptr, BlockState::SOLID);
    Block testBlock2 = Block({Constants::WIDTH_UNIT * 2, Constants::HEIGHT_UNIT * 2}, true, nullptr, BlockState::SOLID);

    testGameLevel.Blocks.push_back(testBlock0);
    testGameLevel.Blocks.push_back(testBlock1);
    testGameLevel.Blocks.push_back(testBlock2);

    testGameLevel.checkThreeDiamonds();
    EXPECT_EQ(5000, testGameLevel.score.score);
    testGameLevel.checkThreeDiamonds();
    EXPECT_EQ(5000, testGameLevel.score.score);

    // Reset States
    testGameLevel.diamondBlocksAligned = false;
    testGameLevel.score.score = 0;

    testGameLevel.Blocks.at(0).position = {Constants::WIDTH_UNIT * 3, Constants::HEIGHT_UNIT * 2};
    testGameLevel.checkThreeDiamonds();
    EXPECT_EQ(10000, testGameLevel.score.score);

    // Reset States
    testGameLevel.diamondBlocksAligned = false;
    testGameLevel.score.score = 0;

    testGameLevel.Blocks.at(0).position = {Constants::WIDTH_UNIT * 4, Constants::HEIGHT_UNIT * 2};
    testGameLevel.checkThreeDiamonds();
    EXPECT_EQ(0, testGameLevel.score.score);
}

TEST(GameLevel, SpawnEnemy) {
    TestGameLevel testGameLevel = TestGameLevel();

    std::array<float, 2> pos = {Constants::WIDTH_UNIT * 0, Constants::HEIGHT_UNIT * 2};
    std::array<float, 2> velocity = {Constants::WIDTH_UNIT * 3, Constants::HEIGHT_UNIT * 3};
    Direction direction = Direction::DOWN;
    std::shared_ptr<Enemy> testEnemy0 =
            std::make_shared<Enemy>(pos, velocity, direction, EnemyState::NONE, EnemyType::WANDERING, false);
    testGameLevel.frozenEnemies.push_back(testEnemy0);
    Block testBlock0 = Block({0, Constants::HEIGHT_UNIT * 2}, false, testEnemy0, BlockState::SOLID);
    testGameLevel.Blocks.push_back(testBlock0);

    pos = {Constants::WIDTH_UNIT * 7, Constants::HEIGHT_UNIT * 4};
    velocity = {Constants::WIDTH_UNIT * 3, Constants::HEIGHT_UNIT * 3};
    direction = Direction::DOWN;
    std::shared_ptr<Enemy> testEnemy1 =
            std::make_shared<Enemy>(pos, velocity, direction, EnemyState::NONE, EnemyType::WANDERING, false);
    testGameLevel.frozenEnemies.push_back(testEnemy1);
    Block testBlock1 =
            Block({Constants::WIDTH_UNIT * 7, Constants::HEIGHT_UNIT * 4}, false, testEnemy1, BlockState::SOLID);
    testGameLevel.Blocks.push_back(testBlock1);


    testGameLevel.spawnEnemy();
    EXPECT_EQ(BlockState::BROKEN, testGameLevel.Blocks.back().state);
    EXPECT_EQ(EnemyState::SPAWNING, testEnemy1->state);
    EXPECT_EQ(testEnemy1, testGameLevel.Enemies.back());
    EXPECT_EQ(testEnemy0, testGameLevel.frozenEnemies.back());
    EXPECT_EQ(BlockState::FLASHING, testGameLevel.Blocks.front().state);
}

TEST(GameLevel, InitialDirections) {
    std::vector<Direction> expectedDirections;
    TestGameLevel testGameLevel = TestGameLevel();

    // upper right corner
    testGameLevel.Pengo = Player({Constants::WIDTH_UNIT * 12, Constants::HEIGHT_UNIT * 14},
                                 {Constants::WIDTH_UNIT * 3, Constants::HEIGHT_UNIT * 3});

    // bottom left corner - wandering enemy
    std::array<float, 2> pos = {Constants::WIDTH_UNIT * 0, Constants::HEIGHT_UNIT * 0};
    std::array<float, 2> velocity = {Constants::WIDTH_UNIT * 3, Constants::HEIGHT_UNIT * 3};
    Direction direction = Direction::DOWN;
    std::shared_ptr<Enemy> testEnemy0 =
            std::make_shared<Enemy>(pos, velocity, direction, EnemyState::NONE, EnemyType::WANDERING, false);
    testGameLevel.Enemies.push_back(testEnemy0);

    Block testBlock0 =
            Block({Constants::WIDTH_UNIT * 1, Constants::HEIGHT_UNIT * 0}, false, nullptr, BlockState::SOLID);
    testGameLevel.Blocks.push_back(testBlock0);

    expectedDirections = {Direction::UP};
    EXPECT_EQ(expectedDirections, testGameLevel.getInitialDirections(*testEnemy0));
    expectedDirections.clear();

    // middle - wandering enemy
    pos = {Constants::WIDTH_UNIT * 6, Constants::HEIGHT_UNIT * 7};
    velocity = {Constants::WIDTH_UNIT * 3, Constants::HEIGHT_UNIT * 3};
    direction = Direction::DOWN;
    std::shared_ptr<Enemy> testEnemy1 =
            std::make_shared<Enemy>(pos, velocity, direction, EnemyState::NONE, EnemyType::WANDERING, false);
    testGameLevel.Enemies.push_back(testEnemy1);

    expectedDirections = {Direction::DOWN, Direction::RIGHT, Direction::LEFT, Direction::UP};
    EXPECT_EQ(expectedDirections, testGameLevel.getInitialDirections(*testEnemy1));
    expectedDirections.clear();

    // lower right area - chasing enemy
    pos = {Constants::WIDTH_UNIT * 11, Constants::HEIGHT_UNIT * 1};
    velocity = {Constants::WIDTH_UNIT * 3, Constants::HEIGHT_UNIT * 3};
    direction = Direction::DOWN;
    std::shared_ptr<Enemy> testEnemy2 =
            std::make_shared<Enemy>(pos, velocity, direction, EnemyState::NONE, EnemyType::CHASING, false);
    testGameLevel.Enemies.push_back(testEnemy2);

    // below chasing enemy
    Block testBlock1 =
            Block({Constants::WIDTH_UNIT * 11, Constants::HEIGHT_UNIT * 0}, false, nullptr, BlockState::SOLID);
    testGameLevel.Blocks.push_back(testBlock1);
    // right to the chasing enemy
    Block testBlock2 =
            Block({Constants::WIDTH_UNIT * 10, Constants::HEIGHT_UNIT * 1}, false, nullptr, BlockState::SOLID);
    testGameLevel.Blocks.push_back(testBlock2);

    expectedDirections = {Direction::UP, Direction::RIGHT, Direction::LEFT, Direction::DOWN};
    EXPECT_EQ(expectedDirections, testGameLevel.getInitialDirections(*testEnemy2));
    expectedDirections.clear();
}

TEST(GameLevel, DirectionIndex) {
    std::vector<int> probabilities = {90, 94, 98, 100};
    TestGameLevel::getDirectionIndex(probabilities);

    EXPECT_GE(3, TestGameLevel::getDirectionIndex(probabilities));
}

TEST(GameLevel, Boxer) {
    TestGameLevel testGameLevel = TestGameLevel();

    std::array<float, 2> pos = {Constants::WIDTH_UNIT * 0, Constants::HEIGHT_UNIT * 0};
    std::array<float, 2> velocity = {Constants::WIDTH_UNIT * 3, Constants::HEIGHT_UNIT * 3};
    Direction direction = Direction::DOWN;
    std::shared_ptr<Enemy> testEnemy0 =
            std::make_shared<Enemy>(pos, velocity, direction, EnemyState::NONE, EnemyType::WANDERING, false);
    testGameLevel.Enemies.push_back(testEnemy0);

    testGameLevel.trySettingBoxer();
    EXPECT_EQ(true, testGameLevel.boxerExists());
}
