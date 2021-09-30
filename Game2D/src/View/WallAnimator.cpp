#include "WallAnimator.h"

#include "SoundManager.h"

WallAnimator::WallAnimator(Wall *wall, float wobbleDuration) : wall{wall}, WOBBLE_DURATION{wobbleDuration} {
    switch (wall->side) {
        case (WallSide::TOP):
        case (WallSide::BOTTOM):
            standardSprite = ResourceManager::GetTexture("wallBT");
            wobblySprite0 = ResourceManager::GetTexture("wallBT_00");
            wobblySprite1 = ResourceManager::GetTexture("wallBT_01");
            break;
        case (WallSide::LEFT):
        case (WallSide::RIGHT):
            standardSprite = ResourceManager::GetTexture("wallLR");
            wobblySprite0 = ResourceManager::GetTexture("wallLR_00");
            wobblySprite1 = ResourceManager::GetTexture("wallLR_01");
    }
    this->sprite = this->standardSprite;
    wall->registerObserver(this);
}

void WallAnimator::update(Wall * /*s*/) {
    switch (wall->state) {
        case (WallState::WOBBLY):
            currentAnimationDuration = 0.0f;
            currentAnimation = &WallAnimator::wobble;
            break;
        default:
            break;
    }

    //	if (s->state == WallState::WOBBLY)
    //	{
    //		this->wall = s;
    //		currentAnimation = &WallAnimator::wobble;
    //
    //		if (wall->side == WallSide::TOP || wall->side == WallSide::BOTTOM)
    //		{
    //			sprite = ResourceManager::GetTexture("wallBT");
    //			wobblySprite0 = ResourceManager::GetTexture("wallBT_00");
    //			wobblySprite1 = ResourceManager::GetTexture("wallBT_01");
    //		}
    //		else if (wall->side == WallSide::LEFT || wall->side == WallSide::RIGHT)
    //		{
    //			sprite = ResourceManager::GetTexture("wallLR");
    //			wobblySprite0 = ResourceManager::GetTexture("wallLR_00");
    //			wobblySprite1 = ResourceManager::GetTexture("wallLR_01");
    //		}
    //	}
}

void WallAnimator::animate(float dt) {
    if (currentAnimation != nullptr) {
        (*this.*currentAnimation)();
        currentAnimationDuration += dt;
    }
}

void WallAnimator::wobble() {
    if (currentAnimationDuration == 0) {
        this->sprite = wobblySprite0;
    } else if (currentAnimationDuration >= (1.0f / 5.0f) * WOBBLE_DURATION &&
               currentAnimationDuration < (2.0f / 5.0f) * WOBBLE_DURATION) {
        this->sprite = wobblySprite1;
    } else if (currentAnimationDuration >= (2.0f / 5.0f) * WOBBLE_DURATION &&
               currentAnimationDuration < (3.0f / 5.0f) * WOBBLE_DURATION) {
        this->sprite = wobblySprite0;
    } else if (currentAnimationDuration >= (3.0f / 5.0f) * WOBBLE_DURATION &&
               currentAnimationDuration < (4.0f / 5.0f) * WOBBLE_DURATION) {
        this->sprite = wobblySprite1;
    } else if (currentAnimationDuration >= WOBBLE_DURATION) {
        this->sprite = standardSprite;
        // wall->setState(WallState::SOLID);
        currentAnimation = nullptr;
        currentAnimationDuration = 0.0f;
    }
}
