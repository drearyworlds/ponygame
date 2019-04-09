#pragma once

#include "Sprite.h"

namespace ParticleHomeEntertainment {
    class Pony : public Sprite {
    public:
        explicit Pony();

        ~Pony();

        void UpdateState(const DirectX::Keyboard::State keyboardState);
    };
}