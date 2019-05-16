#pragma once

#include "Sprite.h"

namespace ParticleHomeEntertainment {
    class BackgroundSprite : public Sprite {
    public:
        BackgroundSprite();

        ~BackgroundSprite();

        BackgroundSprite(const BackgroundSprite&) = delete;

        BackgroundSprite& operator=(const BackgroundSprite&) = delete;

        void Draw(DirectX::SpriteBatch& spriteBatch, const DirectX::SimpleMath::Vector2& location) override;

        void ResetTextures() override;
    };
}