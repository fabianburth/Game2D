#pragma once

#include "../util.h"
#include "GameObject.h"

#include <string>
#include <vector>

class Player : public GameObject {
  public:
    Direction direction = Direction::NONE;
    PengoState state = PengoState::STAND;
    std::array<float, 2> velocity{}, positionToMoveTo{};
    bool ready{};
    int lives = 1;

    static constexpr float WALKING_DURATION = 0.25f;
    static constexpr float PUSHING_DURATION = 0.5f;
    static constexpr float BREAKING_BLOCK_DURATION = 0.5f;
    float walkingFor = 0.0f;
    float pushingFor = 0.0f;
    float breakingBlockFor = 0.0f;

    Player() = default;
    Player(std::array<float, 2> pos, std::array<float, 2> velocity);

    // virtual void registerObserver(const std::shared_ptr<Observer>& observer);
    // virtual void removeObserver(const std::shared_ptr<Observer>& observer);
    // virtual void notifyObservers();

    void setDirection(Direction playerDirection);
    void setState(PengoState pengoState);
    void setPositionToMoveTo();
    void move(float deltaTime);
    // void swapSprite();

    // private:
    //	std::vector<std::shared_ptr<Observer>> observers;
};