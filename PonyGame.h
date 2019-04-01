#pragma once

#include <d3d11_1.h>
#include "SimpleMath.h"
#include <WICTextureLoader.h>
#include <wrl/client.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include "StepTimer.h"
#include <SpriteBatch.h>
#include <algorithm>
#include <exception>
#include <memory>

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
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _Texture;
        std::unique_ptr<DirectX::SpriteBatch> _SpriteBatch;
        DirectX::SimpleMath::Vector2 _ScreenPos;
        DirectX::SimpleMath::Vector2 _Origin;

        void Update(DX::StepTimer const& timer);
        void Render();

        void Clear();
        void Present();

        void CreateDevice();
        void CreateResources();

        void OnDeviceLost();

        // Device resources.
        HWND                                            _Window;
        int                                             _OutputWidth;
        int                                             _OutputHeight;

        D3D_FEATURE_LEVEL                               _FeatureLevel;
        Microsoft::WRL::ComPtr<ID3D11Device1>           _D3dDevice;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext1>    _D3dContext;

        Microsoft::WRL::ComPtr<IDXGISwapChain1>         _SwapChain;
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  _RenderTargetView;
        Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  _DepthStencilView;

        // Rendering loop timer.
        DX::StepTimer                                   _Timer;
    };
}