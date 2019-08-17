#include "PonyGame.h"
#include "Sprite.h"
#include "Constants.h"
#include <nowarn/CommonStates.h>
#include <nowarn/DirectXMath.h>
#include <nowarn/DirectXColors.h>
#include <nowarn/DDSTextureLoader.h>

using namespace ParticleHomeEntertainment;

using Microsoft::WRL::ComPtr;

#pragma comment(lib, "d3d11.lib")

void DX::ThrowIfFailed(HRESULT hr) {
    if (FAILED(hr)) {
        // Set a breakpoint on this line to catch DirectX API errors
        throw std::exception();
    }
}

extern void ExitGame();

PonyGame::PonyGame() noexcept :
    // Initialize pony location
    _Pony(static_cast<float>(2 * SPRITE_WIDTH_PX), static_cast<float>(6 * SPRITE_HEIGHT_PX)) {
    _Window = nullptr;
    _OutputWidth = SCREEN_WIDTH_PX;
    _OutputHeight = SCREEN_HEIGHT_PX;
    _FeatureLevel = D3D_FEATURE_LEVEL_11_1;

    // Initialize Game State
    _GameState = IN_LEVEL;
}

PonyGame& PonyGame::Instance() {
    static PonyGame instance;
    return instance;
}

// Initialize the Direct3D resources required to run.
void PonyGame::Initialize(HWND window, int width, int height) {
    _Window = window;
    _OutputWidth = std::max(width, 1);
    _OutputHeight = std::max(height, 1);

    CreateDevice();

    InitializeTextures();

    CreateResources();

    _LevelManager.Initialize(_D3dDevice, _D3dContext);

    _LevelManager.SetLevel(1, _D3dDevice, _D3dContext);
    //_TileFactory.Initialize(device);

    _OriginLocationPx.x = static_cast<float>(0);
    _OriginLocationPx.y = static_cast<float>(0);

    //_Mouse.SetWindow(window);

    // Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    _Timer.SetFixedTimeStep(true);
    _Timer.SetTargetElapsedSeconds(1.0 / SECONDS_PER_MINUTE);
}

// These are the resources that depend on the device.
void PonyGame::CreateDevice() {
    const UINT CREATION_FLAGS = 0;

#ifdef _DEBUG
    //creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    static const D3D_FEATURE_LEVEL featureLevels[] {
        // TODO: Modify for supported Direct3D feature levels
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1,
    };

    // Create the DX11 API device object, and get a corresponding context.
    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> context;

    HRESULT hr = D3D11CreateDevice(
        // specify nullptr to use the default adapter
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        CREATION_FLAGS,
        featureLevels,
        _countof(featureLevels),
        D3D11_SDK_VERSION,
        // returns the Direct3D device created
        device.ReleaseAndGetAddressOf(),
        // returns feature level of device created
        &_FeatureLevel,
        // returns the device immediate context
        context.ReleaseAndGetAddressOf()
    );
    DX::ThrowIfFailed(hr);

#ifndef NDEBUG
    ComPtr<ID3D11Debug> d3dDebug;
    if (SUCCEEDED(device.As(&d3dDebug))) {
        ComPtr<ID3D11InfoQueue> d3dInfoQueue;
        if (SUCCEEDED(d3dDebug.As(&d3dInfoQueue))) {
#ifdef _DEBUG
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif
            D3D11_MESSAGE_ID hide[] = {
                D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
                // TODO: Add more message IDs here as needed.
            };
            D3D11_INFO_QUEUE_FILTER filter = {};
            filter.DenyList.NumIDs = _countof(hide);
            filter.DenyList.pIDList = hide;
            d3dInfoQueue->AddStorageFilterEntries(&filter);
        }
    }
#endif

    DX::ThrowIfFailed(device.As(&_D3dDevice));
    DX::ThrowIfFailed(context.As(&_D3dContext));
}

// Initialize device dependent objects here (independent of window size).
void PonyGame::InitializeTextures() {
    _Pony.Initialize(_D3dContext, _D3dDevice);
    //_LevelManager.Initialize(_D3dDevice, _D3dContext);
}

// Allocate all memory resources that change on a window SizeChanged event.
void PonyGame::CreateResources() {
    // Clear the previous window size specific context.
    ID3D11RenderTargetView* nullViews[] = { nullptr };
    _D3dContext->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
    _RenderTargetView.Reset();
    _DepthStencilView.Reset();
    _D3dContext->Flush();

    UINT backBufferWidth = static_cast<UINT>(_OutputWidth);
    UINT backBufferHeight = static_cast<UINT>(_OutputHeight);
    DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
    DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    UINT backBufferCount = 2;

    // If the swap chain already exists, resize it, otherwise create one.
    if (_SwapChain) {
        HRESULT hr = _SwapChain->ResizeBuffers(backBufferCount, backBufferWidth, backBufferHeight, backBufferFormat, 0);

        if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET) {
            // If the device was removed for any reason, a new device and swap chain will need to be created.
            OnDeviceLost();

            // Everything is set up now. Do not continue execution of this method. OnDeviceLost will reenter this method 
            // and correctly set up the new device.
            return;
        } else {
            DX::ThrowIfFailed(hr);
        }
    } else {
        // First, retrieve the underlying DXGI Device from the D3D Device.
        ComPtr<IDXGIDevice1> dxgiDevice;
        DX::ThrowIfFailed(_D3dDevice.As(&dxgiDevice));

        // Identify the physical adapter (GPU or card) this device is running on.
        ComPtr<IDXGIAdapter> dxgiAdapter;
        DX::ThrowIfFailed(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));

        // And obtain the factory object that created it.
        ComPtr<IDXGIFactory2> dxgiFactory;
        DX::ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf())));

        // Create a descriptor for the swap chain.
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
        swapChainDesc.Width = backBufferWidth;
        swapChainDesc.Height = backBufferHeight;
        swapChainDesc.Format = backBufferFormat;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = backBufferCount;

        DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = {};
        fsSwapChainDesc.Windowed = TRUE;

        // Create a SwapChain from a Win32 window.
        DX::ThrowIfFailed(dxgiFactory->CreateSwapChainForHwnd(
            _D3dDevice.Get(),
            _Window,
            &swapChainDesc,
            &fsSwapChainDesc,
            nullptr,
            _SwapChain.ReleaseAndGetAddressOf()
        ));

        // This template does not support exclusive fullscreen mode and prevents DXGI from responding to the ALT+ENTER shortcut.
        DX::ThrowIfFailed(dxgiFactory->MakeWindowAssociation(_Window, DXGI_MWA_NO_ALT_ENTER));
    }

    // Obtain the backbuffer for this window which will be the final 3D rendertarget.
    ComPtr<ID3D11Texture2D> backBuffer;
    DX::ThrowIfFailed(_SwapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));

    // Create a view interface on the rendertarget to use on bind.
    DX::ThrowIfFailed(_D3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, _RenderTargetView.ReleaseAndGetAddressOf()));

    // Allocate a 2D surface as the depth/stencil buffer and create a DepthStencil view on this surface to use on bind.
    CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat, backBufferWidth, backBufferHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL);

    ComPtr<ID3D11Texture2D> depthStencil;
    DX::ThrowIfFailed(_D3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf()));

    CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
    DX::ThrowIfFailed(_D3dDevice->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, _DepthStencilView.ReleaseAndGetAddressOf()));

    // TODO: Initialize windows-size dependent objects here.
}

// Executes the basic game loop.
void PonyGame::Tick() {
    _InputState.UpdateKeyStates();

    if (_InputState.IsKeyDown(DirectX::Keyboard::Keys::Escape)) {
        ExitGame();
    }

    if (_GameState == PAUSED) {
        return;
    }



#pragma warning (push)
#pragma warning (disable: 4626)
    // Update the game world
    _Timer.Tick([&](const double& elapsedSecs) {
        _Pony.Tick(elapsedSecs);
    });
#pragma warning (pop)

    // Don't try to render anything before the first Update.
    if (_Timer.GetFrameCount() > 0) {
        // Draw the frame
        RenderScene();
    }
}

void PonyGame::RenderScene() {
    Clear();

    DrawBackground();

#ifdef _DEBUG
    HighlightCollisionTiles();
#endif

    DrawObstacles();

    DrawEnemies();

    DrawPony();

#ifdef _DEBUG
    DrawPonyBoundingBox();
#endif

    Present();
}

void PonyGame::Clear() {
    // Clear the views.
    _D3dContext->ClearRenderTargetView(_RenderTargetView.Get(), DirectX::Colors::White);
    _D3dContext->ClearDepthStencilView(_DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    _D3dContext->OMSetRenderTargets(1, _RenderTargetView.GetAddressOf(), _DepthStencilView.Get());

    // Set the viewport.
    CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(_OutputWidth), static_cast<float>(_OutputHeight));
    _D3dContext->RSSetViewports(1, &viewport);
}

void PonyGame::DrawBackground() {
    _LevelManager.Draw();
}

void PonyGame::HighlightCollisionTiles() {
    //_BackgroundSpriteBatch->Begin(DirectX::SpriteSortMode_Deferred, nullptr, nullptr);

    //RECT tileRectangle {};
    //tileRectangle.left = 0;
    //tileRectangle.top = 0;
    //tileRectangle.right = SPRITE_WIDTH_PX;
    //tileRectangle.bottom = SPRITE_HEIGHT_PX;

    //const float ROTATION = 0.f;
    //const float SCALE = 1.f;
    //const float LAYER_DEPTH = 0.f;

    //while (!_Pony.GetCollisionTileCoordinates().empty()) {
    //    DirectX::SimpleMath::Vector2 coords = _Pony.GetCollisionTileCoordinates().back();

    //    auto& currentScreen = _Levels[_CurrentLevel]._Screens[_Levels[_CurrentLevel]._CurrentScreen];

    //    auto rect = currentScreen.GetTileRect(currentScreen.GetTileIndex(coords.x, coords.y));
    //    DirectX::SimpleMath::Vector2 tileLocation = { static_cast<float>(rect.left),  static_cast<float>(rect.top) };

    //    _BackgroundSpriteBatch->Draw(currentScreen.GetTile(coords.x, coords.y)._Tile.Get(),
    //        tileLocation,
    //        &tileRectangle,
    //        DirectX::Colors::Black,
    //        ROTATION,
    //        _OriginLocationPx,
    //        SCALE,
    //        DirectX::SpriteEffects_None,
    //        LAYER_DEPTH);

    //    _Pony.GetCollisionTileCoordinates().pop_back();
    //}

    //_BackgroundSpriteBatch->End();
}

void PonyGame::DrawObstacles() {

}

void PonyGame::DrawEnemies() {

}

void PonyGame::DrawPony() {
    _Pony.Draw();
}

void PonyGame::DrawPonyBoundingBox() {

}

// Presents the back buffer contents to the screen.
void PonyGame::Present() {
    // The first argument instructs DXGI to block until VSync, putting the application
    // to sleep until the next VSync. This ensures we don't waste any cycles rendering
    // frames that will never be displayed to the screen.
    HRESULT hr = _SwapChain->Present(1, 0);

    // If the device was reset we must completely reinitialize the renderer.
    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET) {
        OnDeviceLost();
    } else {
        DX::ThrowIfFailed(hr);
    }
}

LevelManager& PonyGame::GetLevelManager() {
    return _LevelManager;
}

InputState& PonyGame::GetInputState() {
    return _InputState;
}

void PonyGame::GetDefaultSize(int& width, int& height) const {
    // Desired default window size (note minimum size is 320x200).
    width = SCREEN_WIDTH_PX;
    height = SCREEN_HEIGHT_PX;
}

// Message handlers
void PonyGame::OnActivated() {
    // TODO: PonyGame is becoming active window.
}

void PonyGame::OnDeactivated() {
    // TODO: PonyGame is becoming background window.
}

void PonyGame::OnSuspending() {
    // TODO: PonyGame is being power-suspended (or minimized).
}

void PonyGame::OnResuming() {
    _Timer.ResetElapsedTime();

    // TODO: PonyGame is being power-resumed (or returning from minimize).
}

void PonyGame::OnWindowSizeChanged(int width, int height) {
    _OutputWidth = std::min(static_cast<int>(SCREEN_WIDTH_PX), std::max(width, 1));
    _OutputHeight = std::min(static_cast<int>(SCREEN_HEIGHT_PX), std::max(height, 1));

    CreateResources();

    // TODO: PonyGame window is being resized.
}

void PonyGame::OnDeviceLost() {
    // Direct3D resource cleanup
    _Pony.Reset();

    _LevelManager.Reset();

    _DepthStencilView.Reset();
    _RenderTargetView.Reset();
    _SwapChain.Reset();
    _D3dContext.Reset();
    _D3dDevice.Reset();

    CreateDevice();

    CreateResources();
}