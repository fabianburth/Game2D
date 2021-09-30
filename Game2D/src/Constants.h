#pragma once

namespace Constants {
    // game size in pixels
    inline const float GAME_WIDTH = 448.0f;
    inline const float GAME_HEIGHT = 576.0f;

    // 2.0f because the opengl window ranges from -1 to 1
    inline const float WIDTH_UNIT = ((2.0f / GAME_WIDTH) * 32.0f);
    inline const float HEIGHT_UNIT = ((2.0f / GAME_HEIGHT) * 32.0f);

    inline const float EPSILON = 0.01;
}// namespace Constants
