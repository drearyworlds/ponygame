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
    return _Levels[_CurrentLevel - 1]->GetTileInteractivity(_CurrentScreen, x, y);
}

void LevelManager::SetLevel(uint32_t level, Microsoft::WRL::ComPtr<ID3D11Device1> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext1> context) {
    // Set the current level and load it
    _CurrentLevel = level;
    Load(device, context);
}

void LevelManager::Load(Microsoft::WRL::ComPtr<ID3D11Device1> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext1> context) {
    // Load the current level
    std::shared_ptr<Level> level = std::make_shared<Level>(_CurrentLevel);
    level->Load(device, context);

    if (_Levels.size() < _CurrentLevel) {
        _Levels.resize(_Levels.size() + 1);
    }

    _Levels[_CurrentLevel - 1] = level;
}

void LevelManager::Draw() {
    // Draw the current level
    _Levels[_CurrentLevel - 1]->Draw(_CurrentScreen);


}

void LevelManager::Reset() {
}