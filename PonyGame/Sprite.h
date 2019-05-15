#pragma once

#include <nowarn/SimpleMath.h>
#include <nowarn/Keyboard.h>
#include <nowarn/stdint.h>
#include <nowarn/wrl/client.h>
#include <nowarn/SpriteBatch.h>
#include <nowarn/vector>
#include "LevelScreen.h"

namespace ParticleHomeEntertainment {
    enum SpriteFacingStateEnum {
        FACING_LEFT, FACING_RIGHT
    };

    enum SpriteAnimationStateEnum {
        IDLE, RUNNING, JUMPING
    };

    struct SpriteCollisionResult {
        bool _Top = false;
        bool _Bottom = false;
        bool _Left = false;
        bool _Right = false;
    };

    class Sprite {
    private:
    public:
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _CurrentTile;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _IdleTile;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _RunningTile;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _JumpingTile;

        RECT _SourceRectangle;
        uint32_t _SpriteSheetFrameWidth;

        uint8_t _CurrentFrame;
        uint32_t _IdleSpriteSheetWidth;
        uint32_t _IdleSpriteSheetHeight;
        uint32_t _RunningSpriteSheetWidth;
        uint32_t _RunningSpriteSheetHeight;
        uint32_t _JumpingSpriteSheetWidth;
        uint32_t _JumpingSpriteSheetHeight;
        double _TotalElapsedSec;

        SpriteFacingStateEnum _Facing;
        SpriteAnimationStateEnum _AnimationState;
        DirectX::SpriteEffects _Transform;

        explicit Sprite();

        ~Sprite();

        void Animate(const double& elapsedSeconds);

        void UpdateTexture();

        void ResetTiles();
    };
}