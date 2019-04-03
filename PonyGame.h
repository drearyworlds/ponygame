#pragma once

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
        enum SpriteFacingEnum {
            LEFT, RIGHT
        };

        enum SpriteMovementState {
            IDLE, RUNNING
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

        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _PonyIdleTexture;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _PonyRunningTexture;
        std::unique_ptr<DirectX::SpriteBatch> _SpriteBatch;
        std::unique_ptr<DirectX::CommonStates> _States;
        DirectX::SimpleMath::Vector2 _ScreenPosition;
        DirectX::SimpleMath::Vector2 _PonyLocation;
        SpriteFacingEnum _PonyFacing = SpriteFacingEnum::RIGHT;
        SpriteMovementState _PonyState = SpriteMovementState::IDLE;
        bool _Paused;

        uint8_t _PonyCurrentFrame;
        float _TotalElapsedSec;
        uint32_t _PonyIdleSpriteSheetWidth;
        uint32_t _PonyIdleSpriteSheetHeight;
        uint32_t _PonyRunSpriteSheetWidth;
        uint32_t _PonyRunSpriteSheetHeight;
        float _PonyIdleTimePerFrameSec;
        float _PonyRunTimePerFrameSec;

        std::unique_ptr<DirectX::Keyboard> _Keyboard;
        std::unique_ptr<DirectX::Mouse> _Mouse;

        void DrawBackground();

        void DrawPony();

        void Update(DX::StepTimer const& timer);
        void HandleInput();
        void Render();

        void Clear();
        void Present();

        void CreateDevice();
        void CreateResources();

        void OnDeviceLost();
    };
}