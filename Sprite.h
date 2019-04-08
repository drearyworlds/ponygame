#pragma once

#include <stdint.h>

namespace ParticleHomeEntertainment {
    class Sprite {
    public:
        uint8_t _CurrentFrame;
        uint32_t _IdleSpriteSheetWidth;
        uint32_t _IdleSpriteSheetHeight;
        uint32_t _RunningSpriteSheetWidth;
        uint32_t _RunningSpriteSheetHeight;
        uint32_t _JumpingSpriteSheetWidth;
        uint32_t _JumpingSpriteSheetHeight;

        Sprite() {
            _CurrentFrame = 0;
            _IdleSpriteSheetWidth = 0;
            _IdleSpriteSheetHeight = 0;
            _RunningSpriteSheetWidth = 0;
            _RunningSpriteSheetHeight = 0;
            _JumpingSpriteSheetWidth = 0;
            _JumpingSpriteSheetHeight = 0;
        }

        ~Sprite() = default;
    };
}