#pragma once

#include <nowarn/windef.h>
#include <nowarn/Keyboard.h>
#include <nowarn/Mouse.h>
#include <nowarn/vector>

namespace ParticleHomeEntertainment {
    class InputState {
        DirectX::Keyboard::KeyboardStateTracker _KeyboardStateTracker;
        std::unique_ptr<DirectX::Keyboard> _Keyboard;
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

        bool AreAnyKeysDown(std::vector<DirectX::Keyboard::Keys> keys);

        bool WasKeyReleased(DirectX::Keyboard::Keys key);
    };
}