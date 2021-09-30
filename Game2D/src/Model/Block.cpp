#include "Block.h"

#include <utility>

Block::Block(std::array<float, 2> pos, bool isUnbreakable, std::shared_ptr<Enemy> containedEnemy, BlockState state) :
    GameObject{pos}, state{state}, positionToMoveTo{pos}, isUnbreakable{isUnbreakable},
    containedEnemy{std::move(containedEnemy)}, direction{Direction::NONE} {}

void Block::setState(BlockState blockState) {
    if (this->state != blockState) {
        this->state = blockState;
        this->notifyObservers();
    }
}

void Block::setPositionToMoveTo(Direction d) {
    switch (d) {
        case Direction::RIGHT:
            this->positionToMoveTo[0] = this->position[0] + Constants::WIDTH_UNIT;
            break;
        case Direction::LEFT:
            this->positionToMoveTo[0] = this->position[0] - Constants::WIDTH_UNIT;
            break;
        case Direction::UP:
            this->positionToMoveTo[1] = this->position[1] + Constants::HEIGHT_UNIT;
            break;
        case Direction::DOWN:
            this->positionToMoveTo[1] = this->position[1] - Constants::HEIGHT_UNIT;
            break;
        default:
            break;
    }
}

void Block::push(Direction d, int stepRange) {
    this->direction = d;
    switch (d) {
        case Direction::RIGHT:
            this->positionToMoveTo[0] = this->position[0] + static_cast<float>(stepRange) * Constants::WIDTH_UNIT;
            break;
        case Direction::LEFT:
            this->positionToMoveTo[0] = this->position[0] - static_cast<float>(stepRange) * Constants::WIDTH_UNIT;
            break;
        case Direction::UP:
            this->positionToMoveTo[1] = this->position[1] + static_cast<float>(stepRange) * Constants::HEIGHT_UNIT;
            break;
        case Direction::DOWN:
            this->positionToMoveTo[1] = this->position[1] - static_cast<float>(stepRange) * Constants::HEIGHT_UNIT;
            break;
        default:
            break;
    }
}

void Block::move(float deltaTime) {
    // Movement to the right
    if (this->position[0] < this->positionToMoveTo[0]) {
        if (this->positionToMoveTo[0] - this->position[0] <= (Block::VELOCITY[0] * deltaTime)) {
            this->position[0] = this->positionToMoveTo[0];
            this->direction = Direction::NONE;
            this->killedWithOneMove = 0;
            // return true;
        } else {
            this->position[0] += Block::VELOCITY[0] * deltaTime;
            // return false;
        }
    }
    // Movement to the left
    if (this->position[0] > this->positionToMoveTo[0]) {
        if (this->positionToMoveTo[0] - this->position[0] >= -(Block::VELOCITY[0] * deltaTime)) {
            this->position[0] = this->positionToMoveTo[0];
            this->direction = Direction::NONE;
            this->killedWithOneMove = 0;
            // return true;
        } else {
            this->position[0] -= Block::VELOCITY[0] * deltaTime;
            // return false;
        }
    }
    // Movement up
    if (this->position[1] < this->positionToMoveTo[1]) {
        if (this->positionToMoveTo[1] - this->position[1] <= (Block::VELOCITY[1] * deltaTime)) {
            this->position[1] = this->positionToMoveTo[1];
            this->direction = Direction::NONE;
            this->killedWithOneMove = 0;
            // return true;
        } else {
            this->position[1] += Block::VELOCITY[1] * deltaTime;
            // return false;
        }
    }
    // Movement down
    if (this->position[1] > this->positionToMoveTo[1]) {
        if (this->positionToMoveTo[1] - this->position[1] >= -(Block::VELOCITY[1] * deltaTime)) {
            this->position[1] = this->positionToMoveTo[1];
            this->direction = Direction::NONE;
            this->killedWithOneMove = 0;
            // return true;
        } else {
            this->position[1] -= Block::VELOCITY[1] * deltaTime;
            // return false;
        }
    }
}
