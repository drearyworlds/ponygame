#include "Sprite.h"
#include "Constants.h"
#include "BackgroundTile.h"
#include <nowarn/CommonStates.h>

using namespace ParticleHomeEntertainment;

Sprite::Sprite() {
    _SourceRectangle = { 0,0,SPRITE_WIDTH_PX, SPRITE_HEIGHT_PX };
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

void Sprite::SetSourceRectangle(const RECT sourceRectangle) {
    _SourceRectangle = sourceRectangle;

}

DirectX::SpriteEffects Sprite::GetTransform() {
    return _Transform;
}