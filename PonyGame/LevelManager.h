#pragma once

#include "Level.h"
#include "TileFactory.h"
#include <nowarn/vector>

namespace ParticleHomeEntertainment {
    class LevelManager {

    private:
        std::vector<Level> _Levels;
        uint32_t _CurrentLevel;
        uint32_t _CurrentScreen;

        LevelScreen _Screen;

        std::shared_ptr<BackgroundTile> _GrassTile;
        std::shared_ptr<BackgroundTile> _SkyTileNw, _SkyTileNe, _SkyTileSw, _SkyTileSe;
        std::shared_ptr<BackgroundTile> _MoonTileNw, _MoonTileNe, _MoonTileSw, _MoonTileSe;
    public:
        std::unique_ptr<DirectX::SpriteBatch> _SpriteBatch;
        std::vector<std::shared_ptr<BackgroundTile>> _Tiles;
        //TileFactory _TileFactory;

        LevelManager();

        ~LevelManager();

        LevelManager(const LevelManager&) = delete;

        LevelManager& operator=(const LevelManager&) = delete;

        void Initialize(Microsoft::WRL::ComPtr<ID3D11Device1> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext1> context);

        TileInteractiveEnum GetTileInteractivity(uint32_t x, uint32_t y);

        void SetLevel(uint32_t level, ID3D11DeviceContext* context);

        void LoadLevel(uint32_t level, ID3D11DeviceContext* context);

        void Draw();

        void Reset();
    };
}