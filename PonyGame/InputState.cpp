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
    return std::any_of(keys.begin(), keys.end(), [&](DirectX::Keyboard::Keys key) {
        return _Keyboard->GetState().IsKeyDown(key);
    });
}

bool InputState::WasKeyReleased(DirectX::Keyboard::Keys key) {
    return _KeyboardStateTracker.IsKeyReleased(key);
}