#pragma once

#include "BackgroundTile.h"
#include <nowarn/vector>

namespace ParticleHomeEntertainment {
    class TileFactory {

    private:
        //std::unordered_map<TileStyleEnum, std::shared_ptr<BackgroundTile>, TileStyleEnumHash> _Tiles;

        // Background tiles
        //static BackgroundTile _GrassTile;
        //static BackgroundTile _SkyTileNw, _SkyTileNe, _SkyTileSw, _SkyTileSe;
        //static BackgroundTile _MoonTileNw, _MoonTileNe, _MoonTileSw, _MoonTileSe;

    public:
        void Initialize(Microsoft::WRL::ComPtr<ID3D11Device1> device);

        //std::shared_ptr<BackgroundTile> GetTile(TileStyleEnum style);

        void Reset();
    };
}