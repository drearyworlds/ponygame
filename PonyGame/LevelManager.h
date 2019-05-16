#pragma once

#include "Level.h"
#include <nowarn/vector>

namespace ParticleHomeEntertainment {
    class LevelManager {

    private:
        std::vector<std::shared_ptr<Level>> _Levels;
        uint32_t _CurrentLevel;
        uint32_t _CurrentScreen;

        LevelScreen _Screen;

    public:
        std::vector<std::shared_ptr<BackgroundTile>> _Tiles;

        LevelManager();

        ~LevelManager();

        LevelManager(const LevelManager&) = delete;

        LevelManager& operator=(const LevelManager&) = delete;

        void Initialize(Microsoft::WRL::ComPtr<ID3D11Device1> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext1> context);

        TileInteractiveEnum GetTileInteractivity(uint32_t x, uint32_t y);

        void SetLevel(uint32_t level, Microsoft::WRL::ComPtr<ID3D11Device1> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext1> context);

        void Load(Microsoft::WRL::ComPtr<ID3D11Device1> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext1> context);

        void Draw();

        void Reset();
    };
}