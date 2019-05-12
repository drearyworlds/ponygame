#pragma once

#include "Entity.h"

namespace ParticleHomeEntertainment {
    class Pony : public Entity {
    public:
        explicit Pony(const float x, const float y);

        ~Pony();

        void UpdateStates() override;
    };
}