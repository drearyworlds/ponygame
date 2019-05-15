#include "Pony.h"
#include "Constants.h"
#include "PonyGame.h"
#include "CommonStates.h"
#include "SpriteBatch.h"
#include <nowarn/memory>

using namespace ParticleHomeEntertainment;

Pony::Pony(const float startingLocationX, const float startingLocationY) : Entity(startingLocationX, startingLocationY) {
}

Pony::~Pony() {
}

void Pony::Reset() {
    _Sprite.ResetTextures();
}

void Pony::Tick(const double& elapsedSecs) {
    Move(PONY_X_SPEED_PX, 0);

    _Sprite.Animate(elapsedSecs);
}

void Pony::Draw(Microsoft::WRL::ComPtr<ID3D11Device1> device, std::shared_ptr<DirectX::SpriteBatch> spriteBatch, DirectX::SimpleMath::Vector2 originLocationPx) {
    // Rendering code
    DirectX::CommonStates states(device.Get());

    spriteBatch->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred, states.NonPremultiplied());

    _Sprite.UpdateTexture();

    if (_Sprite.GetTexture() != nullptr) {
        const float ROTATION = 0.f;
        const float SCALE = 1.f;
        const float LAYER_DEPTH = 0.f;
        spriteBatch->Draw(_Sprite.GetTexture(),
            GetLocation(),
            &_Sprite.GetSourceRectangle(),
            DirectX::Colors::White,
            ROTATION,
            originLocationPx,
            SCALE,
            _Sprite.GetTransform(),
            LAYER_DEPTH);
    } else {
        // ponyTexture is nullptr
    }

    spriteBatch->End();
}