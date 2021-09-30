#pragma once
#include <xaudio2.h>

struct AudioData {
    WAVEFORMATEXTENSIBLE wfx = {0};
    XAUDIO2_BUFFER buffer = {0};
};
