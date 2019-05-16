#include "LevelManager.h"
#include "SimpleMath.h"
#include "PonyGame.h"
#include <nowarn/DDSTextureLoader.h>

using namespace ParticleHomeEntertainment;

LevelManager::LevelManager() {
    _CurrentLevel = 0;
    _CurrentScreen = 0;
}

LevelManager::~LevelManager() {
}

void LevelManager::Initialize(Microsoft::WRL::ComPtr<ID3D11Device1> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext1> context) {
}

TileInteractiveEnum LevelManager::GetTileInteractivity(uint32_t x, uint32_t y) {
    return _Levels[_CurrentLevel - 1]->_Screens[_CurrentScreen]->GetTileInteractivity(x, y);
}

void LevelManager::SetLevel(uint32_t level, Microsoft::WRL::ComPtr<ID3D11Device1> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext1> context) {
    _CurrentLevel = level;
    LoadLevel(_CurrentLevel, device, context);
}

void LevelManager::LoadLevel(uint32_t level, Microsoft::WRL::ComPtr<ID3D11Device1> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext1> context) {
    // Level array is 0-indexed
    uint32_t levelIndex = level -1;

    //TODO: Move this into a text file
    //TODO: Draw LevelScreens in layers. e.g. empty sky files, then star tiles, then foreground interactive layer

    _Levels.resize(_Levels.size() + 1);

    std::shared_ptr<Level> levelOne = std::make_shared<Level>();

    levelOne->_Screens.resize(1);

    levelOne->_Screens[0] = std::make_shared<LevelScreen>();
    levelOne->_Screens[0]->Initialize(device, context);

    _Levels[levelIndex] = levelOne;
    _CurrentLevel = level;
}

void LevelManager::Draw() {
    _Levels[_CurrentLevel - 1]->Draw();


}

void LevelManager::Reset() {
    //_TileFactory.Reset();
}