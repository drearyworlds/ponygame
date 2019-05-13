#pragma once

#include <nowarn/windef.h>
#include <nowarn/Keyboard.h>
#include <nowarn/Mouse.h>

namespace ParticleHomeEntertainment {
    class InputState {
        DirectX::Keyboard _Keyboard;
        DirectX::Keyboard::KeyboardStateTracker _KeyboardStateTracker;
        DirectX::Keyboard::State _PreviousState;
        DirectX::Mouse _Mouse;

    public:
        InputState();

        ~InputState();

        InputState(const InputState&) = delete;
        InputState& operator=(const InputState&) = delete;

        void UpdateKeyStates();

        bool IsKeyUp(DirectX::Keyboard::Keys key);

        bool WasKeyPressed(DirectX::Keyboard::Keys key);

        bool IsKeyDown(DirectX::Keyboard::Keys key);

        bool WasKeyReleased(DirectX::Keyboard::Keys key);
    };
}