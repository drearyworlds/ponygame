#pragma once

#include "BackgroundTile.h"
#include "Constants.h"
#include "SpriteBatch.h"
#include <nowarn/vector>

namespace ParticleHomeEntertainment {
    class LevelScreen {
    private:
        std::unique_ptr<DirectX::SpriteBatch> _SpriteBatch;

        std::shared_ptr<BackgroundTile> _GrassTile;
        std::shared_ptr<BackgroundTile> _SkyTileNw, _SkyTileNe, _SkyTileSw, _SkyTileSe;
        std::shared_ptr<BackgroundTile> _MoonTileNw, _MoonTileNe, _MoonTileSw, _MoonTileSe;

    public:
        std::vector<std::shared_ptr<BackgroundTile>> _Tiles;

        LevelScreen();

        LevelScreen(const LevelScreen&) = delete;

        LevelScreen& operator=(const LevelScreen&) = delete;

        void Initialize(Microsoft::WRL::ComPtr<ID3D11Device1> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext1> context);

        size_t GetTileIndex(const size_t x, const size_t y) const;

        const TileInteractiveEnum GetTileInteractivity(const size_t x, const size_t y) const;

        RECT GetTileRect(const size_t index) const;

        void Draw();

        void Reset();
    };
}