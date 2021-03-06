#include "Level.h"
#include <nowarn/filesystem>

using namespace ParticleHomeEntertainment;

Level::Level(uint32_t levelNumber) {
    _LevelNumber = levelNumber;
}

void Level::Load(Microsoft::WRL::ComPtr<ID3D11Device1> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext1> context) {
    //TODO: Read level metadata from file. Get number of screens, then read each screen in the loop
    
    std::filesystem::path levelPath(L"levels/01-01.json");
    if (levelPath.has_filename()) {
        // Deserialize level 1 metadata
        uint64_t level1Screens = 1;
         _Screens.resize(level1Screens);

        for (size_t screenNumber = 0; screenNumber < _Screens.size(); screenNumber++) {
            _Screens[screenNumber] = std::make_shared<LevelScreen>();
            _Screens[screenNumber]->Initialize(device, context);

            // Deserialize tiles from file
            _Screens[screenNumber]->_Tiles = {  };
            _Screens[screenNumber]->Load();
        }
    }
}

TileInteractiveEnum Level::GetTileInteractivity(uint32_t screen, uint32_t x, uint32_t y) {
    return _Screens[screen]->GetTileInteractivity(x, y);
}

void Level::Draw(uint32_t screen) {
    // Draw the level's current screen
    _Screens[screen]->Draw();
}