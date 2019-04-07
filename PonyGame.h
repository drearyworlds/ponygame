#pragma once

// Use the C++ standard templated min/max
#define NOMINMAX
#include <d3d11_1.h>
#include "StepTimer.h"
#include <CommonStates.h>
#include <SimpleMath.h>
#include <SpriteBatch.h>
#include <WICTextureLoader.h>
#include <Keyboard.h>
#include <Mouse.h>
#include <wrl/client.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <algorithm>

namespace DX {
    inline void ThrowIfFailed(HRESULT hr) {
        if (FAILED(hr)) {
            // Set a breakpoint on this line to catch DirectX API errors
            throw std::exception();
        }
    }
}

namespace ParticleHomeEntertainment {
    class PonyGame {
    public:
        enum GameStateEnum {
            TITLE_SCREEN, IN_LEVEL, PAUSED, CREDITS
        };

        enum SpriteFacingEnum {
            LEFT, RIGHT
        };

        enum SpriteMovementState {
            IDLE, RUNNING, JUMPING
        };

        PonyGame() noexcept;

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

        // Sprite tiles
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _PonyIdleTile;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _PonyRunningTile;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _PonyJumpingTile;
        std::unique_ptr<DirectX::SpriteBatch> _SpriteBatch;
        DirectX::SimpleMath::Vector2 _PonyLocation;
        SpriteFacingEnum _PonyFacing = SpriteFacingEnum::RIGHT;
        SpriteMovementState _PonyState = SpriteMovementState::IDLE;

        GameStateEnum _GameState = TITLE_SCREEN;

        uint32_t _CurrentLevel;

        // Other
        DirectX::SimpleMath::Vector2 _OriginLocation;

        uint8_t _PonyCurrentFrame;
        float _TotalElapsedSec;
        uint32_t _PonyIdleSpriteSheetWidth;
        uint32_t _PonyIdleSpriteSheetHeight;
        uint32_t _PonyRunSpriteSheetWidth;
        uint32_t _PonyRunSpriteSheetHeight;
        uint32_t _PonyJumpSpriteSheetWidth;
        uint32_t _PonyJumpSpriteSheetHeight;

        std::unique_ptr<DirectX::Keyboard> _Keyboard;
        std::unique_ptr<DirectX::Mouse> _Mouse;

        void HandleInput();

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