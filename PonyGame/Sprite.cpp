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

void Sprite::UpdateTexture() {
    if (_AnimationState == SpriteAnimationStateEnum::JUMPING) {
        _SpriteSheetFrameWidth = _JumpingSpriteSheetWidth / PONY_JUMPING_FRAMES;
        _SourceRectangle.left = static_cast<int32_t>(_SpriteSheetFrameWidth * _CurrentFrame);
        _SourceRectangle.top = 0;
        _SourceRectangle.right = static_cast<int32_t>(_SourceRectangle.left + _SpriteSheetFrameWidth);
        _SourceRectangle.bottom = static_cast<int32_t>(_JumpingSpriteSheetHeight);
        _CurrentTile = _JumpingTile;
    } else if (_AnimationState == SpriteAnimationStateEnum::RUNNING) {
        _SpriteSheetFrameWidth = _RunningSpriteSheetWidth / PONY_RUNNING_FRAMES;
        _SourceRectangle.left = static_cast<int32_t>(_SpriteSheetFrameWidth * _CurrentFrame);
        _SourceRectangle.top = 0;
        _SourceRectangle.right = static_cast<int32_t>(_SourceRectangle.left + _SpriteSheetFrameWidth);
        _SourceRectangle.bottom = static_cast<int32_t>(_RunningSpriteSheetHeight);
        _CurrentTile = _RunningTile;
    } else {
        _SpriteSheetFrameWidth = _IdleSpriteSheetWidth / PONY_IDLE_FRAMES;
        _SourceRectangle.left = static_cast<int32_t>(_SpriteSheetFrameWidth * _CurrentFrame);
        _SourceRectangle.top = 0;
        _SourceRectangle.right = static_cast<int32_t>(_SourceRectangle.left + _SpriteSheetFrameWidth);
        _SourceRectangle.bottom = static_cast<int32_t>(_IdleSpriteSheetHeight);
        _CurrentTile = _IdleTile.Get();
    }

    _Transform = (_Facing == SpriteFacingStateEnum::FACING_RIGHT) ? DirectX::SpriteEffects_FlipHorizontally : DirectX::SpriteEffects_None;
}

void Sprite::ResetTiles() {
    _IdleTile.Reset();
    _RunningTile.Reset();
    _JumpingTile.Reset();
}