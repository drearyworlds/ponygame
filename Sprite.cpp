#include "Sprite.h"

namespace ParticleHomeEntertainment {
    Sprite::Sprite() {
        _CurrentFrame = 0;
        _IdleSpriteSheetWidth = 0;
        _IdleSpriteSheetHeight = 0;
        _RunningSpriteSheetWidth = 0;
        _RunningSpriteSheetHeight = 0;
        _JumpingSpriteSheetWidth = 0;
        _JumpingSpriteSheetHeight = 0;
        _Facing = SpriteFacingEnum::RIGHT;
        _State = SpriteMovementState::IDLE;
    }
}