#pragma once

#include "LevelScreen.h"

namespace ParticleHomeEntertainment {
    class Level {
    private:
        std::unique_ptr<DirectX::SpriteBatch> _SpriteBatch;

    public:
        std::vector<std::shared_ptr<LevelScreen>> _Screens;

        Level();

        Level(const Level&) = delete;

        Level& operator=(const Level&) = delete;

        void Draw();
    };
}