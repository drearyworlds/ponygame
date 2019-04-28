#pragma once

class BackgroundTile {
    enum TileStyleEnum {
        Grass, Sky, Bush, TreeTrunk, TreeLeaves, Cloud
    };

    enum InteractiveEnum {
        Solid, Empty
    };

    TileStyleEnum _TileStyle = Grass;
    InteractiveEnum _Interactive = Solid;
};
