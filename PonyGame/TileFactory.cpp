#include "TileFactory.h"
#include "PonyGame.h"
#include <nowarn/DDSTextureLoader.h>

using namespace ParticleHomeEntertainment;

void TileFactory::Initialize(Microsoft::WRL::ComPtr<ID3D11Device1>) {
    //std::shared_ptr<BackgroundTile> grassTile = std::make_shared<BackgroundTile>(TileInteractiveEnum::Solid);
    //DX::ThrowIfFailed(DirectX::CreateDDSTextureFromFile(device, FILE_PATH_SPRITE_GRASS, nullptr, _GrassTile._Sprite->_CurrentTexture.ReleaseAndGetAddressOf()));
    //_Tiles[TileStyleEnum::Grass] = grassTile;

    //std::shared_ptr<BackgroundTile> skyTileNw = std::make_shared<BackgroundTile>(TileInteractiveEnum::Empty);
    //std::shared_ptr<BackgroundTile> skyTileNe = std::make_shared<BackgroundTile>(TileInteractiveEnum::Empty);
    //std::shared_ptr<BackgroundTile> skyTileSw = std::make_shared<BackgroundTile>(TileInteractiveEnum::Empty);
    //std::shared_ptr<BackgroundTile> skyTileSe = std::make_shared<BackgroundTile>(TileInteractiveEnum::Empty);
    //DX::ThrowIfFailed(DirectX::CreateDDSTextureFromFile(device, FILE_PATH_SPRITE_SKYNW, nullptr, skyTileNw->_Sprite->_CurrentTexture.ReleaseAndGetAddressOf()));
    //DX::ThrowIfFailed(DirectX::CreateDDSTextureFromFile(device, FILE_PATH_SPRITE_SKYNE, nullptr, skyTileNe->_Sprite->_CurrentTexture.ReleaseAndGetAddressOf()));
    //DX::ThrowIfFailed(DirectX::CreateDDSTextureFromFile(device, FILE_PATH_SPRITE_SKYSW, nullptr, skyTileSw->_Sprite->_CurrentTexture.ReleaseAndGetAddressOf()));
    //DX::ThrowIfFailed(DirectX::CreateDDSTextureFromFile(device, FILE_PATH_SPRITE_SKYSE, nullptr, skyTileSe->_Sprite->_CurrentTexture.ReleaseAndGetAddressOf()));
    //_Tiles[TileStyleEnum::SkyNw] = skyTileNw;
    //_Tiles[TileStyleEnum::SkyNe] = skyTileNe;
    //_Tiles[TileStyleEnum::SkySw] = skyTileSw;
    //_Tiles[TileStyleEnum::SkySe] = skyTileSe;

    //std::shared_ptr<BackgroundTile> moonTileNw = std::make_shared<BackgroundTile>(TileInteractiveEnum::Empty);
    //std::shared_ptr<BackgroundTile> moonTileNe = std::make_shared<BackgroundTile>(TileInteractiveEnum::Empty);
    //std::shared_ptr<BackgroundTile> moonTileSw = std::make_shared<BackgroundTile>(TileInteractiveEnum::Empty);
    //std::shared_ptr<BackgroundTile> moonTileSe = std::make_shared<BackgroundTile>(TileInteractiveEnum::Empty);
    //DX::ThrowIfFailed(DirectX::CreateDDSTextureFromFile(device, FILE_PATH_SPRITE_MOONNW, nullptr, moonTileNw->_Sprite->_CurrentTexture.ReleaseAndGetAddressOf()));
    //DX::ThrowIfFailed(DirectX::CreateDDSTextureFromFile(device, FILE_PATH_SPRITE_MOONNE, nullptr, moonTileNe->_Sprite->_CurrentTexture.ReleaseAndGetAddressOf()));
    //DX::ThrowIfFailed(DirectX::CreateDDSTextureFromFile(device, FILE_PATH_SPRITE_MOONSW, nullptr, moonTileSw->_Sprite->_CurrentTexture.ReleaseAndGetAddressOf()));
    //DX::ThrowIfFailed(DirectX::CreateDDSTextureFromFile(device, FILE_PATH_SPRITE_MOONSE, nullptr, moonTileSe->_Sprite->_CurrentTexture.ReleaseAndGetAddressOf()));
    //_Tiles[TileStyleEnum::MoonNw] = moonTileNw;
    //_Tiles[TileStyleEnum::MoonNe] = moonTileNe;
    //_Tiles[TileStyleEnum::MoonSw] = moonTileSw;
    //_Tiles[TileStyleEnum::MoonSe] = moonTileSe;
}

//std::shared_ptr<BackgroundTile> TileFactory::GetTile(TileStyleEnum style) {
//    return _Tiles[style];
//}

void TileFactory::Reset() {
    //_Tiles[TileStyleEnum::Grass].reset();

    //_Tiles[TileStyleEnum::SkyNw].reset();
    //_Tiles[TileStyleEnum::SkyNe].reset();
    //_Tiles[TileStyleEnum::SkySw].reset();
    //_Tiles[TileStyleEnum::SkySe].reset();

    //_Tiles[TileStyleEnum::MoonNw].reset();
    //_Tiles[TileStyleEnum::MoonNe].reset();
    //_Tiles[TileStyleEnum::MoonSw].reset();
    //_Tiles[TileStyleEnum::MoonSe].reset();
}
