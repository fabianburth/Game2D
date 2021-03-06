#pragma once

#ifndef GAME_H
#define GAME_H

#include "../Constants.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <algorithm>
#include <cstdlib>
#include "../Model/GameLevel.h"
#include "../Model/Player.h"
#include "../View/SpriteRenderer.h"
#include "../Model/Wall.h"
#include <string>
#include <ctime>
#include <algorithm>

// Represents the current state of the game
enum class GameState 
{
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

const float EPSILON = 0.01;


// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game : public Observer<GameLevel>
{
public:
    // game state
    GameState    PengoState;
    SpriteRenderer* Renderer;
    bool         Keys[1024];
    unsigned int Width, Height;
    std::vector<GameLevel>  Levels;
    unsigned int            Level;
    unsigned int score = 0;
    // constructor/destructor
    Game(unsigned int width, unsigned int height);
    ~Game();
    // initialize game state (load all shaders/textures/levels)
    void Init();
    // game loop
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();

    void update(GameLevel* s) override;

private:
    /**
     * Manages the begin of the level and also the transition to another level
     * Initiates the initialization of states and also binds all observers necessary
     */
    void initLevel();

    /** Carries the score of the finished level over to the next level */
    void carryOverScore();
};

#endif