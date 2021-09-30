#pragma once
#ifndef GAMELEVEL_H
    #define GAMELEVEL_H
    #include <array>
    #include <ctime>
    #include <memory>
    #include <vector>

//#include <GL/glew.h>

    #include "../Constants.h"
    #include "Block.h"
    #include "GameObject.h"
    #include "Player.h"
    //#include "../View/ResourceManager.h"
    #include "Enemy.h"
    #include "Wall.h"
    //#include "../View/GameInformation.h"
    #include "../util.h"
    #include "Score.h"


/// GameLevel holds all Tiles as part of a Breakout level and
/// hosts functionality to Load/render levels from the harddisk.
///
/// In a level definition file, each number represents a type of block
/// 0: no block, empty space
/// 1: a destroyable ice block
/// 2: an undestroyable diamond block
/// 3: a sno-bee
/// 4: pengo
class GameLevel : public Subject<GameLevel> {
  public:
    // level state
    Player Pengo;
    std::vector<std::shared_ptr<Enemy>> Enemies;
    std::vector<std::shared_ptr<Enemy>> frozenEnemies;
    std::vector<GameObject> Walls;
    Wall LeftWall;
    Wall RightWall;
    Wall TopWall;
    Wall BottomWall;
    std::vector<Block> Blocks;

    /** Is set to the primary event in specific situations */
    Events event{};
    /** Keeps track of the players score and provides useful methods */
    Score score;

    /** Clock to track how long it takes the player to finish the level (relevant for score) */
    std::clock_t startClockLevel = 0;

    /** Clock to track how much time has passed since the last enemy was killed (relevant for enemy state) */
    std::clock_t startClockEnemyKill = 0;

    /** Is set to true when 3 diamond blocks get aligned */
    bool diamondBlocksAligned = false;


    GameLevel() = default;

    void registerObserver(Observer<GameLevel> *o) override;
    void removeObserver(Observer<GameLevel> *o) override;
    void notifyObservers() override;

    auto setEvent(Events e) -> void;

    /**
     * Loads level from file
     */
    auto Load(const char *file) -> void;

    /**
     * Check if the level is completed (all non-solid tiles are destroyed)
     */

    auto IsCompleted() -> bool;

    /**
     * Method which is called every game loop iteration which does some checks and updates the game state under certain
     * conditions
     * @param dt: The time passed since the last game loop iteration
     */
    auto updateGameState(float dt) -> void;


    //! Following methods are moved from the Controller (Game) to this Model (GameLevel) to abide with MVC
    /**
     * Processes an attack of the Player (=Pressing LeftCtrl) and hence determines whether a block is pushed or
     * destroyed or whether a wall will wobble
     */
    auto processPengoAttack() -> void;

    /**
     * Processes movement of the Player (=Pressing WASD) and hence determines where Pengo will go
     */
    auto processPengoMovement(Direction d) -> void;

    /**
     * Sets the states of blocks and enemies to an level start state
     * (Among other reasons, this is functionality is encapsulated in a separate method and is not part of the init
     * method, because this should be done AFTER the view is initialized so it gets informed about change of states)
     */
    auto initStates() -> void;

    // protected:
    std::vector<Observer<GameLevel> *> observers;

    /**
     * Initialize level from tile data
     * @param tileData: level data from txt-file in a vector
     */
    auto init(std::vector<std::vector<unsigned int>> tileData) -> void;

    //! Following methods are moved from the Controller (Game) to this Model (GameLevel) to abide with MVC

    /**
     * Auxiliary method for checkCollisions - checks whether GameObject one collides with GameObject two if it is moved
     * one unit in the given direction
     * @param one: The GameObject to be moved
     * @param two: The GameObject with which collision will be checked
     * @param d: The Direction to which it would be moved
     * @return true, if the blocks would collide, false otherwise
     */
    static auto checkBlockCollision(GameObject &one, GameObject &two, Direction d) -> bool;

    /**
     * Auxiliary method for checkCollisions - checks whether GameObject will be colliding with the wall when moved one
     * unit in the given Direction
     * @param one: The GameObject to be moved
     * @param d: The Direction to which it would be moved
     * @return true, if it would collide with the wall, false otherwise
     */
    static auto checkWallCollision(GameObject &one, Direction d) -> bool;

    /**
     * Auxiliary method, checks whether the given gameObject is touching the wall at the given side (used to check every
     * game loop iteration, whether enemies are touching the wall and need to be stunned, in case the wall is wobbly -
     * thus and offset of 1 WIDTH_UNIT as in checkWallCollision would not make sense and lead to enemies being stunned
     * too early)
     * @param gameObject: The GameObject to check the collision for
     * @param direction: The Direction in which collision has to be checked
     * @return true, if it is touching the wall, false otherwise
     */
    static auto checkWallCollisionPrecise(GameObject &one, Direction d) -> bool;

    /**
     * Auxiliary method which returns a vector containing all directions which are possible for the enemy to move to
     * 1) In case the enemy is in state wandering, the vector is ordered like:
     *    [current direction, not current and not opposite, not current and not opposite, opposite direction]
     *    and then the not possible directions (because there is a wall or a block) are removed from the array
     * 2) In case the enemy is in state chasing, the vector is ordered like:
     *    [direction with the greatest distance to pengo, not greatest distance to pengo and not opposite, not greatest
     * distance to pengo and not opposite, opposite direction] and then the not possible directions (because there is a
     * wall or an unbreakable block) are removed from the array
     * @param enemy: The enemy for which the directions array shall get generated
     * @return: The vector containing the possible directions
     */
    auto getInitialDirections(Enemy &enemy) -> std::vector<Direction>;

    /**
     * Auxiliary method which determines (an index of a) direction based on the probability vector
     * @param chances: A probability vector for this enemy
     * @return: The index of the direction in a direction vector
     */
    static auto getDirectionIndex(const std::vector<int> &chances) -> int;

    /**
     * Auxiliary method which checks whether 3 diamond blocks got aligned and updates the score if so
     */
    void checkThreeDiamonds();

    /**
     * Auxiliary method which checks whether a block touches a wall
     * @param b: The block to check whether it touches the wall
     * @return true, if it touches the wall, false otherwise
     */
    static auto blockTouchesWall(Block &b) -> bool;

    /**
     * Auxiliary method which determines and update the game state concerning bots behavior, movement and their
     * interaction with the player and the environment
     * @param dt: The time passed since the last game loop iteration
     */
    auto determineBotBehavior(float dt) -> void;

    /**
     * Auxiliary method which updates the game state concerning block movement and their interaction with the
     * environment
     * @param dt: The time passed since the last game loop iteration
     */
    auto updateBlockInteractions(float dt) -> void;

    /**
     * Sets one of the remaining active enemies to type boxer if possible
     */
    auto trySettingBoxer() -> void;

    /**
     * Checks whether an enemy of type boxer exists at the moment
     * @return true, if one exists, false otherwise
     */
    auto boxerExists() -> bool;

    /**
     * Spawns one of the currently frozen enemies
     */
    auto spawnEnemy() -> void;

    /**
     * Checks whether the given gameObject is colliding with another block or a wall when moving one unit to the given
     * direction
     * @param gameObject: The GameObject to check the collision for
     * @param direction: The Direction in which collision has to be checked
     * @return true, if there is a collision, false otherwise
     */
    auto checkCollisions(GameObject &gameObject, Direction d) -> bool;

    /**
     * Return the Block with which the given GameObject is colliding
     * @param gameObject: The GameObject to check the collision for
     * @param direction: The Direction in which collision has to be checked
     * @return a pointer to the gameObject with which the given GameObject is colliding or nullptr otherwise
     */
    auto getCollisionBlock(GameObject &gameObject, Direction d) -> Block *;

    /**
     * Calculates how many units the given GameObject could move until colliding with another block or the wall (step
     * range)
     * @param gameObject: The GameObject the step range has to be calculated for
     * @param direction: The Direction in which the step range has to be checked
     */
    auto calculateStepRange(Block &block, Direction d) -> int;

    /**
     * Checks whether the given gameObject is colliding with another gameObject
     * @param two: The GameObject to check the collision
     * @param one: The GameObject to check the collision
     */
    static auto checkCollisionPrecise(GameObject &one, GameObject &two) -> bool;

    /**
     * Removes the enemy from the vector of active enemies
     * @param enemy: A pointer to the enemy to be removed
     */
    auto killEnemy(std::shared_ptr<Enemy> &enemy) -> void;
};

#endif