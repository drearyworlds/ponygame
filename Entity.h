#pragma once

#include "Sprite.h"
#include "BackgroundTile.h"

namespace ParticleHomeEntertainment {
    class Entity : public Sprite {
    public:
        explicit Entity(const float startingLocationX, const float startingLocationY);

        virtual ~Entity();

        DirectX::SimpleMath::Vector2 GetLocation() const;

    protected:
        DirectX::SimpleMath::Vector2 _Location;
        DirectX::SimpleMath::Vector2 _Velocity;

        void Move(int xVelocity, int yVelocity);

        void MoveX(int velocity);

        void MoveY(int velocity);

        virtual void UpdateStates() = 0;

        bool CollisionWithTile(float x, float y);
    };
}