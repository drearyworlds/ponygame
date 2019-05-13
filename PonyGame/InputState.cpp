#pragma once

#include "InputState.h"
#include <nowarn/Keyboard.h>
#include <nowarn/Mouse.h>
#include <nowarn/map>

using namespace ParticleHomeEntertainment;

InputState::InputState() :_PreviousState({ false }) {
}

InputState::~InputState() {
}

void InputState::UpdateKeyStates() {
    _PreviousState = _Keyboard.GetState();
    _KeyboardStateTracker.Update(_Keyboard.GetState());
}

bool InputState::IsKeyUp(DirectX::Keyboard::Keys key) {
    return _KeyboardStateTracker.GetLastState().IsKeyUp(key);
}

bool InputState::WasKeyPressed(DirectX::Keyboard::Keys key) {
    // Was up and is now down
    return (!_PreviousState.IsKeyDown(key) && _KeyboardStateTracker.GetLastState().IsKeyDown(key));
}

bool InputState::IsKeyDown(DirectX::Keyboard::Keys key) {
    return _KeyboardStateTracker.GetLastState().IsKeyDown(key);
}

bool InputState::WasKeyReleased(DirectX::Keyboard::Keys key) {
    // Was down and is now up
    return (_PreviousState.IsKeyDown(key) && !_KeyboardStateTracker.GetLastState().IsKeyDown(key));
}