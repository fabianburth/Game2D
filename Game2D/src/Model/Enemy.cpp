#include "Enemy.h"

unsigned int Enemy::nextID = 0;

Enemy::Enemy(std::array<float, 2> pos, std::array<float, 2> velocity, Direction direction, EnemyState state,
             EnemyType type, bool ready) :
    GameObject{pos},
    id{nextID}, direction{direction}, state{state}, type{type}, velocity{velocity},
    positionToMoveTo{pos}, ready{ready} {
    nextID++;
}

void Enemy::setDirection(Direction enemyDirection) {
    if (this->direction != enemyDirection) {
        this->direction = enemyDirection;
        this->notifyObservers();
    }
}

void Enemy::setState(EnemyState enemyState) {
    if (this->state != enemyState) {
        this->state = enemyState;
        this->notifyObservers();
    }
}

void Enemy::setType(EnemyType enemyType) {
    if (this->type != enemyType) {
        this->type = enemyType;
        this->notifyObservers();
    }
}

void Enemy::setPositionToMoveTo() {
    switch (this->direction) {
        case Direction::RIGHT:
            this->positionToMoveTo[0] = this->position[0] + Constants::WIDTH_UNIT;
            this->ready = false;
            break;
        case Direction::LEFT:
            this->positionToMoveTo[0] = this->position[0] - Constants::WIDTH_UNIT;
            this->ready = false;
            break;
        case Direction::UP:
            this->positionToMoveTo[1] = this->position[1] + Constants::HEIGHT_UNIT;
            this->ready = false;
            break;
        case Direction::DOWN:
            this->positionToMoveTo[1] = this->position[1] - Constants::HEIGHT_UNIT;
            this->ready = false;
            break;
        default:
            break;
    }
}

void Enemy::move(float deltaTime) {
    if (this->position[0] < this->positionToMoveTo[0]) {
        if (this->positionToMoveTo[0] - this->position[0] <= (this->velocity[0] * deltaTime)) {
            this->position[0] = this->positionToMoveTo[0];
            // setState(PengoState::STAND);
            this->ready = true;
        } else {
            this->position[0] += this->velocity[0] * deltaTime;
        }
    }
    // Movement to the left
    else if (this->position[0] > this->positionToMoveTo[0]) {
        if (this->positionToMoveTo[0] - this->position[0] >= -(this->velocity[0] * deltaTime)) {
            this->position[0] = this->positionToMoveTo[0];
            // setState(PengoState::STAND);
            this->ready = true;
        } else {
            this->position[0] -= this->velocity[0] * deltaTime;
        }
    }
    // Movement up
    else if (this->position[1] < this->positionToMoveTo[1]) {
        if (this->positionToMoveTo[1] - this->position[1] <= (this->velocity[1] * deltaTime)) {
            this->position[1] = this->positionToMoveTo[1];
            // setState(PengoState::STAND);
            this->ready = true;
        } else {
            this->position[1] += this->velocity[1] * deltaTime;
        }
    }
    // Movement down
    else if (this->position[1] > this->positionToMoveTo[1]) {
        if (this->positionToMoveTo[1] - this->position[1] >= -(this->velocity[1] * deltaTime)) {
            this->position[1] = this->positionToMoveTo[1];
            // setState(PengoState::STAND);
            this->ready = true;
        } else {
            this->position[1] -= this->velocity[1] * deltaTime;
        }
    } else {
        this->ready = true;
    }
}

auto Enemy::getProbabilityArray(std::vector<Direction> directions) const -> std::vector<int> {
    std::vector<int> probabilities;
    bool currentDirectionPossible = false;
    bool oppositeDirectionPossible = false;
    switch (directions.size()) {
        case 4:
            probabilities.push_back(90);
            probabilities.push_back(94);
            probabilities.push_back(98);
            probabilities.push_back(100);
            break;
        case 3:
            currentDirectionPossible =
                    std::find(directions.begin(), directions.end(), this->direction) != directions.end();
            oppositeDirectionPossible = std::find(directions.begin(), directions.end(),
                                                  oppositeDirection(this->direction)) != directions.end();
            if (currentDirectionPossible && oppositeDirectionPossible) {
                probabilities.push_back(80);
                probabilities.push_back(98);
                probabilities.push_back(100);
            } else if (currentDirectionPossible) {
                probabilities.push_back(80);
                probabilities.push_back(90);
                probabilities.push_back(100);
            } else if (oppositeDirectionPossible) {
                probabilities.push_back(49);
                probabilities.push_back(98);
                probabilities.push_back(100);
            }
            break;
        case 2:
            currentDirectionPossible =
                    std::find(directions.begin(), directions.end(), this->direction) != directions.end();
            oppositeDirectionPossible = std::find(directions.begin(), directions.end(),
                                                  oppositeDirection(this->direction)) != directions.end();
            if (currentDirectionPossible && oppositeDirectionPossible) {
                probabilities.push_back(98);
                probabilities.push_back(100);
            } else if (currentDirectionPossible) {
                probabilities.push_back(80);
                probabilities.push_back(100);
            } else if (oppositeDirectionPossible) {
                probabilities.push_back(98);
                probabilities.push_back(100);
            }
            break;
        case 1:
            probabilities.push_back(100);
            break;
        default:
            probabilities.push_back(100);
            break;
    }

    return probabilities;
}
