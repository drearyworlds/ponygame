#include "Sprite.h"
#include "Constants.h"
#include "BackgroundTile.h"

using namespace ParticleHomeEntertainment;

Sprite::Sprite() {
    _CurrentFrame = 0;
    _IdleSpriteSheetWidth = 0;
    _IdleSpriteSheetHeight = 0;
    _RunningSpriteSheetWidth = 0;
    _RunningSpriteSheetHeight = 0;
    _JumpingSpriteSheetWidth = 0;
    _JumpingSpriteSheetHeight = 0;
    _TotalElapsedSec = 0.f;
    _Facing = SpriteFacingStateEnum::FACING_RIGHT;
    _AnimationState = SpriteAnimationStateEnum::IDLE;
    _SpecialState = SpriteSpecialStateEnum::ON_GROUND;

    // Initialize sprite location
    _Location.x = 2 * SPRITE_WIDTH_PX;
    _Location.y = 6 * SPRITE_HEIGHT_PX;

    // Initialize sprite velocity
    _Velocity.x = 0;
    _Velocity.y = 0;
}

Sprite::~Sprite() {
    ResetTiles();
}

void Sprite::Animate(const double& elapsedSeconds) {
    // Add the elapsed seconds to our total
    _TotalElapsedSec += elapsedSeconds;

    uint8_t totalFramesForCurrentState = 0;
    float timePerFrameForCurrentStateSec = 0;

    if (_AnimationState == SpriteAnimationStateEnum::JUMPING) {
        totalFramesForCurrentState = PONY_JUMPING_FRAMES;
        timePerFrameForCurrentStateSec = PONY_JUMPING_TIME_PER_FRAME_SEC;
    } else if (_AnimationState == SpriteAnimationStateEnum::RUNNING) {
        totalFramesForCurrentState = PONY_RUNNING_FRAMES;
        timePerFrameForCurrentStateSec = PONY_RUNNING_TIME_PER_FRAME_SEC;
    } else {
        totalFramesForCurrentState = PONY_IDLE_FRAMES;
        timePerFrameForCurrentStateSec = PONY_IDLE_TIME_PER_FRAME_SEC;
    }

    if (_TotalElapsedSec > timePerFrameForCurrentStateSec) {
        // Move to the next frame
        _CurrentFrame = static_cast<uint8_t>(++_CurrentFrame % totalFramesForCurrentState);
        // Subtract the time per frame from this sprite's total elapsed seconds
        _TotalElapsedSec -= timePerFrameForCurrentStateSec;
    }
}

void Sprite::GetTexture(ID3D11ShaderResourceView* texture, RECT& sourceRectangle, DirectX::SpriteEffects& transform) {
    texture = nullptr;

    if (_AnimationState == SpriteAnimationStateEnum::JUMPING) {
        uint32_t frameWidth = _JumpingSpriteSheetWidth / PONY_JUMPING_FRAMES;
        sourceRectangle.left = static_cast<int32_t>(frameWidth * _CurrentFrame);
        sourceRectangle.top = 0;
        sourceRectangle.right = static_cast<int32_t>(sourceRectangle.left + frameWidth);
        sourceRectangle.bottom = static_cast<int32_t>(_JumpingSpriteSheetHeight);
        *texture = *_JumpingTile.Get();
    } else if (_AnimationState == SpriteAnimationStateEnum::RUNNING) {
        uint32_t frameWidth = _RunningSpriteSheetWidth / PONY_RUNNING_FRAMES;
        sourceRectangle.left = static_cast<int32_t>(frameWidth * _CurrentFrame);
        sourceRectangle.top = 0;
        sourceRectangle.right = static_cast<int32_t>(sourceRectangle.left + frameWidth);
        sourceRectangle.bottom = static_cast<int32_t>(_RunningSpriteSheetHeight);
        *texture = *_RunningTile.Get();
    } else {
        uint32_t frameWidth = _IdleSpriteSheetWidth / PONY_IDLE_FRAMES;
        sourceRectangle.left = static_cast<int32_t>(frameWidth * _CurrentFrame);
        sourceRectangle.top = 0;
        sourceRectangle.right = static_cast<int32_t>(sourceRectangle.left + frameWidth);
        sourceRectangle.bottom = static_cast<int32_t>(_IdleSpriteSheetHeight);
        texture = _IdleTile.Get();
    }

    transform = (_Facing == SpriteFacingStateEnum::FACING_RIGHT) ? DirectX::SpriteEffects_FlipHorizontally : DirectX::SpriteEffects_None;
}

bool Sprite::DetectCollision(const RECT& subjectRect, const RECT& objectRect, SpriteCollisionResult& collisionResult) {
    RECT dest;
    bool collisionDetected = IntersectRect(&dest, &subjectRect, &objectRect);

    if (collisionDetected) {
        const float topCollision = static_cast<float>(objectRect.bottom - subjectRect.top);
        const float bottomCollision = static_cast<float>(subjectRect.bottom - objectRect.top);
        const float leftCollision = static_cast<float>(objectRect.right - subjectRect.left);
        const float rightCollision = static_cast<float>(subjectRect.right - objectRect.left);

        if (topCollision < bottomCollision && topCollision < leftCollision && topCollision < rightCollision) {
            collisionResult._Top = true;
        }

        if (bottomCollision < topCollision && bottomCollision < leftCollision && bottomCollision < rightCollision) {
            collisionResult._Bottom = true;
        }

        if (leftCollision < rightCollision && leftCollision < topCollision && leftCollision < bottomCollision) {
            collisionResult._Left = true;
        }

        if (rightCollision < leftCollision && rightCollision < topCollision && rightCollision < bottomCollision) {
            collisionResult._Right = true;
        }
    }

    return collisionDetected;
}

SpriteCollisionResult Sprite::GetCollisions(const DirectX::SimpleMath::Vector2& subjectLocation, const LevelScreen& screen) {
    SpriteCollisionResult collisionResult;

    RECT subjectRect;
    subjectRect.top = static_cast<long>(subjectLocation.y);
    subjectRect.bottom = static_cast<long>(subjectLocation.y + SPRITE_HEIGHT_PX);
    subjectRect.left = static_cast<long>(subjectLocation.x);
    subjectRect.right = static_cast<long>(subjectLocation.x + SPRITE_WIDTH_PX);

    for (size_t index = 0; index < screen._Tiles.size(); index++) {
        BackgroundTile tile = screen._Tiles.at(index);
        if (tile._Interactive == BackgroundTile::TileInteractiveEnum::Solid) {
            // Only detect collisions if we haven't already detected all four directions
            if (!(collisionResult._Top && collisionResult._Bottom
                && collisionResult._Left && collisionResult._Right)) {
                DetectCollision(subjectRect, screen.GetTileRect(index), collisionResult);
            }
        }
    }

    return collisionResult;
}

void Sprite::ResetTiles() {
    _IdleTile.Reset();
    _RunningTile.Reset();
    _JumpingTile.Reset();
}