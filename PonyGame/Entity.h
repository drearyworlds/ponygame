#pragma once

#include "ForegroundSprite.h"

namespace ParticleHomeEntertainment {
    class Entity {
    private:
        std::vector<DirectX::SimpleMath::Vector2> _CollisionTileCoordinatesList;

    protected:
        struct Rectangle {
            long x;
            long y;
            long width;
            long height;
        };

        enum class EntitySpecialStateEnum {
            ON_GROUND, IN_AIR
        };

        ForegroundSprite _Sprite;

        DirectX::SimpleMath::Vector2 _Location;
        Rectangle _AaBbOffsetLeftFacing;
        Rectangle _AaBbOffsetRightFacing;

        EntitySpecialStateEnum _SpecialState;

        DirectX::SimpleMath::Vector2 _Velocity;

        void Move(int xVelocity, int yVelocity);

        void MoveX(int velocity);

        void MoveY(int velocity);

        virtual void Tick(const double& elapsedSecs) = 0;

        bool CollisionWithTile(float x, float y);

    public:
        explicit Entity(const float startingLocationX, const float startingLocationY);

        virtual ~Entity();

        Entity(const Entity&) = delete;

        Entity& operator=(const Entity&) = delete;

        virtual void Reset() = 0;

        DirectX::SimpleMath::Vector2 GetLocation() const;

        void SetLocation(const float x, const float y);

        void SetIdle();

        void SetRunning();

        void SetJumping();

        RECT GetBoundingBoxLocation();

        std::vector<DirectX::SimpleMath::Vector2>& GetCollisionTileCoordinates();
    };
}