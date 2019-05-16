#pragma once

#include <nowarn/string>

namespace ParticleHomeEntertainment {
    enum TileTypeEnum {
        Grass = 0,
        SkyNw = 1, SkyNe = 2, SkySw = 3, SkySe = 4,
        MoonNw = 5, MoonNe = 6, MoonSw = 7, MoonSe = 8
    };

    // Allows use of TileStyleEnumHash in a std::unordered_map
    struct TileTypeEnumHash {
        TileTypeEnumHash() = default;

        template <typename T>
        std::size_t operator()(T t) const {
            return static_cast<std::size_t>(t);
        }
    };

    enum TileInteractiveEnum {
        Empty = 0,
        Solid = 1
    };

    class BackgroundTileType {
    private:
        TileTypeEnum _Type;
        TileInteractiveEnum _Interactive;
        std::wstring _FilePath;

    public:
        BackgroundTileType(TileTypeEnum type, TileInteractiveEnum interactive);

        ~BackgroundTileType();

        TileInteractiveEnum GetInteractive();

        std::wstring GetFilePath();

        static std::wstring GetFilePathForTileTypeEnum(TileTypeEnum type);
    };
}