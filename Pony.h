#pragma once

#include "Sprite.h"

namespace ParticleHomeEntertainment {
    class Pony : public Sprite {
    public:
        explicit Pony();

        ~Pony();

        void UpdateStates(const DirectX::Keyboard::State& keyboardState, const DirectX::Keyboard::KeyboardStateTracker& keyboardStateTracker, const LevelScreen& screen);
    };
}