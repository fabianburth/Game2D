#pragma once
#include "../Model/GameLevel.h"
#include "../Observer.h"
#include "../util.h"
#include "SoundManager.h"
class SoundModule : public Observer<GameLevel> {
  public:
    void update(GameLevel *gameLevel) override;

    SoundModule();

  private:
    // SoundManager* soundManager;
    void initGameSounds();
};