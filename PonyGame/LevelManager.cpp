#include "LevelManager.h"
#include "SimpleMath.h"
#include "PonyGame.h"
#include <nowarn/DDSTextureLoader.h>

using namespace ParticleHomeEntertainment;

LevelManager::LevelManager() {
    _CurrentLevel = 0;
    _CurrentScreen = 0;
}

LevelManager::~LevelManager() {
}

void LevelManager::Initialize(Microsoft::WRL::ComPtr<ID3D11Device1> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext1> context) {
    _SpriteBatch = std::make_unique<DirectX::SpriteBatch>(context.Get());
    //_TileFactory.Initialize(device);
    _GrassTile = std::make_shared<BackgroundTile>(TileInteractiveEnum::Solid);
    DX::ThrowIfFailed(DirectX::CreateDDSTextureFromFile(device.Get(), FILE_PATH_SPRITE_GRASS, nullptr, _GrassTile->_Sprite->_CurrentTexture.ReleaseAndGetAddressOf()));

    _SkyTileNw = std::make_shared<BackgroundTile>(TileInteractiveEnum::Empty);
    _SkyTileNe = std::make_shared<BackgroundTile>(TileInteractiveEnum::Empty);
    _SkyTileSw = std::make_shared<BackgroundTile>(TileInteractiveEnum::Empty);
    _SkyTileSe = std::make_shared<BackgroundTile>(TileInteractiveEnum::Empty);
    DX::ThrowIfFailed(DirectX::CreateDDSTextureFromFile(device.Get(), FILE_PATH_SPRITE_SKYNW, nullptr, _SkyTileNw->_Sprite->_CurrentTexture.ReleaseAndGetAddressOf()));
    DX::ThrowIfFailed(DirectX::CreateDDSTextureFromFile(device.Get(), FILE_PATH_SPRITE_SKYNE, nullptr, _SkyTileNe->_Sprite->_CurrentTexture.ReleaseAndGetAddressOf()));
    DX::ThrowIfFailed(DirectX::CreateDDSTextureFromFile(device.Get(), FILE_PATH_SPRITE_SKYSW, nullptr, _SkyTileSw->_Sprite->_CurrentTexture.ReleaseAndGetAddressOf()));
    DX::ThrowIfFailed(DirectX::CreateDDSTextureFromFile(device.Get(), FILE_PATH_SPRITE_SKYSE, nullptr, _SkyTileSe->_Sprite->_CurrentTexture.ReleaseAndGetAddressOf()));
    //_Tiles[TileStyleEnum::SkyNw] = skyTileNw;
    //_Tiles[TileStyleEnum::SkyNe] = skyTileNe;
    //_Tiles[TileStyleEnum::SkySw] = skyTileSw;
    //_Tiles[TileStyleEnum::SkySe] = skyTileSe;

    _MoonTileNw = std::make_shared<BackgroundTile>(TileInteractiveEnum::Empty);
    _MoonTileNe = std::make_shared<BackgroundTile>(TileInteractiveEnum::Empty);
    _MoonTileSw = std::make_shared<BackgroundTile>(TileInteractiveEnum::Empty);
    _MoonTileSe = std::make_shared<BackgroundTile>(TileInteractiveEnum::Empty);
    DX::ThrowIfFailed(DirectX::CreateDDSTextureFromFile(device.Get(), FILE_PATH_SPRITE_MOONNW, nullptr, _MoonTileNw->_Sprite->_CurrentTexture.ReleaseAndGetAddressOf()));
    DX::ThrowIfFailed(DirectX::CreateDDSTextureFromFile(device.Get(), FILE_PATH_SPRITE_MOONNE, nullptr, _MoonTileNe->_Sprite->_CurrentTexture.ReleaseAndGetAddressOf()));
    DX::ThrowIfFailed(DirectX::CreateDDSTextureFromFile(device.Get(), FILE_PATH_SPRITE_MOONSW, nullptr, _MoonTileSw->_Sprite->_CurrentTexture.ReleaseAndGetAddressOf()));
    DX::ThrowIfFailed(DirectX::CreateDDSTextureFromFile(device.Get(), FILE_PATH_SPRITE_MOONSE, nullptr, _MoonTileSe->_Sprite->_CurrentTexture.ReleaseAndGetAddressOf()));
    //_Tiles[TileStyleEnum::MoonNw] = moonTileNw;
    //_Tiles[TileStyleEnum::MoonNe] = moonTileNe;
    //_Tiles[TileStyleEnum::MoonSw] = moonTileSw;
    //_Tiles[TileStyleEnum::MoonSe] = moonTileSe;
}

TileInteractiveEnum LevelManager::GetTileInteractivity(uint32_t x, uint32_t y) {
    //return _Levels[_CurrentLevel - 1]._Screens[_CurrentScreen]->GetTileInteractivity(x, y);

    size_t tileIndex = static_cast<size_t>(SCREEN_WIDTH_TILES * static_cast<size_t>(y) + static_cast<size_t>(x));
    if (tileIndex > _Tiles.size()) {
        tileIndex = _Tiles.size() - 1;
    }

    return _Tiles.at(tileIndex)->_Interactive;
}

void LevelManager::SetLevel(uint32_t level, ID3D11DeviceContext* context) {
    _CurrentLevel = level;
    LoadLevel(_CurrentLevel, context);
}

void LevelManager::LoadLevel(uint32_t level, ID3D11DeviceContext*) {
    // Level array is 0-indexed
    uint32_t levelIndex = level -1;

    //TODO: Move this into a text file
    //TODO: Draw LevelScreens in layers. e.g. empty sky files, then star tiles, then foreground interactive layer

    _Levels.resize(_Levels.size() + 1);

    Level levelOne;
    levelOne._Screens.resize(1);

    //auto grassTile = _TileFactory.GetTile(TileStyleEnum::Grass);
    //auto skyTileNw = _TileFactory.GetTile(TileStyleEnum::SkyNw);
    //auto skyTileNe = _TileFactory.GetTile(TileStyleEnum::SkyNe);
    //auto skyTileSw = _TileFactory.GetTile(TileStyleEnum::SkySw);
    //auto skyTileSe = _TileFactory.GetTile(TileStyleEnum::SkySe);
    //auto moonTileNw = _TileFactory.GetTile(TileStyleEnum::MoonNw);
    //auto moonTileNe = _TileFactory.GetTile(TileStyleEnum::MoonNe);
    //auto moonTileSw = _TileFactory.GetTile(TileStyleEnum::MoonSw);
    //auto moonTileSe = _TileFactory.GetTile(TileStyleEnum::MoonSe);

    //std::shared_ptr<LevelScreen> screen = std::make_shared<LevelScreen>(context);
    //screen->_Tiles = {
    //    grassTile, skyTileNe, skyTileNw, moonTileNw, moonTileNe, skyTileNe, skyTileNw, skyTileNe, skyTileNw, skyTileNe, skyTileNw, skyTileNe, skyTileNw, skyTileNe, skyTileNw, grassTile,
    //    grassTile, skyTileSe, skyTileSw, moonTileSw, moonTileSe, skyTileSe, skyTileSw, skyTileSe, skyTileSw, skyTileSe, skyTileSw, skyTileSe, skyTileSw, skyTileSe, skyTileSw, grassTile,
    //    grassTile, skyTileNe, skyTileNw, skyTileNe, skyTileNw, skyTileNe, skyTileNw, skyTileNe, skyTileNw, skyTileNe, skyTileNw, skyTileNe, skyTileNw, skyTileNe, skyTileNw, grassTile,
    //    grassTile, skyTileSe, skyTileSw, skyTileSe, skyTileSw, skyTileSe, skyTileSw, skyTileSe, skyTileSw, skyTileSe, skyTileSw, skyTileSe, skyTileSw, skyTileSe, skyTileSw, grassTile,
    //    grassTile, skyTileNe, skyTileNw, skyTileNe, skyTileNw, skyTileNe, skyTileNw, skyTileNe, skyTileNw, skyTileNe, skyTileNw, skyTileNe, skyTileNw, skyTileNe, skyTileNw, grassTile,
    //    grassTile, skyTileSe, skyTileSw, skyTileSe, skyTileSw, skyTileSe, skyTileSw, grassTile, skyTileSw, skyTileSe, skyTileSw, skyTileSe, skyTileSw, skyTileSe, skyTileSw, grassTile,
    //    grassTile, skyTileNe, skyTileNw, skyTileNe, skyTileNw, skyTileNe, grassTile, skyTileNe, skyTileNw, grassTile, skyTileNw, skyTileNe, skyTileNw, skyTileNe, skyTileNw, grassTile,
    //    grassTile, skyTileSe, skyTileSw, skyTileSe, grassTile, grassTile, skyTileSw, skyTileSe, grassTile, grassTile, grassTile, skyTileSe, skyTileSw, skyTileSe, skyTileSw, grassTile,
    //    grassTile, grassTile, grassTile, grassTile, grassTile, grassTile, grassTile, grassTile, grassTile, grassTile, grassTile, grassTile, grassTile, grassTile, grassTile, grassTile
    //};

    _Tiles = {
        _GrassTile, _SkyTileNe, _SkyTileNw, _MoonTileNw, _MoonTileNe, _SkyTileNe, _SkyTileNw, _SkyTileNe, _SkyTileNw, _SkyTileNe, _SkyTileNw, _SkyTileNe, _SkyTileNw, _SkyTileNe, _SkyTileNw, _GrassTile,
        _GrassTile, _SkyTileSe, _SkyTileSw, _MoonTileSw, _MoonTileSe, _SkyTileSe, _SkyTileSw, _SkyTileSe, _SkyTileSw, _SkyTileSe, _SkyTileSw, _SkyTileSe, _SkyTileSw, _SkyTileSe, _SkyTileSw, _GrassTile,
        _GrassTile, _SkyTileNe, _SkyTileNw, _SkyTileNe, _SkyTileNw, _SkyTileNe, _SkyTileNw, _SkyTileNe, _SkyTileNw, _SkyTileNe, _SkyTileNw, _SkyTileNe, _SkyTileNw, _SkyTileNe, _SkyTileNw, _GrassTile,
        _GrassTile, _SkyTileSe, _SkyTileSw, _SkyTileSe, _SkyTileSw, _SkyTileSe, _SkyTileSw, _SkyTileSe, _SkyTileSw, _SkyTileSe, _SkyTileSw, _SkyTileSe, _SkyTileSw, _SkyTileSe, _SkyTileSw, _GrassTile,
        _GrassTile, _SkyTileNe, _SkyTileNw, _SkyTileNe, _SkyTileNw, _SkyTileNe, _SkyTileNw, _SkyTileNe, _SkyTileNw, _SkyTileNe, _SkyTileNw, _SkyTileNe, _SkyTileNw, _SkyTileNe, _SkyTileNw, _GrassTile,
        _GrassTile, _SkyTileSe, _SkyTileSw, _SkyTileSe, _SkyTileSw, _SkyTileSe, _SkyTileSw, _GrassTile, _SkyTileSw, _SkyTileSe, _SkyTileSw, _SkyTileSe, _SkyTileSw, _SkyTileSe, _SkyTileSw, _GrassTile,
        _GrassTile, _SkyTileNe, _SkyTileNw, _SkyTileNe, _SkyTileNw, _SkyTileNe, _GrassTile, _SkyTileNe, _SkyTileNw, _GrassTile, _SkyTileNw, _SkyTileNe, _SkyTileNw, _SkyTileNe, _SkyTileNw, _GrassTile,
        _GrassTile, _SkyTileSe, _SkyTileSw, _SkyTileSe, _GrassTile, _GrassTile, _SkyTileSw, _SkyTileSe, _GrassTile, _GrassTile, _GrassTile, _SkyTileSe, _SkyTileSw, _SkyTileSe, _SkyTileSw, _GrassTile,
        _GrassTile, _GrassTile, _GrassTile, _GrassTile, _GrassTile, _GrassTile, _GrassTile, _GrassTile, _GrassTile, _GrassTile, _GrassTile, _GrassTile, _GrassTile, _GrassTile, _GrassTile, _GrassTile
    };

    //levelOne._Screens[0] = screen;

    _Levels[levelIndex] = levelOne;
    _CurrentLevel = level;
}

void LevelManager::Draw() {
    //_Levels[_CurrentLevel - 1]._Screens[_CurrentScreen]->Draw();

    DirectX::SimpleMath::Vector2 tileLocation = {};
    RECT tileRectangle;
    tileRectangle.left = 0;
    tileRectangle.top = 0;
    tileRectangle.right = SPRITE_WIDTH_PX;
    tileRectangle.bottom = SPRITE_HEIGHT_PX;

    _SpriteBatch->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred, nullptr);

    for (size_t y = 0; y < SCREEN_HEIGHT_TILES; y++) {
        for (size_t x = 0; x < SCREEN_WIDTH_TILES; x++) {
            tileLocation.x = SPRITE_WIDTH_PX * static_cast<float>(x);
            tileLocation.y = SPRITE_HEIGHT_PX * static_cast<float>(y);

            size_t tileIndex = static_cast<size_t>(SCREEN_WIDTH_TILES * static_cast<size_t>(y) + static_cast<size_t>(x));
            if (tileIndex > _Tiles.size()) {
                tileIndex = _Tiles.size() - 1;
            }

            _SpriteBatch->Draw(_Tiles.at(tileIndex)->_Sprite->_CurrentTexture.Get(),
                tileLocation,
                &tileRectangle,
                DirectX::Colors::White,
                0,
                { 0,0 },
                1,
                DirectX::SpriteEffects::SpriteEffects_None,
                0
            );
            //_GrassTile->_Sprite->Draw(*_SpriteBatch, tileLocation);
        }
    }

    _SpriteBatch->End();
}

void LevelManager::Reset() {
    //_TileFactory.Reset();
}