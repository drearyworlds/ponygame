#pragma once

#include "Sprite.h"

namespace ParticleHomeEntertainment {
    enum SpriteFacingStateEnum {
        FACING_LEFT, FACING_RIGHT
    };

    enum SpriteAnimationStateEnum {
        IDLE, RUNNING, JUMPING
    };

    class ForegroundSprite : public Sprite {
    public:
        ForegroundSprite();

        ~ForegroundSprite();

        ForegroundSprite(const ForegroundSprite&) = delete;

        ForegroundSprite& operator=(const ForegroundSprite&) = delete;

        // TODO Make all of these fields private:
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _IdleTexture;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _RunningTexture;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _JumpingTexture;

        uint32_t _IdleSpriteSheetWidth;
        uint32_t _IdleSpriteSheetHeight;
        uint32_t _RunningSpriteSheetWidth;
        uint32_t _RunningSpriteSheetHeight;
        uint32_t _JumpingSpriteSheetWidth;
        uint32_t _JumpingSpriteSheetHeight;

        SpriteFacingStateEnum _Facing;
        SpriteAnimationStateEnum _AnimationState;

        uint32_t _SpriteSheetFrameWidth;

        uint8_t _CurrentFrame;

        double _TotalElapsedSec;

        bool IsFacingLeft();

        bool IsFacingRight();

        void SetFacing(SpriteFacingStateEnum facing);

        void SetAnimationState(SpriteAnimationStateEnum state);

        void Animate(const double& elapsedSeconds);

        void UpdateSpriteFrame();

        void Draw(DirectX::SpriteBatch& spriteBatch, const DirectX::SimpleMath::Vector2& location) override;

        void ResetTextures() override;
    };
}