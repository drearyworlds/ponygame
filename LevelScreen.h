#pragma once

#include "BackgroundTile.h"
#include "Constants.h"
#include <nowarn/vector>

namespace ParticleHomeEntertainment {
    class LevelScreen {
    public:
        std::vector<BackgroundTile> _Tiles;

        BackgroundTile GetTile(const size_t& x, const size_t& y) const;

        RECT GetTileRect(const size_t& index) const;
    };
}