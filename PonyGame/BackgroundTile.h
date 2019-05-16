#pragma once

#include "BackgroundSprite.h"
#include <nowarn/wrl/client.h>
#include <nowarn/d3d11_1.h>
#include <nowarn/map>
#include <nowarn/unordered_map>

namespace ParticleHomeEntertainment {
    enum TileStyleEnum {
        Grass, SkyNw, SkyNe, SkySw, SkySe, MoonNw, MoonNe, MoonSw, MoonSe
    };

    // Allows use of TileStyleEnumHash in a std::unordered_map
    struct TileStyleEnumHash {
        TileStyleEnumHash() = default;

        template <typename T>
        std::size_t operator()(T t) const {
            return static_cast<std::size_t>(t);
        }
    };

    enum TileInteractiveEnum {
        Solid, Empty
    };

    class BackgroundTile {
    public:
        BackgroundTile();

        BackgroundTile(TileInteractiveEnum interactivity);

        ~BackgroundTile();

        BackgroundTile(const BackgroundTile&) = delete;

        BackgroundTile& operator=(const BackgroundTile&) = delete;

        std::shared_ptr<BackgroundSprite> _Sprite;

        TileInteractiveEnum _Interactive = Solid;
    };
}