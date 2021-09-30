#include "PengoAnimator.h"

#include "SoundManager.h"

PengoAnimator::PengoAnimator(Player *pengo, float pushDuration, float walkAnimationDuration, Texture2D sprite) :
    pengo(pengo), sprite{sprite}, toggleSprite{true}, PUSH_DURATION(pushDuration),
    WALK_ANIMATION_DURATION(walkAnimationDuration), currentAnimation(&PengoAnimator::stand) {
    pengo->registerObserver(this);
}

void PengoAnimator::animate(float dt) {
    (*this.*currentAnimation)();
    currentAnimationDuration += dt;
}

void PengoAnimator::update(GameObject * /*s*/) {
    // SoundSource mySpeaker = SoundSource();
    // Player *pengo = dynamic_cast<Player *>(s);
    switch (pengo->state) {
        case (PengoState::STAND):
            currentAnimationDuration = 0.0f;
            currentAnimation = &PengoAnimator::stand;
            break;
        case (PengoState::WALK):
            currentAnimationDuration = 0.0f;
            currentAnimation = &PengoAnimator::walk;
            break;
        case (PengoState::PUSH):
            currentAnimationDuration = 0.0f;
            currentAnimation = &PengoAnimator::pushBlock;
            break;
        case (PengoState::BREAK):
            currentAnimationDuration = 0.0f;
            currentAnimation = &PengoAnimator::breakBlock;
            break;
    }
}

void PengoAnimator::stand() {
    // Last Sprite just stays active
}

void PengoAnimator::walk() {
    if (currentAnimationDuration == 0) {
        this->swapSprite();
    } /*else if (currentAnimationDuration >= WALK_ANIMATION_DURATION) {
        pengo->setState(PengoState::STAND);
    }*/
}

void PengoAnimator::pushBlock() {
    // pengo->ready = false;
    this->sprite = ResourceManager::GetTexture("pengoPush" + stringDirection(pengo->direction));
    if (currentAnimationDuration >= PUSH_DURATION) {
        this->sprite = ResourceManager::GetTexture("pengoPostPush" + stringDirection(pengo->direction));
        // pengo->state = PengoState::STAND;
        // pengo->ready = true;
    }
}

void PengoAnimator::breakBlock() {
    pengo->ready = false;
    if (currentAnimationDuration == 0) {
        this->sprite = ResourceManager::GetTexture("pengoPush" + stringDirection(pengo->direction));
    } else if (currentAnimationDuration >= (1.0f / 3.0f) * PUSH_DURATION &&
               currentAnimationDuration < (2.0f / 3.0f) * PUSH_DURATION) {
        this->sprite = ResourceManager::GetTexture("pengoPostPush" + stringDirection(pengo->direction));
    } else if (currentAnimationDuration >= (2.0f / 3.0f) * PUSH_DURATION && currentAnimationDuration < PUSH_DURATION) {
        this->sprite = ResourceManager::GetTexture("pengoPush" + stringDirection(pengo->direction));
    } else if (currentAnimationDuration >= PUSH_DURATION) {
        this->sprite = ResourceManager::GetTexture("pengoPostPush" + stringDirection(pengo->direction));
        pengo->state = PengoState::STAND;
        pengo->ready = true;
    }
}

void PengoAnimator::swapSprite() {
    if (toggleSprite) {
        toggleSprite = false;
        this->sprite = ResourceManager::GetTexture("pengo" + stringDirection(this->pengo->direction));
    } else {
        toggleSprite = true;
        this->sprite = ResourceManager::GetTexture("pengoMove" + stringDirection(this->pengo->direction));
    }
}
