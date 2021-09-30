#pragma once

#include "../util.h"
#include "Enemy.h"
#include "GameObject.h"

#include <string>
#include <vector>

enum class BlockState { SOLID, BREAKING, BROKEN, SPAWNING, FLASHING };

class Block : public GameObject {
  public:
    inline static const std::array<float, 2> VELOCITY = {Constants::WIDTH_UNIT * 12, Constants::HEIGHT_UNIT * 12};

    BlockState state = BlockState::SOLID;
    std::array<float, 2> positionToMoveTo{};
    bool isUnbreakable{};
    std::shared_ptr<Enemy> containedEnemy;
    Direction direction;
    int killedWithOneMove = 0;

    static constexpr float BREAKING_DURATION = 0.5f;
    static constexpr float FLASHING_DURATION = 3.0f;
    float breakingFor = 0.0f;
    float flashingFor = 0.0f;

    Block() = default;
    Block(std::array<float, 2> pos, bool isUnbreakable, std::shared_ptr<Enemy> containedEnemy, BlockState state);

    void setState(BlockState blockState);
    void setPositionToMoveTo(Direction direction);
    void push(Direction direction, int stepRange);
    void move(float deltaTime);
};