//
// Created by burth on 27.08.2021.
//

#include "Score.h"

void Score::addLevelCompletion(double levelDuration) {
    if (levelDuration < 20) {
        this->score += 5000;
    } else if (levelDuration < 30) {
        this->score += 2000;
    } else if (levelDuration < 40) {
        this->score += 1000;
    } else if (levelDuration < 50) {
        this->score += 500;
    } else if (levelDuration < 60) {
        this->score += 10;
    } else {
        this->score += 0;
    }
}

void Score::addDiamondsTouchingWall() {
    this->score += 5000;
}

void Score::addDiamondsNotTouchingWall() {
    this->score += 10000;
}

void Score::addWalkedOverStunnedEnemy() {
    this->score += 100;
}

void Score::addCrushedEnemy(int killedWithOneMove) {
    switch (killedWithOneMove) {
        case 0:
            this->score += 400;
            break;
        case 1:
            this->score += 1200;
            break;
        case 2:
            this->score += 1600;
            break;
        default:
            break;
    }
}

void Score::addDestroyBlock(Block *block) {
    if (block->containedEnemy == nullptr) {
        score += 30;
    } else {
        score += 500;
    }
}
