#pragma once

#include <string>
#include <vector>

#include "GameObject.h"
#include "Enemy.h"

enum class BlockState
{
    SOLID,
    BREAKING,
    BROKEN,
    SPAWNING
};

class Block : public GameObject
{
public:
    inline static const std::array<float, 2> VELOCITY = { Constants::WIDTH_UNIT * 12, Constants::HEIGHT_UNIT * 12 };

    BlockState state = BlockState::SOLID;
    std::array<float, 2> positionToMoveTo;
    bool isUnbreakable;
    Enemy* containedEnemy;
    Direction direction;
    int killedWithOneMove = 0;

    Block();
    Block(std::array<float, 2> pos, Texture2D sprite, bool isUnbreakable, Enemy* containedEnemy);

    //virtual void registerObserver(const std::shared_ptr<Observer<Block*>>& observer);
    //virtual void removeObserver(const std::shared_ptr<Observer<Block*>>& observer);
    //virtual void notifyObservers();

    void setState(BlockState state);
    void setPositionToMoveTo(Direction direction);
    void push(Direction direction, int stepRange);
    void move(float deltaTime);

//private:
//    std::vector<std::shared_ptr<Observer<Block*>>> observers;
};