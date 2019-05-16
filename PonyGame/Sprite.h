#pragma once

#include <nowarn/SimpleMath.h>
#include <nowarn/Keyboard.h>
#include <nowarn/stdint.h>
#include <nowarn/wrl/client.h>
#include <nowarn/SpriteBatch.h>
#include <nowarn/vector>

namespace ParticleHomeEntertainment {
    class Sprite {
    protected:
    public:
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _CurrentTexture;

        RECT _SourceRectangle;
        DirectX::SpriteEffects _Transform;

        const DirectX::SimpleMath::Vector2 _OriginLocationPx { 0, 0 };
        const float _Rotation = 0.f;
        const float _Scale = 1.f;
        const float _LayerDepth = 0.f;

    public:
        explicit Sprite();

        Sprite(const Sprite&) = delete;

        Sprite& operator=(const Sprite&) = delete;

        virtual ~Sprite();

        virtual void UpdateTexture() = 0;

        virtual void Draw(DirectX::SpriteBatch& spriteBatch, const DirectX::SimpleMath::Vector2& location) = 0;

        const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetTexture() const;

        RECT& GetSourceRectangle();

        DirectX::SpriteEffects GetTransform();

        virtual void ResetTextures() = 0;
    };
}