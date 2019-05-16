#include "BackgroundTile.h"
#include "Constants.h"

using namespace ParticleHomeEntertainment;

BackgroundTile::BackgroundTile(Microsoft::WRL::ComPtr<ID3D11Device1> device, TileTypeEnum type, TileInteractiveEnum interactivity)
    : _TileType(type, interactivity) {
    _Sprite = std::make_shared<BackgroundSprite>();
    _Sprite->SetTexture(device, _TileType.GetFilePath());
}

BackgroundTile::~BackgroundTile() {
}

BackgroundTileType BackgroundTile::GetTileType() {
    return _TileType;
}