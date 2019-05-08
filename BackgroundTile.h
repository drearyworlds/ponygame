#pragma once

#include <nowarn/wrl/client.h>
#include <nowarn/d3d11_1.h>

namespace ParticleHomeEntertainment {
    class BackgroundTile {
    public:
        enum TileStyleEnum {
            Grass, Sky, Bush, TreeTrunk, TreeLeaves, Cloud
        };

        enum TileInteractiveEnum {
            Solid, Empty
        };

        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _Tile;
        TileStyleEnum _TileStyle = Grass;
        TileInteractiveEnum _Interactive = Solid;
    };
}