#pragma once

#include "../Animator.h"
#include "../Model/Enemy.h"
#include "ResourceManager.h"

#include <string>

class EnemyAnimator : public Animator<GameObject> {
  public:
    std::shared_ptr<Enemy> enemy;
    Texture2D sprite;
    std::string spriteType;
    const float WALK_ANIMATION_DURATION;
    const float STUNNED_ANIMATION_DURATION;
    const float SPAWNING_ANIMATION_DURATION;
    void (EnemyAnimator::*currentAnimation)() = nullptr;

    EnemyAnimator(std::shared_ptr<Enemy> enemy, float walkAnimationDuration, float stunnedAnimationDuration,
                  float spawningAnimationDuration);
    void update(GameObject *s) override;
    void animate(float dt) override;
    void walk();
    void breakBlock();
    void stunned();
    void spawning();
    void spawningFromBlock();
};