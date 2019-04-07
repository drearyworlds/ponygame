#pragma once

#include <vector>

namespace ParticleHomeEntertainment {
    class LevelScreen {
        enum TileEnum {
            GRASS, SKY, TREE, CLOUD
        };

    private:
        std::vector<std::vector<TileEnum>> _Tiles;
    };
}