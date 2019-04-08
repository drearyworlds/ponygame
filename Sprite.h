#pragma once

#include <nowarn/SimpleMath.h>
#include <nowarn/stdint.h>

namespace ParticleHomeEntertainment {
    enum SpriteFacingEnum {
        LEFT, RIGHT
    };

    enum SpriteMovementState {
        IDLE, RUNNING, JUMPING
    };

    class Sprite {
    public:
        uint8_t _CurrentFrame;
        uint32_t _IdleSpriteSheetWidth;
        uint32_t _IdleSpriteSheetHeight;
        uint32_t _RunningSpriteSheetWidth;
        uint32_t _RunningSpriteSheetHeight;
        uint32_t _JumpingSpriteSheetWidth;
        uint32_t _JumpingSpriteSheetHeight;

        DirectX::SimpleMath::Vector2 _Location;
        DirectX::SimpleMath::Vector2 _Velocity;
        SpriteFacingEnum _Facing;
        SpriteMovementState _State;

        Sprite();

        ~Sprite() = default;
    };
}