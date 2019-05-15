#pragma once

#include "Entity.h"

namespace ParticleHomeEntertainment {
    class Pony : public Entity {
    public:
        explicit Pony(const float x, const float y);

        ~Pony();

        Pony(const Pony&) = delete;

        Pony& operator=(const Pony&) = delete;

        void Tick(const double& elapsedSecs) override;

        void Draw(Microsoft::WRL::ComPtr<ID3D11Device1> device, std::shared_ptr<DirectX::SpriteBatch> spriteBatch, const DirectX::SimpleMath::Vector2 originLocationPx);
    };
}