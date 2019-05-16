#pragma once

#include <nowarn/SimpleMath.h>
#include <nowarn/Keyboard.h>
#include <nowarn/stdint.h>
#include <nowarn/wrl/client.h>
#include <nowarn/SpriteBatch.h>
#include <nowarn/vector>
#include <nowarn/string>

namespace ParticleHomeEntertainment {
    class Sprite {
    protected:
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

        virtual void Draw(DirectX::SpriteBatch& spriteBatch, const DirectX::SimpleMath::Vector2& location) = 0;

        void SetTexture(Microsoft::WRL::ComPtr<ID3D11Device1> device, const std::wstring& filePath);

        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetTexture();

        RECT GetSourceRectangle();

        void SetSourceRectangle(const RECT sourceRectangle);

        DirectX::SpriteEffects GetTransform();

        void SetTransform(DirectX::SpriteEffects effects);

        virtual void ResetTextures() = 0;
    };
}