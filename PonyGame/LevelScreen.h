#pragma once

#include "BackgroundTile.h"
#include "Constants.h"
#include "SpriteBatch.h"
#include <nowarn/vector>

namespace ParticleHomeEntertainment {
    class LevelScreen {
    private:
        //DirectX::SpriteBatch _SpriteBatch;

    public:
        std::vector<std::shared_ptr<BackgroundTile>> _Tiles;

        LevelScreen();

        LevelScreen(const LevelScreen&) = delete;

        LevelScreen& operator=(const LevelScreen&) = delete;

        size_t GetTileIndex(const size_t x, const size_t y) const;

        const TileInteractiveEnum GetTileInteractivity(const size_t x, const size_t y) const;

        RECT GetTileRect(const size_t index) const;

        void Draw();

        void Reset();
    };
}