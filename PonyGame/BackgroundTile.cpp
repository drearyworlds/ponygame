#include "BackgroundTile.h"
#include "Constants.h"
using namespace ParticleHomeEntertainment;

BackgroundTile::BackgroundTile() {
    _Sprite = std::make_shared<BackgroundSprite>();
}

BackgroundTile::BackgroundTile(TileInteractiveEnum interactivity = TileInteractiveEnum::Empty) {
    _Interactive = interactivity;
    _Sprite = std::make_shared<BackgroundSprite>();
}

BackgroundTile::~BackgroundTile() {
}