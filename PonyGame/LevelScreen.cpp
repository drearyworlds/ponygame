#include "LevelScreen.h"
#include "SimpleMath.h"
#include "LevelManager.h"
#include "PonyGame.h"
#include <nowarn/DDSTextureLoader.h>

using namespace ParticleHomeEntertainment;

LevelScreen::LevelScreen() {
}

void LevelScreen::Initialize(Microsoft::WRL::ComPtr<ID3D11Device1> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext1> context) {
    _SpriteBatch = std::make_unique<DirectX::SpriteBatch>(context.Get());
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

    _MoonTileNw = std::make_shared<BackgroundTile>(TileInteractiveEnum::Empty);
    _MoonTileNe = std::make_shared<BackgroundTile>(TileInteractiveEnum::Empty);
    _MoonTileSw = std::make_shared<BackgroundTile>(TileInteractiveEnum::Empty);
    _MoonTileSe = std::make_shared<BackgroundTile>(TileInteractiveEnum::Empty);
    DX::ThrowIfFailed(DirectX::CreateDDSTextureFromFile(device.Get(), FILE_PATH_SPRITE_MOONNW, nullptr, _MoonTileNw->_Sprite->_CurrentTexture.ReleaseAndGetAddressOf()));
    DX::ThrowIfFailed(DirectX::CreateDDSTextureFromFile(device.Get(), FILE_PATH_SPRITE_MOONNE, nullptr, _MoonTileNe->_Sprite->_CurrentTexture.ReleaseAndGetAddressOf()));
    DX::ThrowIfFailed(DirectX::CreateDDSTextureFromFile(device.Get(), FILE_PATH_SPRITE_MOONSW, nullptr, _MoonTileSw->_Sprite->_CurrentTexture.ReleaseAndGetAddressOf()));
    DX::ThrowIfFailed(DirectX::CreateDDSTextureFromFile(device.Get(), FILE_PATH_SPRITE_MOONSE, nullptr, _MoonTileSe->_Sprite->_CurrentTexture.ReleaseAndGetAddressOf()));

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
}

constexpr size_t LevelScreen::GetTileIndex(const size_t x, const size_t y) const {
    return static_cast<size_t>(SCREEN_WIDTH_TILES * std::min<size_t>(y, SCREEN_HEIGHT_TILES - 1) + std::min<size_t>(x, SCREEN_WIDTH_TILES - 1));
}

const TileInteractiveEnum LevelScreen::GetTileInteractivity(const size_t x, const size_t y) const {
    return _Tiles.at(GetTileIndex(x, y))->_Interactive;
}

RECT LevelScreen::GetTileRect(const size_t index) const {
    auto div = std::div(static_cast<long>(index), SCREEN_WIDTH_TILES);
    int64_t x = div.rem;
    int64_t y = div.quot;

    RECT tileRect;
    tileRect.top = static_cast<long>(SPRITE_HEIGHT_PX * static_cast<float>(y));
    tileRect.bottom = static_cast<long>(SPRITE_HEIGHT_PX * static_cast<float>(y) + SPRITE_HEIGHT_PX);
    tileRect.left = static_cast<long>(SPRITE_WIDTH_PX * static_cast<float>(x));
    tileRect.right = static_cast<long>(SPRITE_WIDTH_PX * static_cast<float>(x) + SPRITE_WIDTH_PX);
    return tileRect;
}

void LevelScreen::Draw() {
    // Draw the level screen's tile sprites
    _SpriteBatch->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred, nullptr);

    DirectX::SimpleMath::Vector2 tileLocation = {};

    for (size_t y = 0; y < SCREEN_HEIGHT_TILES; y++) {
        for (size_t x = 0; x < SCREEN_WIDTH_TILES; x++) {
            tileLocation.x = SPRITE_WIDTH_PX * static_cast<float>(x);
            tileLocation.y = SPRITE_HEIGHT_PX * static_cast<float>(y);

            _Tiles.at(GetTileIndex(x, y))->_Sprite->Draw(*_SpriteBatch, tileLocation);

            //_SpriteBatch->Draw(_Tiles.at(tileIndex)->_Sprite->_CurrentTexture.Get(),
            //    tileLocation,
            //    &tileRectangle,
            //    DirectX::Colors::White,
            //    0,
            //    { 0,0 },
            //    1,
            //    DirectX::SpriteEffects::SpriteEffects_None,
            //    0
            //);
        }
    }

    _SpriteBatch->End();
}

void LevelScreen::Reset() {
}
