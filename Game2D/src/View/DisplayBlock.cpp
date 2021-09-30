#include "DisplayBlock.h"

DisplayBlock::DisplayBlock(std::array<float, 2> position, std::array<float, 4> size, Texture2D sprite) :
    position{position}, size{size}, sprite{sprite} {}
