#pragma once

#include "BackgroundSprite.h"
#include "TileType.h"
#include <nowarn/wrl/client.h>
#include <nowarn/d3d11_1.h>
#include <nowarn/map>
#include <nowarn/unordered_map>

namespace ParticleHomeEntertainment {
    class BackgroundTile {
    private:
        TileType _TileType;

    public:
        BackgroundTile(Microsoft::WRL::ComPtr<ID3D11Device1> device, TileTypeEnum type, TileInteractiveEnum interactivity);

        ~BackgroundTile();

        BackgroundTile(const BackgroundTile&) = delete;
        BackgroundTile& operator=(const BackgroundTile&) = delete;

        std::shared_ptr<BackgroundSprite> _Sprite;

        TileType GetTileType();
    };
}