#include "BackgroundSprite.h"
#include "CommonStates.h"

using namespace ParticleHomeEntertainment;

BackgroundSprite::BackgroundSprite() {
}

BackgroundSprite::~BackgroundSprite() {
    ResetTextures();
}

void BackgroundSprite::UpdateTexture() {
}

void BackgroundSprite::Draw(DirectX::SpriteBatch& spriteBatch, const DirectX::SimpleMath::Vector2& location) {
    spriteBatch.Draw(_CurrentTexture.Get(),
        location,
        &_SourceRectangle,
        DirectX::Colors::White,
        _Rotation,
        _OriginLocationPx,
        _Scale,
        _Transform,
        _LayerDepth);
}

void BackgroundSprite::ResetTextures() {
    _CurrentTexture.Reset();
}