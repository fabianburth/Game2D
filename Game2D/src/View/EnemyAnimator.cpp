#include "EnemyAnimator.h"
#include "../util.h"

#include <utility>


EnemyAnimator::EnemyAnimator(std::shared_ptr<Enemy> enemy, float walkAnimationDuration, float stunnedAnimationDuration,
                             float spawningAnimationDuration) :
    enemy(std::move(enemy)),
    WALK_ANIMATION_DURATION(walkAnimationDuration), STUNNED_ANIMATION_DURATION(stunnedAnimationDuration),
    SPAWNING_ANIMATION_DURATION(spawningAnimationDuration) {
    switch (this->enemy->type) {
        case (EnemyType::WANDERING):
            spriteType = "Move";
            break;
        case (EnemyType::CHASING):
            spriteType = "Boxer";
            break;
        default:
            break;
    }
    this->sprite = ResourceManager::GetTexture("enemy" + this->spriteType + stringDirection(Direction::DOWN) + "00");
    this->enemy->registerObserver(this);
}

void EnemyAnimator::animate(float dt) {
    if (currentAnimation != nullptr) {
        (*this.*currentAnimation)();
        currentAnimationDuration += dt;
    }
}

void EnemyAnimator::update(GameObject * /*s*/) {
    // Enemy* enemy = dynamic_cast<Enemy*>(s);
    switch (this->enemy->state) {
        // case(EnemyState::WANDERING):
        // case(EnemyState::CHASING):
        //	currentAnimationDuration = 0.0f;
        //	currentAnimation = &EnemyAnimator::walk;
        //	break;
        case (EnemyState::BREAKING):
            currentAnimationDuration = 0.0f;
            currentAnimation = &EnemyAnimator::breakBlock;
            break;
        case (EnemyState::STUNNED):
            currentAnimationDuration = 0.0f;
            currentAnimation = &EnemyAnimator::stunned;
            break;
        case (EnemyState::SPAWNING):
            currentAnimationDuration = 0.0f;
            currentAnimation = &EnemyAnimator::spawningFromBlock;
            break;
        case (EnemyState::INITIAL_SPAWNING):
            currentAnimationDuration = 0.0f;
            currentAnimation = &EnemyAnimator::spawning;
            break;
        case (EnemyState::NONE):
            currentAnimationDuration = 0.0f;
            currentAnimation = &EnemyAnimator::walk;
            break;
    }

    switch (this->enemy->type) {
        case (EnemyType::WANDERING):
            spriteType = "Move";
            break;
        case (EnemyType::CHASING):
            spriteType = "Boxer";
            break;
        default:
            break;
    }
}

void EnemyAnimator::walk() {
    if (currentAnimationDuration == 0.0f) {
        this->sprite =
                ResourceManager::GetTexture("enemy" + this->spriteType + stringDirection(enemy->direction) + "00");
    } else if (currentAnimationDuration > 0.5f * WALK_ANIMATION_DURATION &&
               currentAnimationDuration <= WALK_ANIMATION_DURATION) {
        this->sprite =
                ResourceManager::GetTexture("enemy" + this->spriteType + stringDirection(enemy->direction) + "01");
    } else if (currentAnimationDuration > WALK_ANIMATION_DURATION) {
        currentAnimationDuration = 0.0f;
        this->sprite =
                ResourceManager::GetTexture("enemy" + this->spriteType + stringDirection(enemy->direction) + "00");
    }
}

void EnemyAnimator::breakBlock() {
    enemy->ready = false;
    if (currentAnimationDuration == 0.0f) {
        this->sprite =
                ResourceManager::GetTexture("enemy" + this->spriteType + stringDirection(enemy->direction) + "00");
    } else if (currentAnimationDuration > 0.25f * WALK_ANIMATION_DURATION &&
               currentAnimationDuration <= 0.5f * WALK_ANIMATION_DURATION) {
        this->sprite =
                ResourceManager::GetTexture("enemy" + this->spriteType + stringDirection(enemy->direction) + "01");
    } else if (currentAnimationDuration > 0.5f * WALK_ANIMATION_DURATION &&
               currentAnimationDuration <= 0.75f * WALK_ANIMATION_DURATION) {
        this->sprite =
                ResourceManager::GetTexture("enemy" + this->spriteType + stringDirection(enemy->direction) + "00");
    } else if (currentAnimationDuration > 0.75f * WALK_ANIMATION_DURATION &&
               currentAnimationDuration <= WALK_ANIMATION_DURATION) {
        this->sprite =
                ResourceManager::GetTexture("enemy" + this->spriteType + stringDirection(enemy->direction) + "01");
    } else if (currentAnimationDuration > WALK_ANIMATION_DURATION) {
        this->sprite =
                ResourceManager::GetTexture("enemy" + this->spriteType + stringDirection(enemy->direction) + "00");
        // enemy->setState(EnemyState::CHASING);
        // enemy->setPositionToMoveTo();
    }
}

void EnemyAnimator::stunned() {
    enemy->ready = false;
    if (currentAnimationDuration == 0.0f) {
        this->sprite = ResourceManager::GetTexture("enemyStunnedRight");
    } else if (currentAnimationDuration > (1.0f / 12.0f) * STUNNED_ANIMATION_DURATION &&
               currentAnimationDuration <= (2.0f / 12.0f) * STUNNED_ANIMATION_DURATION) {
        this->sprite = ResourceManager::GetTexture("enemyStunnedLeft");
    } else if (currentAnimationDuration > (2.0f / 12.0f) * STUNNED_ANIMATION_DURATION &&
               currentAnimationDuration <= (3.0f / 12.0f) * STUNNED_ANIMATION_DURATION) {
        this->sprite = ResourceManager::GetTexture("enemyStunnedRight");
    } else if (currentAnimationDuration > (3.0f / 12.0f) * STUNNED_ANIMATION_DURATION &&
               currentAnimationDuration <= (4.0f / 12.0f) * STUNNED_ANIMATION_DURATION) {
        this->sprite = ResourceManager::GetTexture("enemyStunnedLeft");
    } else if (currentAnimationDuration > (4.0f / 12.0f) * STUNNED_ANIMATION_DURATION &&
               currentAnimationDuration <= (5.0f / 12.0f) * STUNNED_ANIMATION_DURATION) {
        this->sprite = ResourceManager::GetTexture("enemyStunnedRight");
    } else if (currentAnimationDuration > (5.0f / 12.0f) * STUNNED_ANIMATION_DURATION &&
               currentAnimationDuration <= (6.0f / 12.0f) * STUNNED_ANIMATION_DURATION) {
        this->sprite = ResourceManager::GetTexture("enemyStunnedLeft");
    } else if (currentAnimationDuration > (6.0f / 12.0f) * STUNNED_ANIMATION_DURATION &&
               currentAnimationDuration <= (7.0f / 12.0f) * STUNNED_ANIMATION_DURATION) {
        this->sprite = ResourceManager::GetTexture("enemyStunnedRight");
    } else if (currentAnimationDuration > (7.0f / 12.0f) * STUNNED_ANIMATION_DURATION &&
               currentAnimationDuration <= (8.0f / 12.0f) * STUNNED_ANIMATION_DURATION) {
        this->sprite = ResourceManager::GetTexture("enemyStunnedLeft");
    } else if (currentAnimationDuration > (8.0f / 12.0f) * STUNNED_ANIMATION_DURATION &&
               currentAnimationDuration <= (9.0f / 12.0f) * STUNNED_ANIMATION_DURATION) {
        this->sprite = ResourceManager::GetTexture("enemyStunnedRight");
    } else if (currentAnimationDuration > (9.0f / 12.0f) * STUNNED_ANIMATION_DURATION &&
               currentAnimationDuration <= (10.0f / 12.0f) * STUNNED_ANIMATION_DURATION) {
        this->sprite = ResourceManager::GetTexture("enemyStunnedLeft");
    } else if (currentAnimationDuration > (10.0f / 12.0f) * STUNNED_ANIMATION_DURATION &&
               currentAnimationDuration <= (11.0f / 12.0f) * STUNNED_ANIMATION_DURATION) {
        this->sprite = ResourceManager::GetTexture("enemyStunnedRight");
    } else if (currentAnimationDuration > (11.0f / 12.0f) * STUNNED_ANIMATION_DURATION &&
               currentAnimationDuration <= STUNNED_ANIMATION_DURATION) {
        this->sprite = ResourceManager::GetTexture("enemyStunnedLeft");
    } else if (currentAnimationDuration > STUNNED_ANIMATION_DURATION) {
        /*if (this->spriteType == "Boxer")
            enemy->setState(EnemyState::CHASING);
        else
            enemy->setState(EnemyState::WANDERING);*/

        this->sprite =
                ResourceManager::GetTexture("enemy" + this->spriteType + stringDirection(enemy->direction) + "00");
        currentAnimationDuration = 0.0f;
        // enemy->ready = true;
    }
}

void EnemyAnimator::spawning() {
    enemy->ready = false;
    if (currentAnimationDuration == 0) {
        this->sprite = ResourceManager::GetTexture("enemySpawn0");
    } else if (currentAnimationDuration > (1.0f / 8.0f) * SPAWNING_ANIMATION_DURATION &&
               currentAnimationDuration <= (2.0f / 8.0f) * SPAWNING_ANIMATION_DURATION) {
        this->sprite = ResourceManager::GetTexture("enemySpawn1");
    } else if (currentAnimationDuration > (2.0f / 8.0f) * SPAWNING_ANIMATION_DURATION &&
               currentAnimationDuration <= (3.0f / 8.0f) * SPAWNING_ANIMATION_DURATION) {
        this->sprite = ResourceManager::GetTexture("enemySpawn2");
    } else if (currentAnimationDuration > (3.0f / 8.0f) * SPAWNING_ANIMATION_DURATION &&
               currentAnimationDuration <= (4.0f / 8.0f) * SPAWNING_ANIMATION_DURATION) {
        this->sprite = ResourceManager::GetTexture("enemySpawn3");
    } else if (currentAnimationDuration > (4.0f / 8.0f) * SPAWNING_ANIMATION_DURATION &&
               currentAnimationDuration <= (5.0f / 8.0f) * SPAWNING_ANIMATION_DURATION) {
        this->sprite = ResourceManager::GetTexture("enemySpawn4");
    } else if (currentAnimationDuration > (5.0f / 8.0f) * SPAWNING_ANIMATION_DURATION &&
               currentAnimationDuration <= (6.0f / 8.0f) * SPAWNING_ANIMATION_DURATION) {
        this->sprite = ResourceManager::GetTexture("enemySpawn5");
    } else if (currentAnimationDuration > (6.0f / 8.0f) * SPAWNING_ANIMATION_DURATION &&
               currentAnimationDuration <= (7.0f / 8.0f) * SPAWNING_ANIMATION_DURATION) {
        this->sprite = ResourceManager::GetTexture("enemySpawn6");
    } else if (currentAnimationDuration > (7.0f / 8.0f) * SPAWNING_ANIMATION_DURATION &&
               currentAnimationDuration <= SPAWNING_ANIMATION_DURATION) {
        this->sprite = ResourceManager::GetTexture("enemySpawn7");
    } else if (currentAnimationDuration > SPAWNING_ANIMATION_DURATION) {
        /*if (this->spriteType == "Boxer")
            enemy->setState(EnemyState::CHASING);
        else
            enemy->setState(EnemyState::WANDERING);*/

        this->sprite =
                ResourceManager::GetTexture("enemy" + this->spriteType + stringDirection(enemy->direction) + "00");
        currentAnimationDuration = 0.0f;
        // enemy->ready = true;
    }
}

void EnemyAnimator::spawningFromBlock() {
    enemy->ready = false;
    if (currentAnimationDuration == 0) {
        this->sprite = ResourceManager::GetTexture("iceblockBreaking0");
    } else if (currentAnimationDuration >= (1.0f / 12.0f) * SPAWNING_ANIMATION_DURATION &&
               currentAnimationDuration < (2.0f / 12.0f) * SPAWNING_ANIMATION_DURATION) {
        this->sprite = ResourceManager::GetTexture("iceblockBreaking1");
    } else if (currentAnimationDuration >= (2.0f / 12.0f) * SPAWNING_ANIMATION_DURATION &&
               currentAnimationDuration < (3.0f / 12.0f) * SPAWNING_ANIMATION_DURATION) {
        this->sprite = ResourceManager::GetTexture("iceblockBreaking2");
    } else if (currentAnimationDuration >= (3.0f / 12.0f) * SPAWNING_ANIMATION_DURATION &&
               currentAnimationDuration < (4.0f / 12.0f) * SPAWNING_ANIMATION_DURATION) {
        this->sprite = ResourceManager::GetTexture("iceblockBreaking3");
    } else if (currentAnimationDuration > (4.0f / 12.0f) * SPAWNING_ANIMATION_DURATION &&
               currentAnimationDuration <= (5.0f / 12.0f) * SPAWNING_ANIMATION_DURATION) {
        this->sprite = ResourceManager::GetTexture("enemySpawn0");
    } else if (currentAnimationDuration > (5.0f / 12.0f) * SPAWNING_ANIMATION_DURATION &&
               currentAnimationDuration <= (6.0f / 12.0f) * SPAWNING_ANIMATION_DURATION) {
        this->sprite = ResourceManager::GetTexture("enemySpawn1");
    } else if (currentAnimationDuration > (6.0f / 12.0f) * SPAWNING_ANIMATION_DURATION &&
               currentAnimationDuration <= (7.0f / 12.0f) * SPAWNING_ANIMATION_DURATION) {
        this->sprite = ResourceManager::GetTexture("enemySpawn2");
    } else if (currentAnimationDuration > (7.0f / 12.0f) * SPAWNING_ANIMATION_DURATION &&
               currentAnimationDuration <= (8.0f / 12.0f) * SPAWNING_ANIMATION_DURATION) {
        this->sprite = ResourceManager::GetTexture("enemySpawn3");
    } else if (currentAnimationDuration > (8.0f / 12.0f) * SPAWNING_ANIMATION_DURATION &&
               currentAnimationDuration <= (9.0f / 12.0f) * SPAWNING_ANIMATION_DURATION) {
        this->sprite = ResourceManager::GetTexture("enemySpawn4");
    } else if (currentAnimationDuration > (9.0f / 12.0f) * SPAWNING_ANIMATION_DURATION &&
               currentAnimationDuration <= (10.0f / 12.0f) * SPAWNING_ANIMATION_DURATION) {
        this->sprite = ResourceManager::GetTexture("enemySpawn5");
    } else if (currentAnimationDuration > (10.0f / 12.0f) * SPAWNING_ANIMATION_DURATION &&
               currentAnimationDuration <= (11.0f / 12.0f) * SPAWNING_ANIMATION_DURATION) {
        this->sprite = ResourceManager::GetTexture("enemySpawn6");
    } else if (currentAnimationDuration > (10.0f / 12.0f) * SPAWNING_ANIMATION_DURATION &&
               currentAnimationDuration <= SPAWNING_ANIMATION_DURATION) {
        this->sprite = ResourceManager::GetTexture("enemySpawn7");
    } else if (currentAnimationDuration > SPAWNING_ANIMATION_DURATION) {
        /*if (this->spriteType == "boxer")
            enemy->setState(EnemyState::CHASING);
        else
            enemy->setState(EnemyState::WANDERING);*/

        this->sprite =
                ResourceManager::GetTexture("enemy" + this->spriteType + stringDirection(enemy->direction) + "00");
        currentAnimationDuration = 0.0f;
        // enemy->ready = true;
    }
}
