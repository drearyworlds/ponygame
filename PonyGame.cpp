#include "PonyGame.h"
#include "Sprite.h"
#include "Constants.h"
#include <nowarn/CommonStates.h>
#include <nowarn/DirectXMath.h>
#include <nowarn/DirectXColors.h>
#include <nowarn/DDSTextureLoader.h>

using namespace DirectX;
using namespace ParticleHomeEntertainment;

using Microsoft::WRL::ComPtr;

#pragma comment(lib, "d3d11.lib")

namespace DX {
    void ThrowIfFailed(HRESULT hr) {
        if (FAILED(hr)) {
            // Set a breakpoint on this line to catch DirectX API errors
            throw std::exception();
        }
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
    _GameState = TITLE_SCREEN;
    _CurrentLevel = 1;
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

    CreateResources();

    //_Mouse.SetWindow(window);

    // Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    _Timer.SetFixedTimeStep(true);
    _Timer.SetTargetElapsedSeconds(1.0 / SECONDS_PER_MINUTE);
}

void PonyGame::LoadLevel(uint32_t level) {

    //TODO: Move this into a text file
    //TODO: Draw maps in layers. e.g. empty sky files, then star tiles, then foreground interactive layer
    if (level == 1) {
        _CurrentScreen._Tiles = {
        _GrassTile, _SkyTileNe, _SkyTileNw, _MoonTileNw, _MoonTileNe, _SkyTileNe, _SkyTileNw, _SkyTileNe, _SkyTileNw, _SkyTileNe, _SkyTileNw, _SkyTileNe, _SkyTileNw, _SkyTileNe, _SkyTileNw, _GrassTile,
        _GrassTile, _SkyTileSe, _SkyTileSw, _MoonTileSw, _MoonTileSe, _SkyTileSe, _SkyTileSw, _SkyTileSe, _SkyTileSw, _SkyTileSe, _SkyTileSw, _SkyTileSe, _SkyTileSw, _SkyTileSe, _SkyTileSw, _GrassTile,
        _GrassTile, _SkyTileNe, _SkyTileNw, _SkyTileNe, _SkyTileNw, _SkyTileNe, _SkyTileNw, _SkyTileNe, _SkyTileNw, _SkyTileNe, _SkyTileNw, _SkyTileNe, _SkyTileNw, _SkyTileNe, _SkyTileNw, _GrassTile,
        _GrassTile, _SkyTileSe, _SkyTileSw, _SkyTileSe, _SkyTileSw, _SkyTileSe, _SkyTileSw, _SkyTileSe, _SkyTileSw, _SkyTileSe, _SkyTileSw, _SkyTileSe, _SkyTileSw, _SkyTileSe, _SkyTileSw, _GrassTile,
        _GrassTile, _SkyTileNe, _SkyTileNw, _SkyTileNe, _SkyTileNw, _SkyTileNe, _SkyTileNw, _SkyTileNe, _SkyTileNw, _SkyTileNe, _SkyTileNw, _SkyTileNe, _SkyTileNw, _SkyTileNe, _SkyTileNw, _GrassTile,
        _GrassTile, _SkyTileSe, _SkyTileSw, _SkyTileSe, _SkyTileSw, _SkyTileSe, _SkyTileSw, _GrassTile, _SkyTileSw, _SkyTileSe, _SkyTileSw, _SkyTileSe, _SkyTileSw, _SkyTileSe, _SkyTileSw, _GrassTile,
        _GrassTile, _SkyTileNe, _SkyTileNw, _SkyTileNe, _SkyTileNw, _SkyTileNe, _GrassTile, _SkyTileNe, _SkyTileNw, _SkyTileNe, _SkyTileNw, _SkyTileNe, _SkyTileNw, _SkyTileNe, _SkyTileNw, _GrassTile,
        _GrassTile, _SkyTileSe, _SkyTileSw, _SkyTileSe, _GrassTile, _GrassTile, _SkyTileSw, _SkyTileSe, _SkyTileSw, _SkyTileSe, _SkyTileSw, _SkyTileSe, _SkyTileSw, _SkyTileSe, _SkyTileSw, _GrassTile,
        _GrassTile, _GrassTile, _GrassTile, _GrassTile, _GrassTile, _GrassTile, _GrassTile, _GrassTile, _GrassTile, _GrassTile, _GrassTile, _GrassTile, _GrassTile, _GrassTile, _GrassTile, _GrassTile
        };
    }
}

// Executes the basic game loop.
void PonyGame::Tick() {
    // Poll and save the current key/button states
    _KeyboardStateTracker.Update(_Keyboard.GetState());
    //auto mouse = _Mouse->GetState();

    if (_KeyboardStateTracker.IsKeyPressed(Keyboard::Keys::Escape)) {
        ExitGame();
    }

    _Pony.UpdateStates();

    // TODO: Handle collisions

    // Update the game world
    _Timer.Tick([=]() {
        UpdateGameWorld(_Timer);
    });

    // Draw the frame
    RenderScene();
}

void PonyGame::UpdateGameWorld(const DX::StepTimer& timer) {
    if (_GameState == PAUSED) {
        return;
    }

    _Pony.Animate(timer.GetElapsedSeconds());
}

void PonyGame::RenderScene() {
    // Don't try to render anything before the first Update.
    if (_Timer.GetFrameCount() > 0) {
        Clear();

        DrawBackground();

        DrawObstacles();

        DrawEnemies();

        DrawPony();

        Present();
    }
}

void PonyGame::Clear() {
    // Clear the views.
    _D3dContext->ClearRenderTargetView(_RenderTargetView.Get(), Colors::CornflowerBlue);
    _D3dContext->ClearDepthStencilView(_DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    _D3dContext->OMSetRenderTargets(1, _RenderTargetView.GetAddressOf(), _DepthStencilView.Get());

    // Set the viewport.
    CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(_OutputWidth), static_cast<float>(_OutputHeight));
    _D3dContext->RSSetViewports(1, &viewport);
}

void PonyGame::DrawBackground() {
    DirectX::CommonStates states(_D3dDevice.Get());

    _BackgroundSpriteBatch->Begin(SpriteSortMode_Deferred, nullptr, states.LinearWrap());

    DirectX::SimpleMath::Vector2 tileLocation = {};
    RECT tileRectangle {};
    tileRectangle.left = 0;
    tileRectangle.top = 0;
    tileRectangle.right = SPRITE_WIDTH_PX;
    tileRectangle.bottom = SPRITE_HEIGHT_PX;

    const float ROTATION = 0.f;
    const float SCALE = 1.f;
    const float LAYER_DEPTH = 0.f;

    for (float y = 0; y < SCREEN_HEIGHT_TILES; y++) {
        for (float x = 0; x < SCREEN_WIDTH_TILES; x++) {
            tileLocation.x = SPRITE_WIDTH_PX * x;
            tileLocation.y = SPRITE_HEIGHT_PX * y;
            _BackgroundSpriteBatch->Draw(_CurrentScreen.GetTile(x, y)._Tile.Get(),
                tileLocation,
                &tileRectangle,
                Colors::White,
                ROTATION,
                _OriginLocationPx,
                SCALE,
                DirectX::SpriteEffects_None,
                LAYER_DEPTH);
        }
    }

    _BackgroundSpriteBatch->End();
}

void PonyGame::DrawObstacles() {

}

void PonyGame::DrawEnemies() {

}

void PonyGame::DrawPony() {
    // Rendering code
    DirectX::CommonStates states(_D3dDevice.Get());

    _SpriteBatch->Begin(SpriteSortMode::SpriteSortMode_Deferred, states.NonPremultiplied());

    uint32_t frameWidth;
    ID3D11ShaderResourceView* ponyTexture = nullptr;
    RECT sourceRectangle;
    DirectX::SpriteEffects ponyTransform;

    _Pony.GetTexture(ponyTexture, sourceRectangle, ponyTransform);
    if (_Pony._AnimationState == SpriteAnimationStateEnum::JUMPING) {
        frameWidth = _Pony._JumpingSpriteSheetWidth / PONY_JUMPING_FRAMES;
        sourceRectangle.left = static_cast<int32_t>(frameWidth * _Pony._CurrentFrame);
        sourceRectangle.top = 0;
        sourceRectangle.right = static_cast<int32_t>(sourceRectangle.left + frameWidth);
        sourceRectangle.bottom = static_cast<int32_t>(_Pony._JumpingSpriteSheetHeight);
        ponyTexture = _Pony._JumpingTile.Get();
    } else if (_Pony._AnimationState == SpriteAnimationStateEnum::RUNNING) {
        frameWidth = _Pony._RunningSpriteSheetWidth / PONY_RUNNING_FRAMES;
        sourceRectangle.left = static_cast<int32_t>(frameWidth * _Pony._CurrentFrame);
        sourceRectangle.top = 0;
        sourceRectangle.right = static_cast<int32_t>(sourceRectangle.left + frameWidth);
        sourceRectangle.bottom = static_cast<int32_t>(_Pony._RunningSpriteSheetHeight);
        ponyTexture = _Pony._RunningTile.Get();
    } else {
        frameWidth = _Pony._IdleSpriteSheetWidth / PONY_IDLE_FRAMES;
        sourceRectangle.left = static_cast<int32_t>(frameWidth * _Pony._CurrentFrame);
        sourceRectangle.top = 0;
        sourceRectangle.right = static_cast<int32_t>(sourceRectangle.left + frameWidth);
        sourceRectangle.bottom = static_cast<int32_t>(_Pony._IdleSpriteSheetHeight);
        ponyTexture = _Pony._IdleTile.Get();
    }

    if (ponyTexture != nullptr) {
        const float ROTATION = 0.f;
        const float SCALE = 1.f;
        const float LAYER_DEPTH = 0.f;
        _SpriteBatch->Draw(ponyTexture,
            _Pony.GetLocation(),
            &sourceRectangle,
            Colors::White,
            ROTATION,
            _OriginLocationPx,
            SCALE,
            ponyTransform,
            LAYER_DEPTH);
    } else {
        // ponyTexture is nullptr
    }

    _SpriteBatch->End();
}

const LevelScreen& PonyGame::GetScreen() const {
    return _CurrentScreen;
}

const DirectX::Keyboard& PonyGame::GetKeyboard() const {
    return _Keyboard;
}

const DirectX::Keyboard::KeyboardStateTracker& PonyGame::GetKeyboardStateTracker() const {
    return _KeyboardStateTracker;
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

// Properties
void PonyGame::GetDefaultSize(int& width, int& height) const {
    // Desired default window size (note minimum size is 320x200).
    width = SCREEN_WIDTH_PX;
    height = SCREEN_HEIGHT_PX;
}

// These are the resources that depend on the device.
void PonyGame::CreateDevice() {
    UINT creationFlags = 0;

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
        creationFlags,
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

    // Initialize device dependent objects here (independent of window size).
    _SpriteBatch = std::make_unique<SpriteBatch>(_D3dContext.Get());
    _BackgroundSpriteBatch = std::make_unique<SpriteBatch>(_D3dContext.Get());

    // Background Tiles
    _GrassTile._Interactive = BackgroundTile::TileInteractiveEnum::Solid;
    _GrassTile._TileStyle = BackgroundTile::TileStyleEnum::Grass;
    DX::ThrowIfFailed(CreateDDSTextureFromFile(_D3dDevice.Get(), FILE_PATH_SPRITE_GRASS,
        nullptr, _GrassTile._Tile.ReleaseAndGetAddressOf()));

    _SkyTileNw._Interactive = BackgroundTile::TileInteractiveEnum::Empty;
    _SkyTileNw._TileStyle = BackgroundTile::TileStyleEnum::Sky;
    DX::ThrowIfFailed(CreateDDSTextureFromFile(_D3dDevice.Get(), FILE_PATH_SPRITE_SKYNW,
        nullptr, _SkyTileNw._Tile.ReleaseAndGetAddressOf()));

    _SkyTileNe._Interactive = BackgroundTile::TileInteractiveEnum::Empty;
    _SkyTileNe._TileStyle = BackgroundTile::TileStyleEnum::Sky;
    DX::ThrowIfFailed(CreateDDSTextureFromFile(_D3dDevice.Get(), FILE_PATH_SPRITE_SKYNE,
        nullptr, _SkyTileNe._Tile.ReleaseAndGetAddressOf()));

    _SkyTileSw._Interactive = BackgroundTile::TileInteractiveEnum::Empty;
    _SkyTileSw._TileStyle = BackgroundTile::TileStyleEnum::Sky;
    DX::ThrowIfFailed(CreateDDSTextureFromFile(_D3dDevice.Get(), FILE_PATH_SPRITE_SKYSW,
        nullptr, _SkyTileSw._Tile.ReleaseAndGetAddressOf()));

    _SkyTileSe._Interactive = BackgroundTile::TileInteractiveEnum::Empty;
    _SkyTileSe._TileStyle = BackgroundTile::TileStyleEnum::Sky;
    DX::ThrowIfFailed(CreateDDSTextureFromFile(_D3dDevice.Get(), FILE_PATH_SPRITE_SKYSE,
        nullptr, _SkyTileSe._Tile.ReleaseAndGetAddressOf()));

    _MoonTileNw._Interactive = BackgroundTile::TileInteractiveEnum::Empty;
    _MoonTileNw._TileStyle = BackgroundTile::TileStyleEnum::Sky;
    DX::ThrowIfFailed(CreateDDSTextureFromFile(_D3dDevice.Get(), FILE_PATH_SPRITE_MOONNW,
        nullptr, _MoonTileNw._Tile.ReleaseAndGetAddressOf()));

    _MoonTileNe._Interactive = BackgroundTile::TileInteractiveEnum::Empty;
    _MoonTileNe._TileStyle = BackgroundTile::TileStyleEnum::Sky;
    DX::ThrowIfFailed(CreateDDSTextureFromFile(_D3dDevice.Get(), FILE_PATH_SPRITE_MOONNE,
        nullptr, _MoonTileNe._Tile.ReleaseAndGetAddressOf()));

    _MoonTileSw._Interactive = BackgroundTile::TileInteractiveEnum::Empty;
    _MoonTileSw._TileStyle = BackgroundTile::TileStyleEnum::Sky;
    DX::ThrowIfFailed(CreateDDSTextureFromFile(_D3dDevice.Get(), FILE_PATH_SPRITE_MOONSW,
        nullptr, _MoonTileSw._Tile.ReleaseAndGetAddressOf()));

    _MoonTileSe._Interactive = BackgroundTile::TileInteractiveEnum::Empty;
    _MoonTileSe._TileStyle = BackgroundTile::TileStyleEnum::Sky;
    DX::ThrowIfFailed(CreateDDSTextureFromFile(_D3dDevice.Get(), FILE_PATH_SPRITE_MOONSE,
        nullptr, _MoonTileSe._Tile.ReleaseAndGetAddressOf()));

    // Pony Idle Resources
    ComPtr<ID3D11Resource> ponyIdleResource;
    hr = CreateDDSTextureFromFile(_D3dDevice.Get(), FILE_PATH_SPRITE_PONY_IDLE,
        ponyIdleResource.GetAddressOf(), _Pony._IdleTile.ReleaseAndGetAddressOf());
    DX::ThrowIfFailed(hr);

    ComPtr<ID3D11Texture2D> ponyIdleTexture;
    DX::ThrowIfFailed(ponyIdleResource.As(&ponyIdleTexture));

    CD3D11_TEXTURE2D_DESC ponyIdleDesc;
    ponyIdleTexture->GetDesc(&ponyIdleDesc);

    // Get total sprite sheet size
    _Pony._IdleSpriteSheetWidth = ponyIdleDesc.Width;
    _Pony._IdleSpriteSheetHeight = ponyIdleDesc.Height;

    // Pony Running Resources
    ComPtr<ID3D11Resource> ponyRunResource;
    hr = CreateDDSTextureFromFile(_D3dDevice.Get(), FILE_PATH_SPRITE_PONY_RUNNING,
        ponyRunResource.GetAddressOf(), _Pony._RunningTile.ReleaseAndGetAddressOf());
    DX::ThrowIfFailed(hr);

    ComPtr<ID3D11Texture2D> ponyRunTexture;
    DX::ThrowIfFailed(ponyRunResource.As(&ponyRunTexture));

    CD3D11_TEXTURE2D_DESC ponyRunDesc;
    ponyRunTexture->GetDesc(&ponyRunDesc);

    // Get total sprite sheet size
    _Pony._RunningSpriteSheetWidth = ponyRunDesc.Width;
    _Pony._RunningSpriteSheetHeight = ponyRunDesc.Height;

    // Pony Jumping Resources
    ComPtr<ID3D11Resource> ponyJumpResource;
    hr = CreateDDSTextureFromFile(_D3dDevice.Get(), FILE_PATH_SPRITE_PONY_JUMPING,
        ponyJumpResource.GetAddressOf(), _Pony._JumpingTile.ReleaseAndGetAddressOf());
    DX::ThrowIfFailed(hr);

    ComPtr<ID3D11Texture2D> ponyJumpTexture;
    DX::ThrowIfFailed(ponyJumpResource.As(&ponyJumpTexture));

    CD3D11_TEXTURE2D_DESC ponyJumpDesc;
    ponyJumpTexture->GetDesc(&ponyJumpDesc);

    // Get total sprite sheet size
    _Pony._JumpingSpriteSheetWidth = ponyJumpDesc.Width;
    _Pony._JumpingSpriteSheetHeight = ponyJumpDesc.Height;

    LoadLevel(_CurrentLevel);

    _OriginLocationPx.x = static_cast<float>(0);
    _OriginLocationPx.y = static_cast<float>(0);
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

void PonyGame::OnDeviceLost() {
    // Direct3D resource cleanup
    _Pony.ResetTiles();

    _SpriteBatch.reset();
    _BackgroundSpriteBatch.reset();

    _DepthStencilView.Reset();
    _RenderTargetView.Reset();
    _SwapChain.Reset();
    _D3dContext.Reset();
    _D3dDevice.Reset();

    CreateDevice();

    CreateResources();
}