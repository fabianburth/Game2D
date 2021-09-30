#include "Game.h"

#include "../View/ResourceManager.h"

#include <array>
#include <iostream>
// Game-related State data

Game::Game(unsigned int width, unsigned int height) :
    PengoState(GameState::GAME_ACTIVE), Keys(), Width(width), Height(height) {}

void Game::Init() {
    // load shaders
    ResourceManager::LoadShader("../Game2D/res/shaders/sprites.vs", "../Game2D/res/shaders/sprites.fs", "sprite");
    // configure shaders
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    // set render-specific controls
    Shader shader = ResourceManager::GetShader("sprite");
    this->Renderer = new SpriteRenderer(shader);
    // this->soundModule = new SoundModule();

    // load levels
    GameLevel one;
    one.Load("../Game2D/res/levels/level1.lvl");
    GameLevel two;
    two.Load("../Game2D/res/levels/level0.lvl");

    this->Levels.push_back(one);
    this->Levels.push_back(two);
    this->Level = 0;

    this->initLevel();
}

void Game::Update(float dt) {
    if (this->PengoState == GameState::GAME_ACTIVE) {

        // Check if pengo died (= no lives left, cause he was touched by enemies)
        if (this->Levels[this->Level].Pengo.lives <= 0) {
            std::cout << "GAME OVER" << std::endl;
            PengoState = GameState::GAME_MENU;
            return;
        }

        // Check if level is completed (= all enemies killed)
        if (this->Levels[this->Level].IsCompleted()) {
            if (this->Level < this->Levels.size()) {
                initLevel();
                return;
            }
        }

        this->Levels[this->Level].updateGameState(dt);
        this->Renderer->updateView(dt);
    }
}

void Game::ProcessInput(float dt) {
    if (this->PengoState == GameState::GAME_ACTIVE) {
        if (this->Levels[this->Level].Pengo.ready) {
            if (this->Keys[GLFW_KEY_LEFT_CONTROL]) {
                this->Levels[this->Level].processPengoAttack();
                return;
            }
            if (this->Keys[GLFW_KEY_D]) {
                this->Levels[this->Level].processPengoMovement(Direction::RIGHT);
                return;
            }
            if (this->Keys[GLFW_KEY_A]) {
                this->Levels[this->Level].processPengoMovement(Direction::LEFT);
                return;
            }
            if (this->Keys[GLFW_KEY_W]) {
                this->Levels[this->Level].processPengoMovement(Direction::UP);
                return;
            }
            if (this->Keys[GLFW_KEY_S]) {
                this->Levels[this->Level].processPengoMovement(Direction::DOWN);
                return;
            }
        }
    }
}

void Game::Render() {
    if (this->Levels.size() > this->Level) {
        this->Renderer->DrawLevel(this->Levels[this->Level]);
    }
}


void Game::initLevel() {

    if (this->Levels[this->Level].Enemies.empty()) {
        this->Renderer->blockAnimators.clear();
        this->Renderer->enemyAnimators.clear();
        this->Levels[this->Level].removeObserver(this);
        this->Levels[this->Level].removeObserver(this->Renderer);
        this->Levels[this->Level].removeObserver(this->soundModule);

        this->carryOverScore();
        ++this->Level;

        if (this->Levels.size() <= this->Level) {
            std::cout << "GAME WON" << std::endl;
            PengoState = GameState::GAME_MENU;
            return;
        }
    }
    this->Levels[this->Level].registerObserver(this);
    this->Levels[this->Level].registerObserver(this->Renderer);
    this->Levels[this->Level].registerObserver(this->soundModule);
    this->Renderer->initLevelView(&this->Levels[this->Level]);
    this->Levels[this->Level].initStates();
}

void Game::carryOverScore() {
    this->Levels[this->Level + 1].score.score = this->Levels[this->Level].score.score;
}
