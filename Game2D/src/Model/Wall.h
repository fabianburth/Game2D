#pragma once

#include "../Subject.h"
#include "GameObject.h"

#include <vector>
//#include "../View/ResourceManager.h"

enum class WallState { SOLID, WOBBLY };

enum class WallSide { TOP, BOTTOM, RIGHT, LEFT };

class Wall : public Subject<Wall> {
  public:
    WallState state = WallState::SOLID;
    WallSide side{};
    std::vector<GameObject> wallComponents;

    static constexpr float WOBBLY_DURATION = 0.5f;
    float wobblyFor = 0.0f;

    Wall() = default;
    explicit Wall(WallSide side);
    void setState(WallState wallState);
    // void setSprite(Texture2D sprite);
    void addWallComponent(const GameObject &component);

    void registerObserver(Observer<Wall> *o) override;
    void removeObserver(Observer<Wall> *o) override;
    void notifyObservers() override;

  private:
    std::vector<Observer<Wall> *> observers;
};