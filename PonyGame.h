#pragma once

#include "Pony.h"
#include "StepTimer.h"
#include "LevelScreen.h"
#include <nowarn/SimpleMath.h>
#include <nowarn/SpriteBatch.h>
#include <nowarn/Keyboard.h>
#include <nowarn/Mouse.h>
#include <nowarn/wrl/client.h>
#include <nowarn/algorithm>
#include <memory>

namespace ParticleHomeEntertainment {
    class PonyGame {
    public:
        enum GameStateEnum {
            TITLE_SCREEN, IN_LEVEL, PAUSED, CREDITS
        };

        PonyGame() noexcept;

        PonyGame(PonyGame& copy) = delete;
        PonyGame& operator=(PonyGame& copy) = delete;

        // Initialization and management
        void Initialize(HWND window, int width, int height);

        // Basic game loop
        void Tick();

        // Messages
        void OnActivated();
        void OnDeactivated();
        void OnSuspending();
        void OnResuming();
        void OnWindowSizeChanged(int width, int height);

        // Properties
        void GetDefaultSize(int& width, int& height) const;

    private:
        // Device resources.
        HWND _Window;
        int _OutputWidth;
        int _OutputHeight;

        D3D_FEATURE_LEVEL _FeatureLevel;
        Microsoft::WRL::ComPtr<ID3D11Device1> _D3dDevice;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext1> _D3dContext;
        Microsoft::WRL::ComPtr<IDXGISwapChain1> _SwapChain;
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _RenderTargetView;
        Microsoft::WRL::ComPtr<ID3D11DepthStencilView> _DepthStencilView;

        // Rendering loop timer.
        DX::StepTimer _Timer;

        // Background tiles
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _GrassTile;
        std::unique_ptr<DirectX::SpriteBatch> _BackgroundSpriteBatch;
        DirectX::SimpleMath::Vector2 _GrassLocation = {};

        // Sprites
        std::unique_ptr<DirectX::SpriteBatch> _SpriteBatch;

        GameStateEnum _GameState = TITLE_SCREEN;
        uint32_t _CurrentLevel;
        LevelScreen _CurrentLevelScreen;

        // Other
        DirectX::SimpleMath::Vector2 _OriginLocation;

        Pony _Pony;
        Sprite _ZombiePony;

        DirectX::Keyboard _Keyboard;
        DirectX::Keyboard::KeyboardStateTracker _KeyboardTracker;
        //std::unique_ptr<DirectX::Mouse> _Mouse;

        void UpdateGameWorld(const DX::StepTimer& timer);

        void RenderScene();

        /// <summary>Helper method to clear the back buffers.</summary>
        void Clear();

        void DrawBackground();

        void DrawObstacles();

        void DrawEnemies();

        void DrawPony();

        void Present();

        void CreateDevice();
        void CreateResources();

        void OnDeviceLost();
    };
}