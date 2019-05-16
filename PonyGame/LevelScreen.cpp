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
    _GrassTile = std::make_shared<BackgroundTile>(device, TileTypeEnum::Grass, TileInteractiveEnum::Solid);
    _SkyTileNw = std::make_shared<BackgroundTile>(device, TileTypeEnum::SkyNw, TileInteractiveEnum::Empty);
    _SkyTileNe = std::make_shared<BackgroundTile>(device, TileTypeEnum::SkyNe, TileInteractiveEnum::Empty);
    _SkyTileSw = std::make_shared<BackgroundTile>(device, TileTypeEnum::SkySw, TileInteractiveEnum::Empty);
    _SkyTileSe = std::make_shared<BackgroundTile>(device, TileTypeEnum::SkySe, TileInteractiveEnum::Empty);
    _MoonTileNw = std::make_shared<BackgroundTile>(device, TileTypeEnum::MoonNw, TileInteractiveEnum::Empty);
    _MoonTileNe = std::make_shared<BackgroundTile>(device, TileTypeEnum::MoonNe, TileInteractiveEnum::Empty);
    _MoonTileSw = std::make_shared<BackgroundTile>(device, TileTypeEnum::MoonSw, TileInteractiveEnum::Empty);
    _MoonTileSe = std::make_shared<BackgroundTile>(device, TileTypeEnum::MoonSe, TileInteractiveEnum::Empty);
}

void LevelScreen::Load() {
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
    return _Tiles.at(GetTileIndex(x, y))->GetTileType().GetInteractive();
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
        }
    }

    _SpriteBatch->End();
}

void LevelScreen::Reset() {
}
