#pragma once

#include "LevelScreen.h"
#include <nowarn/stdint.h>

namespace ParticleHomeEntertainment {
    class Level {
    private:
        std::unique_ptr<DirectX::SpriteBatch> _SpriteBatch;
        uint32_t _LevelNumber;

    public:
        std::vector<std::shared_ptr<LevelScreen>> _Screens;

        Level(uint32_t levelNumber);

        Level(const Level&) = delete;

        Level& operator=(const Level&) = delete;

        void Load(Microsoft::WRL::ComPtr<ID3D11Device1> device, Microsoft::WRL::ComPtr<ID3D11DeviceContext1> context);

        void Draw(uint32_t screen);
    };
}