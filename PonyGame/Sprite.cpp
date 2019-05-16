#include "Sprite.h"
#include "PonyGame.h"
#include "Constants.h"
#include "BackgroundTile.h"
#include <nowarn/CommonStates.h>
#include <DDSTextureLoader.h>

using namespace ParticleHomeEntertainment;

Sprite::Sprite() {
    _SourceRectangle = { 0,0,SPRITE_WIDTH_PX, SPRITE_HEIGHT_PX };
    _Transform = DirectX::SpriteEffects::SpriteEffects_None;
}

Sprite::~Sprite() {
}

void Sprite::SetTexture(Microsoft::WRL::ComPtr<ID3D11Device1> device, const std::wstring& filePath) {
    DX::ThrowIfFailed(DirectX::CreateDDSTextureFromFile(device.Get(), filePath.c_str(), nullptr, _CurrentTexture.ReleaseAndGetAddressOf()));
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& Sprite::GetTexture() {
    return _CurrentTexture;
}

RECT Sprite::GetSourceRectangle() {
    return _SourceRectangle;
}

void Sprite::SetSourceRectangle(const RECT sourceRectangle) {
    _SourceRectangle = sourceRectangle;

}

DirectX::SpriteEffects Sprite::GetTransform() {
    return _Transform;
}

void Sprite::SetTransform(DirectX::SpriteEffects effects) {
    _Transform = effects;
}