#pragma once

#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include "../Constants.h"

#include <GL/glew.h>

#include "Texture.h"
#include "Shader.h"
#include "../Model/GameObject.h"
#include "../Model/GameLevel.h"
#include "GameInformation.h"
#include "../View/PengoAnimator.h"
#include "../View/BlockAnimator.h"
#include "../View/WallAnimator.h"
#include "../View/EnemyAnimator.h"
#include "../View/SoundDevice.h"
#include "../View/SoundBuffer.h"

#include <array>

// Units
//const float WIDTH_UNIT = ((2.0f / 448) * 32.0f);
//const float HEIGHT_UNIT = ((2.0f / 576.0f) * 32.0f);

class SpriteRenderer: public Observer<GameLevel>
{
public:
    //SoundDevice * soundDevice;
    //View Objects
    PengoAnimator* pengoAnimator;
    //BlockAnimator* blockAnimator;
    WallAnimator* leftWallAnimator;
    WallAnimator* rightWallAnimator;
    WallAnimator* bottomWallAnimator;
    WallAnimator* topWallAnimator;

    std::vector<BlockAnimator*> blockAnimators;
    std::vector<EnemyAnimator*> enemyAnimators;

    GameInformation P1;
    GameInformation Score;

    // Constructor (inits shaders/shapes)
    SpriteRenderer() = default;
    SpriteRenderer(Shader& shader);
    // Destructor
    ~SpriteRenderer();
    // Renders a defined quad textured with given sprite
    void DrawSprite(Texture2D& texture, std::array<float, 2> position, std::array<float, 4> size, std::array<float, 3> color = { 1.0f,1.0f,1.0f });
    
    void DrawLevel(GameLevel& gameLevel);

    void updateView(float dt);

    void initLevelView(GameLevel* gameLevel);

    void update(GameLevel* gameLevel) override;

private:
    // Render state
    Shader       shader;
    unsigned int quadVAO;
    // Initializes and configures the quad's buffer and vertex attributes
    // Thus, it creates the a quad at the bottom left corner
    void initRenderData();

    //void DrawObject(GameObject& gameObject);
    void DrawDisplayElement(GameInformation& gameInformation);

    void initDisplayInformation();

    void loadSprites();

    void removeAnimatorOfKilledEnemy(std::shared_ptr<Enemy>& enemy);

    void createAnimatorForSpawnedEnemy(std::shared_ptr<Enemy>& enemy);
};

#endif