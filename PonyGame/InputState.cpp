#pragma once

#include "InputState.h"
#include <nowarn/Keyboard.h>
#include <nowarn/Mouse.h>
#include <nowarn/vector>
#include <nowarn/map>
#include <nowarn/algorithm>

using namespace ParticleHomeEntertainment;

InputState::InputState() {
    _Keyboard = std::make_unique<DirectX::Keyboard>();
}

InputState::~InputState() {
}

void InputState::UpdateKeyStates() {
    _KeyboardStateTracker.Update(_Keyboard->GetState());
}

bool InputState::IsKeyUp(DirectX::Keyboard::Keys key) {
    return _Keyboard->GetState().IsKeyUp(key);
}

bool InputState::WasKeyPressed(DirectX::Keyboard::Keys key) {
    return _KeyboardStateTracker.IsKeyPressed(key);
}

bool InputState::IsKeyDown(DirectX::Keyboard::Keys key) {
    return _Keyboard->GetState().IsKeyDown(key);
}

bool InputState::AreAnyKeysDown(std::vector<DirectX::Keyboard::Keys> keys) {
#pragma warning (push)
#pragma warning (disable: 4626)
    auto lambdaAnyKeyDown = [&](const DirectX::Keyboard::Keys& key) {
        return _Keyboard->GetState().IsKeyDown(key);
    };
#pragma warning (pop)

    return std::any_of(keys.begin(), keys.end(), lambdaAnyKeyDown);
}

bool InputState::WasKeyReleased(DirectX::Keyboard::Keys key) {
    return _KeyboardStateTracker.IsKeyReleased(key);
}
