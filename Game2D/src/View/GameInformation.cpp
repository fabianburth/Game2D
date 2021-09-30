#include "GameInformation.h"

GameInformation::GameInformation(std::array<float, 2> position, std::array<float, 4> size, int amountDisplayBlocks) :
    position{position}, amountDisplayBlocks{static_cast<unsigned int>(amountDisplayBlocks)}, size{size} {
    for (int i = 0; i < amountDisplayBlocks; ++i) {
        displayBlocks.emplace_back(
                new DisplayBlock({position[0] - static_cast<float>(i) * (0.5f * Constants::WIDTH_UNIT), position[1]},
                                 size, ResourceManager::GetTexture("placeholder")));
    }
}

void GameInformation::show(std::string displayText) {
    if (displayText.empty() || displayText.length() > amountDisplayBlocks) {
        return;
    }

    for (unsigned int i = 0; i < displayText.length(); ++i) {
        std::string c{displayText.at(i)};
        if (displayText == "1P" && c == "1") {
            c = "One";
        }
        displayBlocks.at(displayText.length() - 1 - i)->sprite = ResourceManager::GetTexture(c);
    }
}
