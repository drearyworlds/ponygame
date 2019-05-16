#include "Sprite.h"
#include "Constants.h"
#include "BackgroundTile.h"
#include <nowarn/CommonStates.h>

using namespace ParticleHomeEntertainment;

Sprite::Sprite() {
    _SourceRectangle = {};
    _Transform = DirectX::SpriteEffects::SpriteEffects_None;
}

Sprite::~Sprite() {
}

const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& Sprite::GetTexture() const {
    return _CurrentTexture;
}

RECT& Sprite::GetSourceRectangle() {
    return _SourceRectangle;
}

DirectX::SpriteEffects Sprite::GetTransform() {
    return _Transform;
}