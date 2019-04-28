#pragma once

#include "BackgroundTile.h"
#include <vector>

namespace ParticleHomeEntertainment {
    class LevelScreen {
    private:
        std::vector<std::vector<BackgroundTile>> _Tiles;
    };
}