#pragma once

#include "BackgroundTile.h"
#include "Constants.h"
#include <nowarn/vector>

namespace ParticleHomeEntertainment {
    class LevelScreen {
    public:
        std::vector<BackgroundTile> _Tiles;

        BackgroundTile GetTile(const float x, const float y) const;

        RECT GetTileRect(const long& index) const;
    };
}