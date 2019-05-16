#include "Pony.h"
#include "Constants.h"
#include "PonyGame.h"
#include "CommonStates.h"
#include "SpriteBatch.h"
#include <nowarn/memory>
#include <nowarn/DDSTextureLoader.h>

using namespace ParticleHomeEntertainment;

Pony::Pony(const float startingLocationX, const float startingLocationY)
    : Entity(startingLocationX, startingLocationY) {
}

Pony::~Pony() {
}

void Pony::Initialize(Microsoft::WRL::ComPtr<ID3D11DeviceContext1> context, Microsoft::WRL::ComPtr<ID3D11Device1> device) {
    _SpriteBatch = std::make_unique<DirectX::SpriteBatch>(context.Get());

    // Pony Idle Resources
    Microsoft::WRL::ComPtr<ID3D11Resource> ponyIdleResource;
    HRESULT hr = DirectX::CreateDDSTextureFromFile(device.Get(), FILE_PATH_SPRITE_PONY_IDLE, ponyIdleResource.GetAddressOf(), _Sprite._IdleTexture.ReleaseAndGetAddressOf());
    DX::ThrowIfFailed(hr);

    Microsoft::WRL::ComPtr<ID3D11Texture2D> ponyIdleTexture;
    DX::ThrowIfFailed(ponyIdleResource.As(&ponyIdleTexture));

    CD3D11_TEXTURE2D_DESC ponyIdleDesc;
    ponyIdleTexture->GetDesc(&ponyIdleDesc);

    // Get total sprite sheet size
    _Sprite._IdleSpriteSheetWidth = ponyIdleDesc.Width;
    _Sprite._IdleSpriteSheetHeight = ponyIdleDesc.Height;

    // Pony Running Resources
    Microsoft::WRL::ComPtr<ID3D11Resource> ponyRunResource;
    hr = DirectX::CreateDDSTextureFromFile(device.Get(), FILE_PATH_SPRITE_PONY_RUNNING, ponyRunResource.GetAddressOf(), _Sprite._RunningTexture.ReleaseAndGetAddressOf());
    DX::ThrowIfFailed(hr);

    Microsoft::WRL::ComPtr<ID3D11Texture2D> ponyRunTexture;
    DX::ThrowIfFailed(ponyRunResource.As(&ponyRunTexture));

    CD3D11_TEXTURE2D_DESC ponyRunDesc;
    ponyRunTexture->GetDesc(&ponyRunDesc);

    // Get total sprite sheet size
    _Sprite._RunningSpriteSheetWidth = ponyRunDesc.Width;
    _Sprite._RunningSpriteSheetHeight = ponyRunDesc.Height;

    // Pony Jumping Resources
    Microsoft::WRL::ComPtr<ID3D11Resource> ponyJumpResource;
    hr = DirectX::CreateDDSTextureFromFile(device.Get(), FILE_PATH_SPRITE_PONY_JUMPING, ponyJumpResource.GetAddressOf(), _Sprite._JumpingTexture.ReleaseAndGetAddressOf());
    DX::ThrowIfFailed(hr);

    Microsoft::WRL::ComPtr<ID3D11Texture2D> ponyJumpTexture;
    DX::ThrowIfFailed(ponyJumpResource.As(&ponyJumpTexture));

    CD3D11_TEXTURE2D_DESC ponyJumpDesc;
    ponyJumpTexture->GetDesc(&ponyJumpDesc);

    // Get total sprite sheet size
    _Sprite._JumpingSpriteSheetWidth = ponyJumpDesc.Width;
    _Sprite._JumpingSpriteSheetHeight = ponyJumpDesc.Height;
}

void Pony::Reset() {
    _Sprite.ResetTextures();
    _SpriteBatch.reset();
}

void Pony::Tick(const double& elapsedSecs) {
    Move(PONY_X_SPEED_PX, 0);

    _Sprite.Animate(elapsedSecs);
}

void Pony::Draw() {
    _SpriteBatch->Begin(DirectX::SpriteSortMode::SpriteSortMode_Deferred, nullptr);
    _Sprite.Draw(*_SpriteBatch, _Location);
    _SpriteBatch->End();
}