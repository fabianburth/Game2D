#pragma once

#include "../Animator.h"
#include "../Model/Player.h"
#include "ResourceManager.h"

#include <any>
#include "../View/SoundSource.h"
#include "../View/SoundBuffer.h"

class PengoAnimator : public Animator<GameObject> {
  public:
    Player *pengo;
    Texture2D sprite;
    bool toggleSprite;
    const float PUSH_DURATION;
    const float WALK_ANIMATION_DURATION;
    void (PengoAnimator::*currentAnimation)();

    PengoAnimator(Player *pengo, float pushDuration, float walkAnimationDuration, Texture2D sprite);
    void update(GameObject *s) override;
    void animate(float dt) override;
    void stand();
    void walk();
    void pushBlock();
    void breakBlock();
    void swapSprite();
};