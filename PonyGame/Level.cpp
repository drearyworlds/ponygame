#include "Level.h"

using namespace ParticleHomeEntertainment;

Level::Level() {
    _LevelNumber = 0;
}

Level::Level(uint32_t levelNumber) {
    _LevelNumber = levelNumber;
}

void Level::Load(Microsoft::WRL::ComPtr<ID3D11Device1> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext1> context) {
    _Screens.resize(1);

    for (size_t screenNumber = 0; screenNumber < _Screens.size(); screenNumber++) {
        _Screens[screenNumber] = std::make_shared<LevelScreen>();
        _Screens[screenNumber]->Initialize(device, context);
        _Screens[screenNumber]->Load();
    }
}

void Level::Draw(uint32_t screen) {
    // Draw the level's current screen
    _Screens[screen]->Draw();
}