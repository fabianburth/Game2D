#pragma once
#include "../util.h"
#include "GameObject.h"
#include "../util.h"

enum class EnemyType { CHASING, WANDERING };

enum class EnemyState { BREAKING, STUNNED, SPAWNING, INITIAL_SPAWNING, NONE };

class Enemy : public GameObject {
  public:
    unsigned int id{};
    Direction direction;
    EnemyState state;
    EnemyType type;
    std::array<float, 2> velocity{}, positionToMoveTo{};
    bool ready{};

    static constexpr float BREAKING_BLOCK_DURATION = 0.5f;
    static constexpr float STUNN_DURATION = 3.0f;
    static constexpr float SPAWN_DURATION = 1.5f;
    float breakingBlockFor = 0.0f;
    float stunnedFor = 0.0f;
    float spawningFor = 0.0f;

    Enemy() = default;
    Enemy(std::array<float, 2> pos, std::array<float, 2> velocity, Direction direction, EnemyState state,
          EnemyType type, bool ready);

    auto setDirection(Direction enemyDirection) -> void;
    auto setState(EnemyState enemyState) -> void;
    auto setType(EnemyType enemyType) -> void;
    auto setPositionToMoveTo() -> void;
    auto move(float deltaTime) -> void;

    /** Auxiliary method which generates a vector containing probabilities for the possible directions
     * @param directions: A vector containing all possible directions for the enemy to move to, usually the result of
     * the GameLevel::getInitialDirections-Method
     * @return: A vector containing a probability for each direction in the direction vector
     */
    auto getProbabilityArray(std::vector<Direction> directions) const -> std::vector<int>;

  private:
    static unsigned int nextID;
};