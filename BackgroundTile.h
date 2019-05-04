#pragma once

class BackgroundTile {
public:
    enum TileStyleEnum {
        Grass, Sky, Bush, TreeTrunk, TreeLeaves, Cloud
    };

    enum InteractiveEnum {
        Solid, Empty
    };

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _Tile;
    TileStyleEnum _TileStyle = Grass;
    InteractiveEnum _Interactive = Solid;
};
