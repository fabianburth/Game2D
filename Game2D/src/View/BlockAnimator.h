#pragma once

#include "../Animator.h"
#include "../Model/Block.h"
#include "../Model/GameObject.h"
#include "ResourceManager.h"

#include <any>

class BlockAnimator : public Animator<GameObject> {
  public:
    Block *block = nullptr;
    Texture2D sprite;
    const float BREAKING_DURATION = 0.0f;
    const float FLASHING_DURATION = 0.0f;
    void (BlockAnimator::*currentAnimation)() = nullptr;

    BlockAnimator(Block *block, float flashingDuration, float breakingDuration, Texture2D sprite);
    void update(GameObject *s) override;
    void animate(float dt) override;
    void destroyIceblock();
    // void spawnEnemy();
    void flash();
};