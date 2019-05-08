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

    enum SpriteSpecialStateEnum {
        ON_GROUND, IN_AIR
    };

    enum SpriteCollisionResultEnum {
        COLLISION_NONE, COLLISION_TOP, COLLISION_BOTTOM, COLLISION_LEFT, COLLISION_RIGHT
    };

    class Sprite {
    public:
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _IdleTile;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _RunningTile;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _JumpingTile;

        uint8_t _CurrentFrame;
        uint32_t _IdleSpriteSheetWidth;
        uint32_t _IdleSpriteSheetHeight;
        uint32_t _RunningSpriteSheetWidth;
        uint32_t _RunningSpriteSheetHeight;
        uint32_t _JumpingSpriteSheetWidth;
        uint32_t _JumpingSpriteSheetHeight;
        double _TotalElapsedSec;

        DirectX::SimpleMath::Vector2 _Location;
        DirectX::SimpleMath::Vector2 _Velocity;
        SpriteFacingStateEnum _Facing;
        SpriteAnimationStateEnum _AnimationState;
        SpriteSpecialStateEnum _SpecialState;

        explicit Sprite();

        ~Sprite();

        void Animate(const double& elapsedSeconds);

        void GetTexture(ID3D11ShaderResourceView* ponyTexture, RECT& sourceRectangle, DirectX::SpriteEffects& transform);

        void ResetTiles();

        bool DetectCollision(const RECT& subjectRect, const RECT& objectRect, SpriteCollisionResultEnum& collisionResult);

        SpriteCollisionResultEnum GetCollisions(const DirectX::SimpleMath::Vector2& subjectLocation, const LevelScreen& screen);
    };
}