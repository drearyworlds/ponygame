#include "LevelScreen.h"
#include "SimpleMath.h"
#include "LevelManager.h"
#include "PonyGame.h"

using namespace ParticleHomeEntertainment;

LevelScreen::LevelScreen() {
}

size_t LevelScreen::GetTileIndex(const size_t x, const size_t y) const {
    size_t tileIndex = static_cast<size_t>(SCREEN_WIDTH_TILES * static_cast<size_t>(y) + static_cast<size_t>(x));
    if (tileIndex > _Tiles.size()) {
        tileIndex = _Tiles.size() - 1;
    }

    return tileIndex;
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
    //DirectX::SimpleMath::Vector2 tileLocation = {};
    //RECT tileRectangle;
    //tileRectangle.left = 0;
    //tileRectangle.top = 0;
    //tileRectangle.right = SPRITE_WIDTH_PX;
    //tileRectangle.bottom = SPRITE_HEIGHT_PX;

    //_SpriteBatch.Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred, nullptr);

    //for (size_t y = 0; y < SCREEN_HEIGHT_TILES; y++) {
    //    for (size_t x = 0; x < SCREEN_WIDTH_TILES; x++) {
    //        tileLocation.x = SPRITE_WIDTH_PX * static_cast<float>(x);
    //        tileLocation.y = SPRITE_HEIGHT_PX * static_cast<float>(y);
    //        _Tiles[GetTileIndex(x, y)]->_Sprite->Draw(_SpriteBatch, tileLocation);
    //    }
    //}

    //_SpriteBatch.End();
}

void LevelScreen::Reset() {
}
