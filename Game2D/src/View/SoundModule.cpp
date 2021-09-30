#include "SoundModule.h"

/*void SoundModule::update(GameLevel* gameLevel)
{
    switch (gameLevel->event)
    {
    case Events::ENEMY_KILLED:
        break;
    case Events::ENEMY_SPAWNED:
        soundManager->play("snowBeeSpawning");
        break;
    case Events::PENGO_DIED:
        soundManager->play("pengoDied");
        soundManager->stop("suddenDeathBackgroundTheme");
        soundManager->stop("backgroundTheme");
        break;
    case Events::STUNNED_ENEMY_KILLED:
        soundManager->play("touchSnowBee");
        break;
    case Events::ENEMY_STUNNED:
        break;
    case Events::BREAK_ICE_BLOCK:
        soundManager->play("destroyIceblock");
        break;
    case Events::PUSH_BLOCK:
        soundManager->play("pushIceBlock");
        break;
    case Events::WOBBLE_WALL:
        soundManager->play("pushWall");
        break;
    case Events::SUDDEN_DEATH:
        soundManager->stop("backgroundTheme");
        soundManager->play("suddenDeathBackgroundTheme");
        break;
    case Events::ENEMY_CRUSHED:
        soundManager->play("snowBeeCrushed");
        break;
    case Events::DIAMONDS_ALIGNED:
        soundManager->play("diamondsAligned");
        break;
    default:
        break;
    }
}

SoundModule::SoundModule()
{
    this->initGameSounds();
}

void SoundModule::initGameSounds()
{
    this->soundManager = SoundManager::getInstance();
    this->soundManager->loadAudioData(_TEXT("res\\sounds\\PushIceBlock.wav"), "pushIceBlock", false);
    this->soundManager->loadAudioData(_TEXT("res\\sounds\\IceBlockDestroyed.wav"), "destroyIceblock", false);
    this->soundManager->loadAudioData(_TEXT("res\\sounds\\PushWall.wav"), "pushWall", false);
    this->soundManager->loadAudioData(_TEXT("res\\sounds\\SnowBeeSpawning.wav"), "snowBeeSpawning", false);
    this->soundManager->loadAudioData(_TEXT("res\\sounds\\SnowBeeStunned.wav"), "snowBeeStunned", false);
    this->soundManager->loadAudioData(_TEXT("res\\sounds\\PengoDied.wav"), "pengoDied", false);
    this->soundManager->loadAudioData(_TEXT("res\\sounds\\TouchSnowBee.wav"), "touchSnowBee", false);
    this->soundManager->loadAudioData(_TEXT("res\\sounds\\SnowBeeStunned.wav"), "snowBeeStunned", false);
    this->soundManager->loadAudioData(_TEXT("res\\sounds\\SnowBeeSquashed.wav"), "snowBeeCrushed", false);
    this->soundManager->loadAudioData(_TEXT("res\\sounds\\DiamondBlocksLinedUp.wav"), "diamondsAligned", false);

    this->soundManager->loadAudioData(_TEXT("res\\sounds\\SuddenDeathTheme.wav"), "suddenDeathBackgroundTheme", true);
    this->soundManager->loadAudioData(_TEXT("res\\sounds\\BackgroundTheme.wav"), "backgroundTheme", true);
    this->soundManager->play("backgroundTheme");
}
*/