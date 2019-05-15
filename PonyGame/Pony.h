#pragma once

#include "Entity.h"

namespace ParticleHomeEntertainment {
    class Pony : public Entity {
    public:
        explicit Pony(const float x, const float y);

        ~Pony();

        Pony(const Pony&) = delete;

        Pony& operator=(const Pony&) = delete;

        void Initialize(Microsoft::WRL::ComPtr<ID3D11DeviceContext1> context, Microsoft::WRL::ComPtr<ID3D11Device1> device);

        void Reset() override;

        void Tick(const double& elapsedSecs) override;

        void Draw(Microsoft::WRL::ComPtr<ID3D11Device1> device, const DirectX::SimpleMath::Vector2 originLocationPx);
    };
}