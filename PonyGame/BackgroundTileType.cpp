#include "BackgroundTileType.h"
#include "Constants.h"

using namespace ParticleHomeEntertainment;

BackgroundTileType::BackgroundTileType(TileTypeEnum type, TileInteractiveEnum interactive) {
    _Type = type;
    _Interactive = interactive;
    _FilePath = BackgroundTileType::GetFilePathForTileTypeEnum(type);
}

BackgroundTileType::~BackgroundTileType() {
}

TileInteractiveEnum BackgroundTileType::GetInteractive() {
    return _Interactive;
}

std::wstring BackgroundTileType::GetFilePath() {
    return _FilePath;
}

std::wstring BackgroundTileType::GetFilePathForTileTypeEnum(TileTypeEnum type) {
    switch (type) {
    case TileTypeEnum::Grass:
        return FILE_PATH_SPRITE_GRASS;
    case TileTypeEnum::SkyNw:
        return FILE_PATH_SPRITE_SKYNW;
    case TileTypeEnum::SkyNe:
        return FILE_PATH_SPRITE_SKYNE;
    case TileTypeEnum::SkySw:
        return FILE_PATH_SPRITE_SKYSW;
    case TileTypeEnum::SkySe:
        return FILE_PATH_SPRITE_SKYSE;
    case TileTypeEnum::MoonNw:
        return FILE_PATH_SPRITE_MOONNW;
    case TileTypeEnum::MoonNe:
        return FILE_PATH_SPRITE_MOONNE;
    case TileTypeEnum::MoonSw:
        return FILE_PATH_SPRITE_MOONSW;
    case TileTypeEnum::MoonSe:
        return FILE_PATH_SPRITE_MOONSE;
    default:
        return FILE_PATH_SPRITE_GRASS;
    }
}