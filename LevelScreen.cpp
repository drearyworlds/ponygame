#include "LevelScreen.h"

using namespace ParticleHomeEntertainment;

BackgroundTile LevelScreen::GetTile(const float x, const float y) const {
    auto tileIndex = SCREEN_WIDTH_TILES * static_cast<uint64_t>(y) + static_cast<uint64_t>(x);
    if (tileIndex > _Tiles.size()) {
        tileIndex = _Tiles.size() - 1;
    }
    return _Tiles.at(tileIndex);
}

RECT LevelScreen::GetTileRect(const long& index) const {
    auto div = std::div(index, SCREEN_WIDTH_TILES);
    int64_t x = div.rem;
    int64_t y = div.quot;

    RECT tileRect;
    tileRect.top = static_cast<long>(SPRITE_HEIGHT_PX * static_cast<float>(y));
    tileRect.bottom = static_cast<long>(SPRITE_HEIGHT_PX * static_cast<float>(y) + SPRITE_HEIGHT_PX);
    tileRect.left = static_cast<long>(SPRITE_WIDTH_PX * static_cast<float>(x));
    tileRect.right = static_cast<long>(SPRITE_WIDTH_PX * static_cast<float>(x) + SPRITE_WIDTH_PX);
    return tileRect;
}
