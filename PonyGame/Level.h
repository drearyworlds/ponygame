#pragma once

#include "LevelScreen.h"

namespace ParticleHomeEntertainment {
    class Level {
    public:
        Level();

        std::vector<std::shared_ptr<LevelScreen>> _Screens;
    };
}