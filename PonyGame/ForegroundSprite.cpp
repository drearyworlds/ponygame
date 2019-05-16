#include "ForegroundSprite.h"
#include "CommonStates.h"
#include "Constants.h"

using namespace ParticleHomeEntertainment;

ForegroundSprite::ForegroundSprite() {
    _IdleSpriteSheetWidth = 0;
    _IdleSpriteSheetHeight = 0;
    _RunningSpriteSheetWidth = 0;
    _RunningSpriteSheetHeight = 0;
    _JumpingSpriteSheetWidth = 0;
    _JumpingSpriteSheetHeight = 0;
    _Facing = SpriteFacingStateEnum::FACING_RIGHT;
    _AnimationState = SpriteAnimationStateEnum::IDLE;
    _Transform = DirectX::SpriteEffects::SpriteEffects_None;

    _SpriteSheetFrameWidth = SPRITE_WIDTH_PX;
    _CurrentFrame = 0;
    _TotalElapsedSec = 0.f;
}

ForegroundSprite::~ForegroundSprite() {
    ResetTextures();
}

bool ForegroundSprite::IsFacingLeft() {
    return _Facing == SpriteFacingStateEnum::FACING_LEFT;
}

bool ForegroundSprite::IsFacingRight() {
    return _Facing == SpriteFacingStateEnum::FACING_RIGHT;
}

void ForegroundSprite::SetFacing(SpriteFacingStateEnum facing) {
    _Facing = facing;
}

void ForegroundSprite::SetAnimationState(SpriteAnimationStateEnum state) {
    _AnimationState = state;
}

void ForegroundSprite::Animate(const double& elapsedSeconds) {
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

void ForegroundSprite::UpdateSpriteFrame() {
    if (_AnimationState == SpriteAnimationStateEnum::JUMPING) {
        _SpriteSheetFrameWidth = _JumpingSpriteSheetWidth / PONY_JUMPING_FRAMES;
        _SourceRectangle.left = static_cast<int32_t>(_SpriteSheetFrameWidth * _CurrentFrame);
        _SourceRectangle.top = 0;
        _SourceRectangle.right = static_cast<int32_t>(_SourceRectangle.left + _SpriteSheetFrameWidth);
        _SourceRectangle.bottom = static_cast<int32_t>(_JumpingSpriteSheetHeight);
        _CurrentTexture = _JumpingTexture;
    } else if (_AnimationState == SpriteAnimationStateEnum::RUNNING) {
        _SpriteSheetFrameWidth = _RunningSpriteSheetWidth / PONY_RUNNING_FRAMES;
        _SourceRectangle.left = static_cast<int32_t>(_SpriteSheetFrameWidth * _CurrentFrame);
        _SourceRectangle.top = 0;
        _SourceRectangle.right = static_cast<int32_t>(_SourceRectangle.left + _SpriteSheetFrameWidth);
        _SourceRectangle.bottom = static_cast<int32_t>(_RunningSpriteSheetHeight);
        _CurrentTexture = _RunningTexture;
    } else {
        _SpriteSheetFrameWidth = _IdleSpriteSheetWidth / PONY_IDLE_FRAMES;
        _SourceRectangle.left = static_cast<int32_t>(_SpriteSheetFrameWidth * _CurrentFrame);
        _SourceRectangle.top = 0;
        _SourceRectangle.right = static_cast<int32_t>(_SourceRectangle.left + _SpriteSheetFrameWidth);
        _SourceRectangle.bottom = static_cast<int32_t>(_IdleSpriteSheetHeight);
        _CurrentTexture = _IdleTexture.Get();
    }

    _Transform = (_Facing == SpriteFacingStateEnum::FACING_RIGHT) ? DirectX::SpriteEffects_FlipHorizontally : DirectX::SpriteEffects_None;
}

void ForegroundSprite::Draw(DirectX::SpriteBatch& spriteBatch, const DirectX::SimpleMath::Vector2& location) {
    spriteBatch.Draw(GetTexture().Get(),
        location,
        &_SourceRectangle,
        DirectX::Colors::White,
        _Rotation,
        _OriginLocationPx,
        _Scale,
        _Transform,
        _LayerDepth);
}

void ForegroundSprite::ResetTextures() {
    _CurrentTexture.Reset();
    _IdleTexture.Reset();
    _RunningTexture.Reset();
    _JumpingTexture.Reset();
}