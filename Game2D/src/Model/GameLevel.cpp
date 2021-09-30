#include "GameLevel.h"

#include <algorithm>
#include <fstream>
#include <sstream>

auto GameLevel::Load(const char *file) -> void {
    // clear old data
    this->Blocks.clear();
    // load from file
    unsigned int tileCode = 0;// (=number of block, e.g. 1 for ice block)
    GameLevel level;
    std::string line;
    std::ifstream fstream(file);
    std::vector<std::vector<unsigned int>> tileData;
    if (fstream) {
        while (std::getline(fstream, line))// read each line from level file
        {
            std::istringstream sstream(line);
            std::vector<unsigned int> row;
            while (sstream >> tileCode) {// read each word separated by spaces
                row.push_back(tileCode);
            }
            tileData.push_back(row);
        }
        if (!tileData.empty()) {
            this->init(tileData);
        }
    }
}

auto GameLevel::IsCompleted() -> bool {
    if (this->Enemies.empty()) {
        double levelDuration = (static_cast<double>(std::clock() - startClockLevel) / (double) CLOCKS_PER_SEC);
        this->score.addLevelCompletion(levelDuration);

        return true;
    }
    return false;
}

auto GameLevel::init(std::vector<std::vector<unsigned int>> tileData) -> void {
    // starts clocks to track duration to finish the level and the duration between killing enemies
    startClockLevel = std::clock();
    startClockEnemyKill = std::clock();
    // calculate dimensions
    std::reverse(tileData.begin(), tileData.end());
    unsigned int height = tileData.size();
    unsigned int width =
            tileData[0].size();// note we can index vector at [0] since this function is only called if height > 0
    LeftWall = Wall(WallSide::LEFT);
    RightWall = Wall(WallSide::RIGHT);
    BottomWall = Wall(WallSide::BOTTOM);
    TopWall = Wall(WallSide::TOP);

    // initialize level wall (same for every level)
    // Left and Right Wall
    for (unsigned int x = 0; x < height + 1; ++x) {
        // Left Wall
        std::array<float, 2> posL = {-0.5f * Constants::WIDTH_UNIT,
                                     -0.5f * Constants::HEIGHT_UNIT + Constants::HEIGHT_UNIT * static_cast<float>(x)};
        std::array<float, 4> sizeL = {0.5f, 1.0f, 0.5f * (-1 + 0.5f * Constants::WIDTH_UNIT), 0};
        GameObject objL(posL, sizeL);
        // objL.isUnbreakable = true;
        this->Walls.push_back(objL);
        this->LeftWall.addWallComponent(objL);
        // Right Wall
        std::array<float, 2> posR = {static_cast<float>(width) * Constants::WIDTH_UNIT,
                                     -0.5f * Constants::HEIGHT_UNIT + Constants::HEIGHT_UNIT * static_cast<float>(x)};
        std::array<float, 4> sizeR = {0.5f, 1.0f, 0.5f * (-1 + 0.5f * Constants::WIDTH_UNIT), 0};
        GameObject objR(posR, sizeR);
        // objR.isUnbreakable = true;
        this->Walls.push_back(objR);
        this->RightWall.addWallComponent(objR);
    }
    // Bottom and Top Wall
    for (unsigned int x = 0; x < width; ++x) {
        // Bottom Wall
        std::array<float, 2> posB = {Constants::WIDTH_UNIT * static_cast<float>(x),
                                     static_cast<float>(height) * Constants::HEIGHT_UNIT};
        std::array<float, 4> sizeB = {1.0f, 0.5f, 0, 0.5f * (-1 + 0.5f * Constants::HEIGHT_UNIT)};
        GameObject objB(posB, sizeB);
        // objB.isUnbreakable = true;
        this->Walls.push_back(objB);
        this->TopWall.addWallComponent(objB);
        // Top Wall
        std::array<float, 2> posT = {Constants::WIDTH_UNIT * static_cast<float>(x), -0.5f * Constants::HEIGHT_UNIT};
        std::array<float, 4> sizeT = {1.0f, 0.5f, 0, 0.5f * (-1 + 0.5f * Constants::HEIGHT_UNIT)};
        GameObject objT(posT, sizeT);
        // objT.isUnbreakable = true;
        this->Walls.push_back(objT);
        this->BottomWall.addWallComponent(objT);
    }

    // initialize level tiles based on tileData
    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
            // check block type from level data (2D level array)
            if (tileData[y][x] == 1)// breakable iceblock
            {
                std::array<float, 2> pos = {Constants::WIDTH_UNIT * static_cast<float>(x),
                                            Constants::HEIGHT_UNIT * static_cast<float>(y)};
                this->Blocks.emplace_back(pos, false, nullptr, BlockState::SOLID);
            } else if (tileData[y][x] == 2)// unbreakable diamond block
            {
                std::array<float, 2> pos = {Constants::WIDTH_UNIT * static_cast<float>(x),
                                            Constants::HEIGHT_UNIT * static_cast<float>(y)};
                this->Blocks.emplace_back(pos, true, nullptr, BlockState::SOLID);
            } else if (tileData[y][x] == 3)// breakable iceblock with enemy frozen in it
            {
                std::array<float, 2> pos = {Constants::WIDTH_UNIT * static_cast<float>(x),
                                            Constants::HEIGHT_UNIT * static_cast<float>(y)};
                std::array<float, 2> velocity = {Constants::WIDTH_UNIT * 3, Constants::HEIGHT_UNIT * 3};
                Direction direction = Direction::DOWN;
                std::shared_ptr<Enemy> enemy = std::make_shared<Enemy>(pos, velocity, direction, EnemyState::NONE,
                                                                       EnemyType::WANDERING, false);
                this->frozenEnemies.push_back(enemy);


                this->Blocks.emplace_back(pos, false, enemy, BlockState::SOLID);
            } else if (tileData[y][x] == 4)// pengo
            {
                std::array<float, 2> pos = {Constants::WIDTH_UNIT * static_cast<float>(x),
                                            Constants::HEIGHT_UNIT * static_cast<float>(y)};
                std::array<float, 2> velocity = {Constants::WIDTH_UNIT * 3, Constants::HEIGHT_UNIT * 3};
                this->Pengo = Player(pos, velocity);
            } else if (tileData[y][x] == 5)// walking enemy
            {
                std::array<float, 2> pos = {Constants::WIDTH_UNIT * static_cast<float>(x),
                                            Constants::HEIGHT_UNIT * static_cast<float>(y)};
                std::array<float, 2> velocity = {Constants::WIDTH_UNIT * 3, Constants::HEIGHT_UNIT * 3};
                Direction direction = Direction::DOWN;
                std::shared_ptr<Enemy> enemy = std::make_shared<Enemy>(pos, velocity, direction, EnemyState::NONE,
                                                                       EnemyType::WANDERING, false);
                this->Enemies.push_back(enemy);
            } else if (tileData[y][x] == 6)// boxing enemy
            {
                std::array<float, 2> pos = {Constants::WIDTH_UNIT * static_cast<float>(x),
                                            Constants::HEIGHT_UNIT * static_cast<float>(y)};
                std::array<float, 2> velocity = {Constants::WIDTH_UNIT * 3, Constants::HEIGHT_UNIT * 3};
                Direction direction = Direction::DOWN;
                std::shared_ptr<Enemy> enemy =
                        std::make_shared<Enemy>(pos, velocity, direction, EnemyState::NONE, EnemyType::CHASING, false);
                this->Enemies.push_back(enemy);
            }
        }
    }
}

auto GameLevel::checkCollisions(GameObject &gameObject, Direction d) -> bool {
    for (Block &block : this->Blocks) {
        if (block.state != BlockState::BROKEN) {
            if (checkBlockCollision(gameObject, block, d) || checkWallCollision(gameObject, d)) {
                return true;
            }
        }
    }
    return false;
}

auto GameLevel::checkBlockCollision(GameObject &one, GameObject &two, Direction d) -> bool {
    bool collisionX = false;
    bool collisionY = false;

    switch (d) {
        case Direction::RIGHT:
            collisionX = ((one.position[0] + 2 * Constants::WIDTH_UNIT) - two.position[0]) > Constants::EPSILON &&
                         ((two.position[0] + Constants::WIDTH_UNIT) - (one.position[0] + Constants::WIDTH_UNIT)) >
                                 Constants::EPSILON;
            collisionY = ((one.position[1] + Constants::HEIGHT_UNIT) - two.position[1]) > Constants::EPSILON &&
                         ((two.position[1] + Constants::HEIGHT_UNIT) - one.position[1]) > Constants::EPSILON;
            break;
        case Direction::LEFT:
            collisionX = ((one.position[0] + Constants::WIDTH_UNIT - Constants::WIDTH_UNIT) - two.position[0]) >
                                 Constants::EPSILON &&
                         ((two.position[0] + Constants::WIDTH_UNIT) - (one.position[0] - Constants::WIDTH_UNIT)) >
                                 Constants::EPSILON;
            collisionY = ((one.position[1] + Constants::HEIGHT_UNIT) - two.position[1]) > Constants::EPSILON &&
                         ((two.position[1] + Constants::HEIGHT_UNIT) - one.position[1]) > Constants::EPSILON;
            break;
        case Direction::UP:
            collisionX = ((one.position[0] + Constants::WIDTH_UNIT) - two.position[0]) > Constants::EPSILON &&
                         ((two.position[0] + Constants::WIDTH_UNIT) - one.position[0]) > Constants::EPSILON;
            collisionY = ((one.position[1] + 2 * Constants::HEIGHT_UNIT) - two.position[1]) > Constants::EPSILON &&
                         ((two.position[1] + Constants::HEIGHT_UNIT) - (one.position[1] + Constants::HEIGHT_UNIT)) >
                                 Constants::EPSILON;
            break;
        case Direction::DOWN:
            collisionX = ((one.position[0] + Constants::WIDTH_UNIT) - two.position[0]) > Constants::EPSILON &&
                         ((two.position[0] + Constants::WIDTH_UNIT) - one.position[0]) > Constants::EPSILON;
            collisionY = ((one.position[1] + Constants::HEIGHT_UNIT - Constants::HEIGHT_UNIT) - two.position[1]) >
                                 Constants::EPSILON &&
                         ((two.position[1] + Constants::HEIGHT_UNIT) - (one.position[1] - Constants::HEIGHT_UNIT)) >
                                 Constants::EPSILON;
            break;
        default:
            break;
    }
    return collisionX && collisionY;
}

auto GameLevel::checkWallCollision(GameObject &one, Direction d) -> bool {
    bool collision = false;

    switch (d) {
        case Direction::RIGHT:
            // collision = !((-1.0f + 0.5f * Constants::WIDTH_UNIT) + one.position[0] + Constants::WIDTH_UNIT < 1.0f -
            // 0.5f * Constants::WIDTH_UNIT );
            collision =
                    ((one.position[0] + Constants::WIDTH_UNIT) - (12 * Constants::WIDTH_UNIT)) >= Constants::EPSILON;
            break;
        case Direction::LEFT:
            // collision = !((-1.0f + 0.5f * Constants::WIDTH_UNIT) + one.position[0] - Constants::WIDTH_UNIT >= -1.0f +
            // 0.5f * Constants::WIDTH_UNIT);
            collision = 0 - (one.position[0] - Constants::WIDTH_UNIT) >= Constants::EPSILON;
            break;
        case Direction::UP:
            // collision = !((-1.0f + 0.5f * Constants::HEIGHT_UNIT) + one.position[1] + Constants::HEIGHT_UNIT < 1.0f -
            // 2 * Constants::HEIGHT_UNIT - 0.5f * Constants::HEIGHT_UNIT);
            collision =
                    ((one.position[1] + Constants::HEIGHT_UNIT) - (14 * Constants::HEIGHT_UNIT)) >= Constants::EPSILON;
            break;
        case Direction::DOWN:
            // collision = !((-1.0f + 0.5f * Constants::HEIGHT_UNIT) + one.position[1] - Constants::HEIGHT_UNIT >=
            // (-1.0f + 0.5f * Constants::HEIGHT_UNIT));
            collision = 0 - (one.position[1] - Constants::HEIGHT_UNIT) >= Constants::EPSILON;
            break;
        default:
            break;
    }
    return collision;
}

auto GameLevel::getCollisionBlock(GameObject &gameObject, Direction d) -> Block * {
    for (Block &block : this->Blocks) {
        if (block.state != BlockState::BROKEN) {
            if (checkBlockCollision(gameObject, block, d)) {
                return &block;
            }
        }
    }
    return nullptr;
}

auto GameLevel::calculateStepRange(Block &block, Direction d) -> int {
    int stepWidth = 0;
    std::array<float, 2> originalPosition = block.position;

    while (!checkCollisions(block, d)) {
        block.setPositionToMoveTo(d);
        block.position = block.positionToMoveTo;
        stepWidth++;
    }

    block.position = originalPosition;
    block.positionToMoveTo = block.position;
    return stepWidth;
}

auto GameLevel::checkWallCollisionPrecise(GameObject &one, Direction d) -> bool {
    bool collision = false;

    switch (d) {
        case Direction::RIGHT:
            // collision = !((-1.0f + 0.5f * Constants::WIDTH_UNIT) + one.position[0] + Constants::WIDTH_UNIT < 1.0f -
            // 0.5f * Constants::WIDTH_UNIT );
            collision = ((12 * Constants::WIDTH_UNIT) - one.position[0]) <= Constants::EPSILON;
            break;
        case Direction::LEFT:
            // collision = !((-1.0f + 0.5f * Constants::WIDTH_UNIT) + one.position[0] - Constants::WIDTH_UNIT >= -1.0f +
            // 0.5f * Constants::WIDTH_UNIT);
            collision = 0 + one.position[0] <= Constants::EPSILON;
            break;
        case Direction::UP:
            // collision = !((-1.0f + 0.5f * Constants::HEIGHT_UNIT) + one.position[1] + Constants::HEIGHT_UNIT < 1.0f -
            // 2 * Constants::HEIGHT_UNIT - 0.5f * Constants::HEIGHT_UNIT);
            collision = ((14 * Constants::HEIGHT_UNIT) - one.position[1]) <= Constants::EPSILON;
            break;
        case Direction::DOWN:
            // collision = !((-1.0f + 0.5f * Constants::HEIGHT_UNIT) + one.position[1] - Constants::HEIGHT_UNIT >=
            // (-1.0f + 0.5f * Constants::HEIGHT_UNIT));
            collision = 0 + one.position[1] <= Constants::EPSILON;
            break;
        default:
            break;
    }
    return collision;
}

auto GameLevel::checkCollisionPrecise(GameObject &one, GameObject &two) -> bool {
    bool collisionX = one.position[0] + Constants::WIDTH_UNIT - two.position[0] > Constants::EPSILON &&
                      two.position[0] + Constants::WIDTH_UNIT - one.position[0] > Constants::EPSILON;
    bool collisionY = one.position[1] + Constants::HEIGHT_UNIT - two.position[1] > Constants::EPSILON &&
                      two.position[1] + Constants::HEIGHT_UNIT - one.position[1] > Constants::EPSILON;
    return collisionX && collisionY;
}


// ----------------------------------------------------------------------------
// Methods to determine enemy behavior
// ----------------------------------------------------------------------------
auto GameLevel::getInitialDirections(Enemy &enemy) -> std::vector<Direction> {

    // Build array for determining a direction with certain propabilities
    std::vector<Direction> allDirections{Direction::RIGHT, Direction::LEFT, Direction::UP, Direction::DOWN};
    std::vector<Direction> orderedDirections;
    std::vector<Direction> directions;

    float xDistance = std::abs(enemy.position[0] - Pengo.position[0]) * (2.0f / 448.0f);
    float yDistance = std::abs(enemy.position[1] - Pengo.position[1]) * (2.0f / 576.0f);

    switch (enemy.type) {
        case (EnemyType::WANDERING):
            orderedDirections.push_back(enemy.direction);
            for (Direction d : allDirections) {
                if (d != enemy.direction && d != oppositeDirection(enemy.direction)) {
                    orderedDirections.push_back(d);
                }
            }
            orderedDirections.push_back(oppositeDirection(enemy.direction));

            for (auto &orderedDirection : orderedDirections) {
                if (!checkCollisions(enemy, orderedDirection)) {
                    directions.push_back(orderedDirection);
                }
            }
            break;
        case (EnemyType::CHASING):

            if (xDistance >= yDistance) {
                // Move horizontally (x-direction)
                if (enemy.position[0] > Pengo.position[0]) {
                    // Pengo is left, so move left
                    orderedDirections.push_back(Direction::LEFT);
                } else {
                    orderedDirections.push_back(Direction::RIGHT);
                }

                if (enemy.position[1] > Pengo.position[1]) {
                    // Pengo is left, so move left
                    orderedDirections.push_back(Direction::DOWN);
                } else {
                    orderedDirections.push_back(Direction::UP);
                }
            } else if (xDistance < yDistance) {
                // Move vertically (y-direction)
                if (enemy.position[1] > Pengo.position[1]) {
                    // Pengo is left, so move left
                    orderedDirections.push_back(Direction::DOWN);
                } else {
                    orderedDirections.push_back(Direction::UP);
                }

                if (enemy.position[0] > Pengo.position[0]) {
                    // Pengo is left, so move left
                    orderedDirections.push_back(Direction::LEFT);
                } else {
                    orderedDirections.push_back(Direction::RIGHT);
                }
            }

            for (Direction d : allDirections) {
                if (std::find(orderedDirections.begin(), orderedDirections.end(), d) == orderedDirections.end()) {
                    orderedDirections.push_back(d);
                }
            }

            for (auto &orderedDirection : orderedDirections) {
                if (!checkWallCollision(enemy, orderedDirection)) {
                    Block *block = getCollisionBlock(enemy, orderedDirection);
                    if (block == nullptr || !block->isUnbreakable) {
                        directions.push_back(orderedDirection);
                    }
                }
            }
            break;
    }

    return directions;
}

auto GameLevel::getDirectionIndex(const std::vector<int> &chances) -> int {
    int index = 0;
    int randomNumber = std::rand() % 100;
    for (int chance : chances) {
        if (randomNumber > chance) {
            index++;
        }
    }
    return index;
}
// ----------------------------------------------------------------------------


auto GameLevel::checkThreeDiamonds() -> void {

    if (this->diamondBlocksAligned) {
        return;
    }
    bool bTouchesWall = false;
    bool firstAdjacentBlockTouchesWall = false;
    bool secondAdjacentBlockTouchesWall = false;

    for (Block &b : this->Blocks) {
        if (b.isUnbreakable) {
            bTouchesWall = blockTouchesWall(b);
            Block *firstAdjacentBlock = getCollisionBlock(b, Direction::RIGHT);
            if (firstAdjacentBlock != nullptr && firstAdjacentBlock->isUnbreakable) {
                Block fab = *firstAdjacentBlock;
                firstAdjacentBlockTouchesWall = blockTouchesWall(fab);
                Block *secondAdjacentBlock = getCollisionBlock(fab, Direction::RIGHT);
                if (secondAdjacentBlock != nullptr && secondAdjacentBlock->isUnbreakable) {
                    Block sab = *secondAdjacentBlock;
                    secondAdjacentBlockTouchesWall = blockTouchesWall(sab);
                    if (bTouchesWall || firstAdjacentBlockTouchesWall || secondAdjacentBlockTouchesWall) {
                        this->score.addDiamondsTouchingWall();
                    } else {
                        this->score.addDiamondsNotTouchingWall();
                    }
                    this->setEvent(Events::DIAMONDS_ALIGNED);
                    this->diamondBlocksAligned = true;
                    return;
                }
            }
            // bTouchesWall = checkWallCollision(b, Direction::LEFT);
            firstAdjacentBlock = getCollisionBlock(b, Direction::LEFT);
            if (firstAdjacentBlock != nullptr && firstAdjacentBlock->isUnbreakable) {
                Block fab = *firstAdjacentBlock;
                firstAdjacentBlockTouchesWall = blockTouchesWall(fab);
                Block *secondAdjacentBlock = getCollisionBlock(fab, Direction::LEFT);
                if (secondAdjacentBlock != nullptr && secondAdjacentBlock->isUnbreakable) {
                    Block sab = *secondAdjacentBlock;
                    secondAdjacentBlockTouchesWall = blockTouchesWall(sab);
                    if (bTouchesWall || firstAdjacentBlockTouchesWall || secondAdjacentBlockTouchesWall) {
                        this->score.addDiamondsTouchingWall();
                    } else {
                        this->score.addDiamondsNotTouchingWall();
                    }
                    this->setEvent(Events::DIAMONDS_ALIGNED);
                    this->diamondBlocksAligned = true;
                    return;
                }
            }
            // bTouchesWall = checkWallCollision(b, Direction::UP);
            firstAdjacentBlock = getCollisionBlock(b, Direction::UP);
            if (firstAdjacentBlock != nullptr && firstAdjacentBlock->isUnbreakable) {
                Block fab = *firstAdjacentBlock;
                firstAdjacentBlockTouchesWall = blockTouchesWall(fab);
                Block *secondAdjacentBlock = getCollisionBlock(fab, Direction::UP);
                if (secondAdjacentBlock != nullptr && secondAdjacentBlock->isUnbreakable) {
                    Block sab = *secondAdjacentBlock;
                    secondAdjacentBlockTouchesWall = blockTouchesWall(sab);
                    if (bTouchesWall || firstAdjacentBlockTouchesWall || secondAdjacentBlockTouchesWall) {
                        this->score.addDiamondsTouchingWall();
                    } else {
                        this->score.addDiamondsNotTouchingWall();
                    }
                    this->setEvent(Events::DIAMONDS_ALIGNED);
                    this->diamondBlocksAligned = true;
                    return;
                }
            }
            // bTouchesWall = checkWallCollision(b, Direction::DOWN);
            firstAdjacentBlock = getCollisionBlock(b, Direction::DOWN);
            if (firstAdjacentBlock != nullptr && firstAdjacentBlock->isUnbreakable) {
                Block fab = *firstAdjacentBlock;
                firstAdjacentBlockTouchesWall = blockTouchesWall(fab);
                Block *secondAdjacentBlock = getCollisionBlock(fab, Direction::DOWN);
                if (secondAdjacentBlock != nullptr && secondAdjacentBlock->isUnbreakable) {
                    Block sab = *secondAdjacentBlock;
                    secondAdjacentBlockTouchesWall = blockTouchesWall(sab);
                    if (bTouchesWall || firstAdjacentBlockTouchesWall || secondAdjacentBlockTouchesWall) {
                        this->score.addDiamondsTouchingWall();
                    } else {
                        this->score.addDiamondsNotTouchingWall();
                    }
                    this->setEvent(Events::DIAMONDS_ALIGNED);
                    this->diamondBlocksAligned = true;
                    return;
                }
            }
        }
    }
}

auto GameLevel::blockTouchesWall(Block &b) -> bool {
    return checkWallCollision(b, Direction::RIGHT) || checkWallCollision(b, Direction::LEFT) ||
           checkWallCollision(b, Direction::UP) || checkWallCollision(b, Direction::DOWN);
}

auto GameLevel::killEnemy(std::shared_ptr<Enemy> &enemy) -> void {
    this->Enemies.erase(std::remove_if(std::begin(this->Enemies), std::end(this->Enemies),
                                       [&](const std::shared_ptr<Enemy> &comparison) { return comparison == enemy; }),
                        std::end(this->Enemies));

    this->setEvent(Events::ENEMY_KILLED);
    startClockEnemyKill = std::clock();
}

auto GameLevel::spawnEnemy() -> void {
    bool validEnemy = false;
    std::shared_ptr<Enemy> enemy;
    while (!validEnemy) {
        enemy = this->frozenEnemies.back();
        this->frozenEnemies.pop_back();
        for (const Block &b : this->Blocks) {
            if (b.position == enemy->position && b.state != BlockState::BROKEN && b.state != BlockState::BREAKING) {
                validEnemy = true;
            }
        }
        if (this->frozenEnemies.empty() && !validEnemy) {
            return;
        }
    }
    this->Enemies.push_back(enemy);
    this->setEvent(Events::ENEMY_SPAWNED);
    for (Block &block : this->Blocks) {
        // if (std::abs(block.position[0] - enemy->position[0]) < EPSILON && std::abs(block.position[1] -
        // enemy->position[1]) < EPSILON)
        if (block.containedEnemy == enemy) {
            block.setState(BlockState::BROKEN);
        }
    }

    enemy->setState(EnemyState::SPAWNING);

    for (Block &b : this->Blocks) {
        if ((b.state != BlockState::BROKEN && b.state != BlockState::BREAKING) && b.containedEnemy != nullptr) {
            b.setState(BlockState::FLASHING);
        }
    }
}

auto GameLevel::boxerExists() -> bool {
    return std::any_of(std::begin(this->Enemies), std::end(this->Enemies),
                       [&](const std::shared_ptr<Enemy> &e) { return e->type == EnemyType::CHASING; });
}

auto GameLevel::updateGameState(float dt) -> void {
    int pengoLives = this->Pengo.lives;

    for (Block &b : this->Blocks) {
        switch (b.state) {
            case (BlockState::BREAKING):
                if (b.breakingFor >= Block::BREAKING_DURATION) {
                    b.setState(BlockState::BROKEN);
                    b.breakingFor = 0.0f;
                }
                b.breakingFor += dt;
                break;
            case (BlockState::FLASHING):
                if (b.flashingFor >= Block::FLASHING_DURATION) {
                    b.setState(BlockState::SOLID);
                    b.flashingFor = 0.0f;
                }
                b.flashingFor += dt;
                break;
            default:
                break;
        }
    }

    for (std::shared_ptr<Enemy> &e : this->Enemies) {
        switch (e->state) {
            case (EnemyState::BREAKING):
                if (e->breakingBlockFor >= Enemy::BREAKING_BLOCK_DURATION) {
                    e->setState(EnemyState::NONE);
                    e->setPositionToMoveTo();
                    e->breakingBlockFor = 0.0f;
                }
                e->breakingBlockFor += dt;
                break;
            case (EnemyState::STUNNED):
                if (e->stunnedFor >= Enemy::STUNN_DURATION) {
                    e->setState(EnemyState::NONE);
                    e->ready = true;
                    e->stunnedFor = 0.0f;
                }
                e->stunnedFor += dt;
                break;
            case (EnemyState::INITIAL_SPAWNING):
            case (EnemyState::SPAWNING):
                if (e->spawningFor >= Enemy::SPAWN_DURATION) {
                    e->setState(EnemyState::NONE);
                    e->ready = true;
                    e->spawningFor = 0.0f;
                }
                e->spawningFor += dt;
                break;
            default:
                break;
        }
    }

    switch (this->Pengo.state) {
        case PengoState::WALK:
            if (Pengo.walkingFor >= Player::WALKING_DURATION) {
                Pengo.setState(PengoState::STAND);
                Pengo.walkingFor = 0.0f;
            }
            Pengo.walkingFor += dt;
            break;
        case PengoState::PUSH:
            if (Pengo.pushingFor >= Player::PUSHING_DURATION) {
                Pengo.setState(PengoState::STAND);
                Pengo.ready = true;
                Pengo.pushingFor = 0.0f;
            }
            Pengo.pushingFor += dt;
            break;
        case PengoState::BREAK:
            if (Pengo.breakingBlockFor >= Player::BREAKING_BLOCK_DURATION) {
                Pengo.setState(PengoState::STAND);
                Pengo.ready = true;
                Pengo.breakingBlockFor = 0.0f;
            }
            Pengo.breakingBlockFor += dt;
            break;
        default:
            break;
    }

    if (this->TopWall.state == WallState::WOBBLY) {
        if (TopWall.wobblyFor >= Wall::WOBBLY_DURATION) {
            TopWall.setState(WallState::SOLID);
            TopWall.wobblyFor = 0.0f;
        }
        TopWall.wobblyFor += dt;
    }
    if (this->BottomWall.state == WallState::WOBBLY) {
        if (BottomWall.wobblyFor >= Wall::WOBBLY_DURATION) {
            BottomWall.setState(WallState::SOLID);
            BottomWall.wobblyFor = 0.0f;
        }
        BottomWall.wobblyFor += dt;
    }
    if (this->LeftWall.state == WallState::WOBBLY) {
        if (LeftWall.wobblyFor >= Wall::WOBBLY_DURATION) {
            LeftWall.setState(WallState::SOLID);
            LeftWall.wobblyFor = 0.0f;
        }
        LeftWall.wobblyFor += dt;
    }
    if (this->RightWall.state == WallState::WOBBLY) {
        if (RightWall.wobblyFor >= Wall::WOBBLY_DURATION) {
            RightWall.setState(WallState::SOLID);
            RightWall.wobblyFor = 0.0f;
        }
        RightWall.wobblyFor += dt;
    }


    this->checkThreeDiamonds();

    if (!this->boxerExists()) {
        this->trySettingBoxer();
    }

    if (static_cast<float>((std::clock() - startClockEnemyKill) / (double) CLOCKS_PER_SEC) > 60) {
        this->trySettingBoxer();
        this->setEvent(Events::SUDDEN_DEATH);
    }

    this->determineBotBehavior(dt);
    if (pengoLives > Pengo.lives) {
        this->setEvent(Events::PENGO_DIED);
        return;
    }

    this->updateBlockInteractions(dt);
    this->Pengo.move(dt);
}

auto GameLevel::registerObserver(Observer<GameLevel> *o) -> void {
    observers.push_back(o);
}

auto GameLevel::removeObserver(Observer<GameLevel> *o) -> void {
    observers.erase(std::remove_if(observers.begin(), observers.end(),
                                   [&](Observer<GameLevel> *comparison) { return comparison == o; }),
                    observers.end());
}

auto GameLevel::notifyObservers() -> void {
    for (Observer<GameLevel> *o : observers) {
        o->update(this);
    }
}

auto GameLevel::setEvent(Events e) -> void {
    if (this->event != e) {
        this->event = e;
    }
    this->notifyObservers();
}

auto GameLevel::trySettingBoxer() -> void {
    for (std::shared_ptr<Enemy> &e : this->Enemies) {
        if (e->type == EnemyType::WANDERING) {
            e->setType(EnemyType::CHASING);
            return;
        }
    }
}

auto GameLevel::determineBotBehavior(float dt) -> void {
    // bots do stuff
    for (std::shared_ptr<Enemy> &enemy : this->Enemies) {
        if (enemy == nullptr) {
            continue;
        }
        // Behavior if enemies touch Pengo
        if (this->checkCollisionPrecise(*enemy, this->Pengo)) {
            // if enemies touch pengo while they are not stunned, pengo dies
            if (enemy->state != EnemyState::STUNNED) {
                enemy->setState(EnemyState::BREAKING);
                --this->Pengo.lives;
                return;
            }
            // if enemies touch pengo while they are stunned, the enemy dies
            this->killEnemy(enemy);
            this->setEvent(Events::STUNNED_ENEMY_KILLED);
            this->score.addWalkedOverStunnedEnemy();
            if (!this->frozenEnemies.empty()) {
                this->spawnEnemy();
            }
        }

        // if enemies touch a wall that is wobbly, the enemies shall be stunned
        if ((this->RightWall.state == WallState::WOBBLY && checkWallCollisionPrecise(*enemy, Direction::RIGHT)) ||
            (this->LeftWall.state == WallState::WOBBLY && checkWallCollisionPrecise(*enemy, Direction::LEFT)) ||
            (this->TopWall.state == WallState::WOBBLY && checkWallCollisionPrecise(*enemy, Direction::UP)) ||
            (this->BottomWall.state == WallState::WOBBLY && checkWallCollisionPrecise(*enemy, Direction::DOWN))) {
            enemy->setState(EnemyState::STUNNED);
            this->setEvent(Events::ENEMY_STUNNED);
        }

        if (enemy->ready) {
            std::vector<Direction> directions = getInitialDirections(*enemy);
            std::vector<int> chances = enemy->getProbabilityArray(directions);
            int index = getDirectionIndex(chances);
            // cover the case when there is no direction to go to
            // for example, if you pushed a block and it happens to be currently flying by the only direction this enemy
            // could go to or you trap the enemy
            if (directions.empty()) {
                continue;
            }

            Direction direction = directions.at(index);

            enemy->setDirection(direction);
            if (enemy->type == EnemyType::CHASING) {
                Block *block = getCollisionBlock(*enemy, direction);
                if (block != nullptr) {
                    block->setState(BlockState::BREAKING);
                    enemy->setState(EnemyState::BREAKING);
                } else {
                    enemy->setPositionToMoveTo();
                }
            } else {
                enemy->setPositionToMoveTo();
            }
        }
        enemy->move(dt);
    }
}

auto GameLevel::updateBlockInteractions(float dt) -> void {
    for (Block &block : this->Blocks) {
        if (block.position != block.positionToMoveTo) {
            for (std::shared_ptr<Enemy> &e : this->Enemies) {
                if (e == nullptr) {
                    continue;
                }
                if (this->checkCollisionPrecise(*e, block)) {
                    this->killEnemy(e);
                    this->setEvent(Events::ENEMY_CRUSHED);
                    this->score.addCrushedEnemy(block.killedWithOneMove);
                    ++block.killedWithOneMove;

                    if (!this->frozenEnemies.empty()) {
                        this->spawnEnemy();
                    }
                }
            }
            block.move(dt);
        }
    }
}

auto GameLevel::processPengoAttack() -> void {
    if (this->checkCollisions(Pengo, Pengo.direction)) {
        Block *block = getCollisionBlock(Pengo, Pengo.direction);
        // Behavior if the collision is with an adjacent block
        if (block != nullptr) {
            // Behavior if there actually is a block directly behind the adjacent one
            if (this->checkCollisions(*block, Pengo.direction)) {
                // Pengo always tries to break it (Pengo Animation)
                Pengo.setState(PengoState::BREAK);
                Pengo.ready = false;
                // Block only breaks if it's not solid (= not a diamond block)
                if (!block->isUnbreakable) {
                    block->setState(BlockState::BREAKING);
                    this->setEvent(Events::BREAK_ICE_BLOCK);
                    score.addDestroyBlock(block);
                }
                return;
            }
            // Behavior if there is no block directly behind the adjacent one
            Pengo.ready = false;
            Pengo.setState(PengoState::PUSH);
            block->push(Pengo.direction, calculateStepRange(*block, Pengo.direction));
            this->setEvent(Events::PUSH_BLOCK);
            return;
        }
        // Behavior if the collision is with a wall
        Pengo.setState(PengoState::BREAK);
        this->setEvent(Events::WOBBLE_WALL);
        Pengo.ready = false;

        if (Pengo.direction == Direction::UP) {
            this->TopWall.setState(WallState::WOBBLY);
        } else if (Pengo.direction == Direction::DOWN) {
            this->BottomWall.setState(WallState::WOBBLY);
        } else if (Pengo.direction == Direction::LEFT) {
            this->LeftWall.setState(WallState::WOBBLY);
        } else if (Pengo.direction == Direction::RIGHT) {
            this->RightWall.setState(WallState::WOBBLY);
        }
        return;
    }
}

auto GameLevel::processPengoMovement(Direction d) -> void {
    Pengo.setDirection(d);
    Pengo.setState(PengoState::WALK);
    if (!this->checkCollisions(Pengo, d)) {
        Pengo.setPositionToMoveTo();
    }
}

auto GameLevel::initStates() -> void {
    // Every block that contains an enemy has to indicate this at the start of the game
    for (Block &b : this->Blocks) {
        if ((b.state != BlockState::BROKEN && b.state != BlockState::BREAKING) && b.containedEnemy != nullptr) {
            b.setState(BlockState::FLASHING);
        }
    }

    // All not frozen enemies have to spawn at the start of the game
    for (std::shared_ptr<Enemy> &e : this->Enemies) {
        e->setState(EnemyState::INITIAL_SPAWNING);
    }
}
