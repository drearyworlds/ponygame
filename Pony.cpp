#include "Pony.h"
#include "Constants.h"

using namespace ParticleHomeEntertainment;

Pony::Pony() {}

Pony::~Pony() {}

void Pony::UpdateStates(const DirectX::Keyboard::State& keyboardState, const DirectX::Keyboard::KeyboardStateTracker& keyboardStateTracker) {
    bool facingChanged = false;
    auto projectedLocation = _Location;
    auto projectedVelocity = _Velocity;

    // If she is on the ground
    if (_SpecialState == SpriteSpecialStateEnum::ON_GROUND) {
        if (keyboardStateTracker.IsKeyPressed(DirectX::Keyboard::Keys::Space)) {
            // Give pony a burst of velocity
            projectedVelocity.y = PONY_JUMP_Y_VELOCITY;

            // Set her states
            _AnimationState = SpriteAnimationStateEnum::JUMPING;
            _SpecialState = SpriteSpecialStateEnum::IN_AIR;
        } else if (keyboardState.Left || keyboardState.Right) {
            _AnimationState = SpriteAnimationStateEnum::RUNNING;
        } else {
            _AnimationState = SpriteAnimationStateEnum::IDLE;
        }
    } else {
        // Keep existing state while in the air
    }

    // Handle X-Axis movement
    if (keyboardState.Left) {
        facingChanged = (_Facing == SpriteFacingStateEnum::RIGHT);
        _Facing = SpriteFacingStateEnum::LEFT;

        if (!facingChanged) {
            projectedLocation.x -= PONY_X_SPEED_PX;
        }
    } else if (keyboardState.Right) {
        facingChanged = (_Facing == SpriteFacingStateEnum::LEFT);
        _Facing = SpriteFacingStateEnum::RIGHT;

        if (!facingChanged) {
            projectedLocation.x += PONY_X_SPEED_PX;
        }
    }

    // Adjust sprite Y-velocity for gravity
    projectedVelocity.y += GRAVITY;

    // Apply velocity 
    projectedLocation.x += projectedVelocity.x;
    projectedLocation.y += projectedVelocity.y;

    // TODO: Below is temporary until background tile collision detection is implemented...
    
    // But don't let her go below the grass
    if (projectedLocation.y > (7 * SPRITE_HEIGHT_PX)) {
        projectedLocation.y = 7 * SPRITE_HEIGHT_PX;
        projectedVelocity.y = 0;
        _SpecialState = SpriteSpecialStateEnum::ON_GROUND;
    }

    if (projectedLocation.x < 0) {
        projectedLocation.x = 0;
    } else if (projectedLocation.x > SCREEN_WIDTH_PX - SPRITE_WIDTH_PX) {
        projectedLocation.x = SCREEN_WIDTH_PX - SPRITE_WIDTH_PX;
        projectedVelocity.x = 0;
    }

    _Location = projectedLocation;
    _Velocity = projectedVelocity;
}