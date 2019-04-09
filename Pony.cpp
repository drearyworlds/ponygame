#include "Pony.h"
#include "Constants.h"

using namespace ParticleHomeEntertainment;

Pony::Pony() {
}

Pony::~Pony() {
}

void Pony::UpdateState(const DirectX::Keyboard::State keyboardState) {
    bool facingChanged = false;

    // Handle Animation State
    if (keyboardState.Space) {
        if (_State != SpriteMovementStateEnum::JUMPING) {
            // Give sprite some velocity
            _Velocity.y = PONY_JUMP_Y_VELOCITY;
        }

        _State = SpriteMovementStateEnum::JUMPING;
    } else if (keyboardState.Left || keyboardState.Right) {
        _State = SpriteMovementStateEnum::RUNNING;
    } else {
        _State = SpriteMovementStateEnum::IDLE;
    }

    // Handle X-Axis movement
    if (keyboardState.Left) {
        facingChanged = (_Facing == SpriteFacingStateEnum::RIGHT);
        _Facing = SpriteFacingStateEnum::LEFT;

        if (!facingChanged) {
            _Location.x -= PONY_X_SPEED_PX;
        }
    } else if (keyboardState.Right) {
        facingChanged = (_Facing == SpriteFacingStateEnum::LEFT);
        _Facing = SpriteFacingStateEnum::RIGHT;

        if (!facingChanged) {
            _Location.x += PONY_X_SPEED_PX;
        }
    }

    // Adjust sprite Y-velocity for gravity
    _Velocity.y += GRAVITY;

    // Apply velocity 
    _Location.x += _Velocity.x;
    _Location.y += _Velocity.y;

    // But don't let her go below the grass
    if (_Location.y > (7 * SPRITE_SIZE_HEIGHT_PX)) {
        _Location.y = 7 * SPRITE_SIZE_HEIGHT_PX;
    }

    if (_Location.x < 0) {
        _Location.x = 0;
    } else if (_Location.x > SCREEN_WIDTH_PX - SPRITE_SIZE_WIDTH_PX) {
        _Location.x = SCREEN_WIDTH_PX - SPRITE_SIZE_WIDTH_PX;
    }
}