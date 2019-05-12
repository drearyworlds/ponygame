#include "Entity.h"
#include "Pony.h"
#include "Constants.h"
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
    if (PonyGame::Instance().GetKeyboard().GetState().Left) {
        facingChanged = (_Facing == SpriteFacingStateEnum::FACING_RIGHT);
        _Facing = SpriteFacingStateEnum::FACING_LEFT;

        if (!facingChanged) {
            MoveX(-xSpeed);
        }
    } else if (PonyGame::Instance().GetKeyboard().GetState().Right) {
        facingChanged = (_Facing == SpriteFacingStateEnum::FACING_LEFT);
        _Facing = SpriteFacingStateEnum::FACING_RIGHT;

        if (!facingChanged) {
            MoveX(xSpeed);
        }
    }

    _Velocity.y += ySpeed;

    // If she is on the ground
    if (_SpecialState == SpriteSpecialStateEnum::ON_GROUND) {
        if (PonyGame::Instance().GetKeyboardStateTracker().IsKeyPressed(DirectX::Keyboard::Keys::Space)) {
            // Give pony a burst of velocity
            _Velocity.y = PONY_JUMP_Y_VELOCITY;

            // Set her states
            _AnimationState = SpriteAnimationStateEnum::JUMPING;
            _SpecialState = SpriteSpecialStateEnum::IN_AIR;
        } else if (PonyGame::Instance().GetKeyboard().GetState().Left
            || PonyGame::Instance().GetKeyboard().GetState().Right) {
            _AnimationState = SpriteAnimationStateEnum::RUNNING;
        } else {
            _AnimationState = SpriteAnimationStateEnum::IDLE;
        }
    } else {
        // Keep existing special state while in the air
    }

    _Velocity.y += GRAVITY;
    _Velocity.y = std::min(FALLING_TERMINAL_VELOCITY, _Velocity.y);
    MoveY(static_cast<int>(_Velocity.y));
}

void Entity::MoveX(int velocity) {
    if (velocity > 0) {
        // Moving right
        float projectedTileX = (_Location.x + velocity + SPRITE_WIDTH_PX) / SPRITE_WIDTH_PX;

        DirectX::SimpleMath::Vector2 upperRightCornerCollisionCoordinates(projectedTileX, _Location.y / SPRITE_HEIGHT_PX);
        DirectX::SimpleMath::Vector2 lowerRightCornerCollisionCoordinates(projectedTileX, (_Location.y + SPRITE_HEIGHT_PX) / SPRITE_HEIGHT_PX);

        // Upper right corner
        const bool upperRightCornerCollision = CollisionWithTile(upperRightCornerCollisionCoordinates.x, upperRightCornerCollisionCoordinates.y);
        // Lower left corner
        const bool lowerRightCornerCollision = CollisionWithTile(lowerRightCornerCollisionCoordinates.x, lowerRightCornerCollisionCoordinates.y);

        // Upper right and lower right corners
        if (upperRightCornerCollision || lowerRightCornerCollision) {
            std::wcout<<L"Collision";

            if (upperRightCornerCollision) {
                _CollisionTileCoordinatesList.push_back(upperRightCornerCollisionCoordinates);
            }  if (lowerRightCornerCollision) {
                _CollisionTileCoordinatesList.push_back(lowerRightCornerCollisionCoordinates);
            }
        } else {
            _Location.x += velocity;
        }
    } else if (velocity < 0) {
        // Moving left
        float projectedTileX = (_Location.x + velocity) / SPRITE_WIDTH_PX;

        DirectX::SimpleMath::Vector2 upperLeftCornerCollisionCoordinates(projectedTileX, _Location.y / SPRITE_HEIGHT_PX);
        DirectX::SimpleMath::Vector2 lowerLeftCornerCollisionCoordinates(projectedTileX, (_Location.y + SPRITE_HEIGHT_PX) / SPRITE_HEIGHT_PX);

        // Upper left corner
        const bool upperLeftCornerCollision = CollisionWithTile(upperLeftCornerCollisionCoordinates.x, upperLeftCornerCollisionCoordinates.y);
        // Lower left corner
        const bool lowerLeftCornerCollision = CollisionWithTile(lowerLeftCornerCollisionCoordinates.x, lowerLeftCornerCollisionCoordinates.y);

        if (upperLeftCornerCollision || lowerLeftCornerCollision) {
            std::wcout<<L"Collision";

            if (upperLeftCornerCollision) {
                _CollisionTileCoordinatesList.push_back(upperLeftCornerCollisionCoordinates);
            }  if (lowerLeftCornerCollision) {
                _CollisionTileCoordinatesList.push_back(lowerLeftCornerCollisionCoordinates);
            }
        } else {
            _Location.x += velocity;
        }
    }
}

void Entity::MoveY(int velocity) {
    if (velocity < 0) {
        // Moving up
        float projectedTileY = (_Location.y + velocity) / SPRITE_HEIGHT_PX;

        DirectX::SimpleMath::Vector2 upperLeftCornerCollisionCoordinates(_Location.x / SPRITE_WIDTH_PX, projectedTileY);
        DirectX::SimpleMath::Vector2 upperRightCornerCollisionCoordinates((_Location.x + SPRITE_WIDTH_PX) / SPRITE_WIDTH_PX, projectedTileY);

        // Upper left corner
        const bool upperLeftCornerCollision = CollisionWithTile(upperLeftCornerCollisionCoordinates.x, upperLeftCornerCollisionCoordinates.y);
        // Upper right corner
        const bool upperRightCornerCollision = CollisionWithTile(upperRightCornerCollisionCoordinates.x, upperRightCornerCollisionCoordinates.y);

        if (upperLeftCornerCollision || upperRightCornerCollision) {
            std::wcout<<L"Collision";

            if (upperLeftCornerCollision) {
                _CollisionTileCoordinatesList.push_back(upperLeftCornerCollisionCoordinates);
            }  if (upperRightCornerCollision) {
                _CollisionTileCoordinatesList.push_back(upperRightCornerCollisionCoordinates);
            }
        } else {
            _Location.y += velocity;
        }
    } else if (velocity > 0) {
        // Moving down
        float projectedTileY = (_Location.y + velocity + SPRITE_HEIGHT_PX) / SPRITE_HEIGHT_PX;

        DirectX::SimpleMath::Vector2 lowerLeftCornerCollisionCoordinates(_Location.x / SPRITE_WIDTH_PX, projectedTileY);
        DirectX::SimpleMath::Vector2 lowerRightCornerCollisionCoordinates((_Location.x + SPRITE_WIDTH_PX) / SPRITE_WIDTH_PX, projectedTileY);

        // Lower left corner
        const bool lowerLeftCornerCollision = CollisionWithTile(lowerLeftCornerCollisionCoordinates.x, lowerLeftCornerCollisionCoordinates.y);
        // Lower right corner
        const bool lowerRightCornerCollision = CollisionWithTile(lowerRightCornerCollisionCoordinates.x, lowerRightCornerCollisionCoordinates.y);

        if (lowerLeftCornerCollision || lowerRightCornerCollision) {
            std::wcout<<L"Collision";

            if (lowerLeftCornerCollision) {
                _CollisionTileCoordinatesList.push_back(lowerLeftCornerCollisionCoordinates);
            }  if (lowerRightCornerCollision) {
                _CollisionTileCoordinatesList.push_back(lowerRightCornerCollisionCoordinates);
            }

            // Change state to on ground and zero out the velocity
            _SpecialState = SpriteSpecialStateEnum::ON_GROUND;
            _Velocity.y = 0;
        } else {
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