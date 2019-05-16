#include "Entity.h"
#include "ForegroundSprite.h"
#include "PonyGame.h"
#include "Sprite.h"
#include <nowarn/iostream>

using namespace ParticleHomeEntertainment;

Entity::Entity(const float startingLocationX, const float startingLocationY) {
    // Initialize entity velocity
    _Velocity.x = 0;
    _Velocity.y = 0;

    // Facing left
    //=======37px======//
    //=2px=PONY==26px==//
    //========0px======//
    _AaBbOffsetLeftFacing.x = 2;
    _AaBbOffsetLeftFacing.y = 37;
    _AaBbOffsetLeftFacing.width = 36;
    _AaBbOffsetLeftFacing.height = 26;

    // Facing right
    //=======37px======//
    //==26px==PONY=2px=//
    //========0px======//
    _AaBbOffsetRightFacing.x = 26;
    _AaBbOffsetRightFacing.y = _AaBbOffsetLeftFacing.y;
    _AaBbOffsetRightFacing.width = _AaBbOffsetLeftFacing.width;
    _AaBbOffsetRightFacing.height = _AaBbOffsetLeftFacing.height;

    // Sets location and bounding box coordinates based on offsets above
    SetLocation(startingLocationX, startingLocationY);

    _SpecialState = EntitySpecialStateEnum::IN_AIR;
}

Entity::~Entity() {
}

DirectX::SimpleMath::Vector2 Entity::GetLocation() const {
    return _Location;
}

void Entity::SetLocation(const float x, const float y) {
    _Location.x = x;
    _Location.y = y;
}

void Entity::SetIdle() {
    _Sprite.SetAnimationState(SpriteAnimationStateEnum::IDLE);
}

void Entity::SetRunning() {
    _Sprite.SetAnimationState(SpriteAnimationStateEnum::RUNNING);
}

void Entity::SetJumping() {
    _Sprite.SetAnimationState(SpriteAnimationStateEnum::JUMPING);
    _SpecialState = EntitySpecialStateEnum::IN_AIR;
}

RECT Entity::GetBoundingBoxLocation() {
    RECT boundingBoxLocation;

    if (_Sprite.IsFacingLeft()) {
        boundingBoxLocation.left = static_cast<long>(_Location.x + _AaBbOffsetLeftFacing.x);
        boundingBoxLocation.right = static_cast<long>(_Location.x + _AaBbOffsetLeftFacing.x + _AaBbOffsetLeftFacing.width);
        boundingBoxLocation.top = static_cast<long>(_Location.y + _AaBbOffsetLeftFacing.y);
        boundingBoxLocation.bottom = static_cast<long>(_Location.y + _AaBbOffsetLeftFacing.y + _AaBbOffsetLeftFacing.height);
    } else {
        boundingBoxLocation.left = static_cast<long>(_Location.x + _AaBbOffsetRightFacing.x);
        boundingBoxLocation.right = static_cast<long>(_Location.x + _AaBbOffsetRightFacing.x + _AaBbOffsetRightFacing.width);
        boundingBoxLocation.top = static_cast<long>(_Location.y + _AaBbOffsetRightFacing.y);
        boundingBoxLocation.bottom = static_cast<long>(_Location.y + _AaBbOffsetRightFacing.y + _AaBbOffsetRightFacing.height);
    }

    return boundingBoxLocation;
}

void Entity::Move(int xSpeed, int ySpeed) {
    bool facingChanged = false;
    // Handle X-Axis movement
    if (PonyGame::Instance().GetInputState().IsKeyDown(DirectX::Keyboard::Keys::Left)) {
        facingChanged = _Sprite.IsFacingRight();
        _Sprite.SetFacing(SpriteFacingStateEnum::FACING_LEFT);

        if (!facingChanged) {
            MoveX(-xSpeed);
        }
    } else if (PonyGame::Instance().GetInputState().IsKeyDown(DirectX::Keyboard::Keys::Right)) {
        facingChanged = _Sprite.IsFacingLeft();
        _Sprite.SetFacing(SpriteFacingStateEnum::FACING_RIGHT);

        if (!facingChanged) {
            MoveX(xSpeed);
        }
    }

    _Velocity.y += ySpeed;

    const bool SPACE_IS_DOWN = PonyGame::Instance().GetInputState().IsKeyDown(DirectX::Keyboard::Keys::Space);
    const bool SPACE_IS_PRESSED = PonyGame::Instance().GetInputState().WasKeyPressed(DirectX::Keyboard::Keys::Space);

    // If she is on the ground
    if (_SpecialState == EntitySpecialStateEnum::ON_GROUND) {
        if (SPACE_IS_PRESSED) {
            // Give entity a burst of velocity
            _Velocity.y = PONY_JUMP_Y_VELOCITY;

            // Set its states
            SetJumping();
        } else if (PonyGame::Instance().GetInputState().AreAnyKeysDown({ DirectX::Keyboard::Keys::Left, DirectX::Keyboard::Keys::Right })) {
            SetRunning();
        } else {
            SetIdle();
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
        auto boundingBoxLocation = GetBoundingBoxLocation();

        // Moving right
        float projectedRightTileX = static_cast<float>((boundingBoxLocation.right + velocity) / SPRITE_WIDTH_PX);
        float upperRightYTile = static_cast<float>(boundingBoxLocation.top / SPRITE_HEIGHT_PX);
        float lowerRightYTile = static_cast<float>(boundingBoxLocation.bottom / SPRITE_HEIGHT_PX);

        // Upper right corner or lower right corner
        if (CollisionWithTile(projectedRightTileX, upperRightYTile) || CollisionWithTile(projectedRightTileX, lowerRightYTile)) {
            _Location.x = (projectedRightTileX * SPRITE_WIDTH_PX) - _AaBbOffsetRightFacing.x - _AaBbOffsetRightFacing.width - 1;
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
        auto boundingBoxLocation = GetBoundingBoxLocation();

        // Moving left
        float projectedLeftTileX = static_cast<float>((boundingBoxLocation.left + velocity) / SPRITE_WIDTH_PX);
        float upperLeftYTile = static_cast<float>(boundingBoxLocation.top / SPRITE_HEIGHT_PX);
        float lowerLeftYTile = static_cast<float>(boundingBoxLocation.bottom / SPRITE_HEIGHT_PX);

        if (CollisionWithTile(projectedLeftTileX, upperLeftYTile) || CollisionWithTile(projectedLeftTileX, lowerLeftYTile)) {
            _Location.x = ((projectedLeftTileX + 1) * SPRITE_WIDTH_PX);
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
        auto boundingBoxLocation = GetBoundingBoxLocation();

        // Moving up
        float upperLeftXTile = static_cast<float>(boundingBoxLocation.left / SPRITE_WIDTH_PX);
        float upperRightXTile = static_cast<float>(boundingBoxLocation.right / SPRITE_WIDTH_PX);
        float projectedUpperYTile = static_cast<float>((boundingBoxLocation.top + velocity) / SPRITE_HEIGHT_PX);

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
        auto boundingBoxLocation = GetBoundingBoxLocation();

        // Moving down
        float lowerLeftXTile = static_cast<float>(boundingBoxLocation.left / SPRITE_WIDTH_PX);
        float lowerRightXTile = static_cast<float>(boundingBoxLocation.right / SPRITE_WIDTH_PX);
        float projectedLowerYTile = static_cast<float>((boundingBoxLocation.bottom + velocity) / SPRITE_HEIGHT_PX);

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
            _SpecialState = EntitySpecialStateEnum::ON_GROUND;
            _Velocity.y = 0;
        } else {
            // NO COLLUSION!!
            _SpecialState = EntitySpecialStateEnum::IN_AIR;
            _Location.y += velocity;
        }
    }
}

bool Entity::CollisionWithTile(float x, float y) {
    return PonyGame::Instance().GetLevelManager().GetTileInteractivity(static_cast<uint32_t>(x), static_cast<uint32_t>(y)) == TileInteractiveEnum::Solid;
}

std::vector<DirectX::SimpleMath::Vector2>& Entity::GetCollisionTileCoordinates() {
    return _CollisionTileCoordinatesList;
}