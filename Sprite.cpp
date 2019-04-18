#include "Sprite.h"
#include "Constants.h"

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
    _Facing = SpriteFacingStateEnum::RIGHT;
    _AnimationState = SpriteAnimationStateEnum::IDLE;

    // Initialize sprite location
    _Location.x = 0;
    _Location.y = 7 * SPRITE_SIZE_HEIGHT_PX;

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

    transform = (_Facing == RIGHT) ? DirectX::SpriteEffects_FlipHorizontally : DirectX::SpriteEffects_None;
}

void Sprite::ResetTiles() {
    _IdleTile.Reset();
    _RunningTile.Reset();
    _JumpingTile.Reset();
}