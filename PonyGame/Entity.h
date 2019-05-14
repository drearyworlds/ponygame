#pragma once

#include "Sprite.h"
#include "BackgroundTile.h"

namespace ParticleHomeEntertainment {
    class Entity : public Sprite {
    private:
        std::vector<DirectX::SimpleMath::Vector2> _CollisionTileCoordinatesList;

    protected:
        struct Rectangle {
            long x;
            long y;
            long width;
            long height;
        };

        DirectX::SimpleMath::Vector2 _Location;
        Rectangle _AaBbOffsetLeftFacing;
        Rectangle _AaBbOffsetRightFacing;
        RECT _BoundingBoxLocation;

        DirectX::SimpleMath::Vector2 _Velocity;

        void Move(int xVelocity, int yVelocity);

        void MoveX(int velocity);

        void MoveY(int velocity);

        virtual void UpdateStates() = 0;

        bool CollisionWithTile(float x, float y);

    public:
        explicit Entity(const float startingLocationX, const float startingLocationY);

        virtual ~Entity();

        DirectX::SimpleMath::Vector2 GetLocation() const;

        void SetLocation(const float x, const float y);

        RECT GetBoundingBoxLocation();

        std::vector<DirectX::SimpleMath::Vector2>& GetCollisionTileCoordinates();
    };
}