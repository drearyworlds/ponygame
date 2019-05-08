#include "Pony.h"
#include "Constants.h"
#include "PonyGame.h"

using namespace ParticleHomeEntertainment;

Pony::Pony() { }

Pony::~Pony() { }

void Pony::UpdateStates(const DirectX::Keyboard::State& keyboardState, const DirectX::Keyboard::KeyboardStateTracker& keyboardStateTracker, const LevelScreen& screen) {
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
        facingChanged = (_Facing == SpriteFacingStateEnum::FACING_RIGHT);
        _Facing = SpriteFacingStateEnum::FACING_LEFT;

        if (!facingChanged) {
            projectedLocation.x -= PONY_X_SPEED_PX;
        }
    } else if (keyboardState.Right) {
        facingChanged = (_Facing == SpriteFacingStateEnum::FACING_LEFT);
        _Facing = SpriteFacingStateEnum::FACING_RIGHT;

        if (!facingChanged) {
            projectedLocation.x += PONY_X_SPEED_PX;
        }
    }

    // Adjust sprite Y-velocity for gravity
    projectedVelocity.y += GRAVITY;

    // Apply velocity 
    projectedLocation.x += projectedVelocity.x;
    projectedLocation.y += projectedVelocity.y;

    SpriteCollisionResult collisionResult = GetCollisions(projectedLocation, screen);

    // But don't let her go below the grass
    if (collisionResult._Bottom) {
        projectedLocation.y = _Location.y;
        projectedVelocity.y = 0;
        _SpecialState = SpriteSpecialStateEnum::ON_GROUND;
    }
    
    if (collisionResult._Top) {
        projectedLocation.y = _Location.y;
        projectedVelocity.y = 0;
    }

    if (collisionResult._Left) {
        projectedLocation.x = _Location.x;
        projectedVelocity.x = 0;
    }
    
    if (collisionResult._Right) {
        projectedLocation.x = _Location.x;
        projectedVelocity.x = 0;
    }

    _Location = projectedLocation;
    _Velocity = projectedVelocity;
}