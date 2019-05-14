#include "Entity.h"
#include "PonyGame.h"
#include <nowarn/iostream>

using namespace ParticleHomeEntertainment;

Entity::Entity(const float startingLocationX, const float startingLocationY) {
    _Location.x = startingLocationX;
    _Location.y = startingLocationY;

    // Initialize entity velocity
    _Velocity.x = 0;
    _Velocity.y = 0;
}

Entity::~Entity() {
}

DirectX::SimpleMath::Vector2 ParticleHomeEntertainment::Entity::GetLocation() const {
    return _Location;
}

void Entity::Move(int xSpeed, int ySpeed) {
    bool facingChanged = false;
    // Handle X-Axis movement
    if (PonyGame::Instance().GetInputState().IsKeyDown(DirectX::Keyboard::Keys::Left)) {
        facingChanged = (_Facing == SpriteFacingStateEnum::FACING_RIGHT);
        _Facing = SpriteFacingStateEnum::FACING_LEFT;

        if (!facingChanged) {
            MoveX(-xSpeed);
        }
    } else if (PonyGame::Instance().GetInputState().IsKeyDown(DirectX::Keyboard::Keys::Right)) {
        facingChanged = (_Facing == SpriteFacingStateEnum::FACING_LEFT);
        _Facing = SpriteFacingStateEnum::FACING_RIGHT;

        if (!facingChanged) {
            MoveX(xSpeed);
        }
    }

    _Velocity.y += ySpeed;

    const bool SPACE_IS_DOWN = PonyGame::Instance().GetInputState().IsKeyDown(DirectX::Keyboard::Keys::Space);
    const bool SPACE_IS_PRESSED = PonyGame::Instance().GetInputState().WasKeyPressed(DirectX::Keyboard::Keys::Space);

    // If she is on the ground
    if (_SpecialState == SpriteSpecialStateEnum::ON_GROUND) {
        if (SPACE_IS_PRESSED) {
            // Give entity a burst of velocity
            _Velocity.y = PONY_JUMP_Y_VELOCITY;

            // Set its states
            _AnimationState = SpriteAnimationStateEnum::JUMPING;
            _SpecialState = SpriteSpecialStateEnum::IN_AIR;
        } else if (PonyGame::Instance().GetInputState().IsKeyDown(DirectX::Keyboard::Keys::Left)
            || PonyGame::Instance().GetInputState().IsKeyDown(DirectX::Keyboard::Keys::Right)) {
            _AnimationState = SpriteAnimationStateEnum::RUNNING;
        } else {
            _AnimationState = SpriteAnimationStateEnum::IDLE;
        }
    } else {
        // Keep existing special state while in the air
    }

    _Velocity.y += GRAVITY;

    if (_Velocity.y < 0 && !SPACE_IS_DOWN && !SPACE_IS_PRESSED) {
        _Velocity.y = 0;
    }

    _Velocity.y = std::min(FALLING_TERMINAL_VELOCITY, _Velocity.y);
    MoveY(static_cast<int>(_Velocity.y));
}

void Entity::MoveX(int velocity) {
    if (velocity > 0) {
        // Moving right
        float projectedRightTileX = (_Location.x + velocity + SPRITE_RIGHT_OFFSET_X) / SPRITE_WIDTH_PX;
        float upperRightYTile = _Location.y / SPRITE_HEIGHT_PX;
        float lowerRightYTile = (_Location.y + SPRITE_BOTTOM_OFFSET_Y) / SPRITE_HEIGHT_PX;

        // Upper right corner or lower right corner
        if (CollisionWithTile(projectedRightTileX, upperRightYTile) || CollisionWithTile(projectedRightTileX, lowerRightYTile)) {
#ifdef _DEBUG
            if (CollisionWithTile(projectedRightTileX, upperRightYTile)) {
                _CollisionTileCoordinatesList.push_back({ projectedRightTileX, upperRightYTile });
            }

            if (CollisionWithTile(projectedRightTileX, lowerRightYTile)) {
                _CollisionTileCoordinatesList.push_back({ projectedRightTileX, lowerRightYTile });
            }
#endif
        } else {
            // NO COLLUSION!!
            _Location.x += velocity;
        }
    } else if (velocity < 0) {
        // Moving left
        float projectedLeftTileX = (_Location.x + velocity) / SPRITE_WIDTH_PX;
        float upperLeftYTile = _Location.y / SPRITE_HEIGHT_PX;
        float lowerLeftYTile = (_Location.y + SPRITE_BOTTOM_OFFSET_Y) / SPRITE_HEIGHT_PX;

        if (CollisionWithTile(projectedLeftTileX, upperLeftYTile) || CollisionWithTile(projectedLeftTileX, lowerLeftYTile)) {
#ifdef _DEBUG
            if (CollisionWithTile(projectedLeftTileX, upperLeftYTile)) {
                _CollisionTileCoordinatesList.push_back({ projectedLeftTileX, upperLeftYTile });
            }

            if (CollisionWithTile(projectedLeftTileX, lowerLeftYTile)) {
                _CollisionTileCoordinatesList.push_back({ projectedLeftTileX, lowerLeftYTile });
            }
#endif
        } else {
            // NO COLLUSION!!
            _Location.x += velocity;
        }
    }
}

void Entity::MoveY(int velocity) {
    if (velocity < 0) {
        // Moving up
        float upperLeftXTile = _Location.x / SPRITE_WIDTH_PX;
        float upperRightXTile = (_Location.x + SPRITE_RIGHT_OFFSET_X) / SPRITE_WIDTH_PX;
        float projectedUpperYTile = (_Location.y + velocity) / SPRITE_HEIGHT_PX;

        if (CollisionWithTile(upperLeftXTile, projectedUpperYTile) || CollisionWithTile(upperRightXTile, projectedUpperYTile)) {
#ifdef _DEBUG
            if (CollisionWithTile(upperLeftXTile, projectedUpperYTile)) {
                _CollisionTileCoordinatesList.push_back({ upperLeftXTile, projectedUpperYTile });
            }

            if (CollisionWithTile(upperRightXTile, projectedUpperYTile)) {
                _CollisionTileCoordinatesList.push_back({ upperRightXTile, projectedUpperYTile });
            }
#endif
        } else {
            // NO COLLUSION!!
            _Location.y += velocity;
        }
    } else if (velocity > 0) {
        // Moving down
        float lowerLeftXTile = _Location.x / SPRITE_WIDTH_PX;
        float lowerRightXTile = (_Location.x + SPRITE_RIGHT_OFFSET_X) / SPRITE_WIDTH_PX;
        float projectedLowerYTile = (_Location.y + velocity + SPRITE_BOTTOM_OFFSET_Y) / SPRITE_HEIGHT_PX;

        if (CollisionWithTile(lowerLeftXTile, projectedLowerYTile) || CollisionWithTile(lowerRightXTile, projectedLowerYTile)) {
#ifdef _DEBUG
            if (CollisionWithTile(lowerLeftXTile, projectedLowerYTile)) {
                _CollisionTileCoordinatesList.push_back({ lowerLeftXTile, projectedLowerYTile });
            }

            if (CollisionWithTile(lowerRightXTile, projectedLowerYTile)) {
                _CollisionTileCoordinatesList.push_back({ lowerRightXTile, projectedLowerYTile });
            }
#endif
            // Change state to on ground and zero out the velocity
            _SpecialState = SpriteSpecialStateEnum::ON_GROUND;
            _Velocity.y = 0;
        } else {
            // NO COLLUSION!!
            _Location.y += velocity;
        }
    }
}

bool Entity::CollisionWithTile(float x, float y) {
    return PonyGame::Instance().GetScreen().GetTile(x, y)._TileStyle == BackgroundTile::TileInteractiveEnum::Solid;
}

std::vector<DirectX::SimpleMath::Vector2>& Entity::GetCollisionTileCoordinates() {
    return _CollisionTileCoordinatesList;
}