#pragma once

#include "../Constants.h"
#include "DisplayBlock.h"
#include "ResourceManager.h"

#include <cctype>
#include <string>
#include <vector>

class GameInformation {
  public:
    std::array<float, 2> position{};
    unsigned int amountDisplayBlocks{};
    std::array<float, 4> size{};

    std::string text;
    std::vector<DisplayBlock *> displayBlocks;

    GameInformation() = default;
    GameInformation(std::array<float, 2> position, std::array<float, 4> size, int amountDisplayBlocks);

    void show(std::string displayText);
};
