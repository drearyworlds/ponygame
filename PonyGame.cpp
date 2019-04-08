#include "PonyGame.h"
#include "Sprite.h"
#include "Constants.h"
#include <nowarn/CommonStates.h>
#include <nowarn/DirectXMath.h>
#include <nowarn/DirectXColors.h>
#include <nowarn/DDSTextureLoader.h>
#include <nowarn/WICTextureLoader.h>

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
    _Window(nullptr),
    _OutputWidth(SCREEN_WIDTH_PX),
    _OutputHeight(SCREEN_HEIGHT_PX),
    _FeatureLevel(D3D_FEATURE_LEVEL_11_1),
    _TotalElapsedSec(0) {
    // Initialize Game State
    _GameState = TITLE_SCREEN;
    _CurrentLevel = 1;

    // Initialize Pony Location
    _Pony._Location.x = 0;
    _Pony._Location.y = 7 * SPRITE_SIZE_HEIGHT_PX;

    _Pony._Velocity.x = 0;
    _Pony._Velocity.y = 0;
}

// Initialize the Direct3D resources required to run.
void PonyGame::Initialize(HWND window, int width, int height) {
    _Window = window;
    _OutputWidth = std::max(width, 1);
    _OutputHeight = std::max(height, 1);

    CreateDevice();

    CreateResources();

    _Keyboard = std::make_unique<Keyboard>();
    //_Mouse = std::make_unique<Mouse>();
    //_Mouse->SetWindow(window);

    // Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    _Timer.SetFixedTimeStep(true);
    _Timer.SetTargetElapsedSeconds(1.0 / SECONDS_PER_MINUTE);
}

// Executes the basic game loop.
void PonyGame::Tick() {
    // Poll and save the current key/button states
    HandleInput();

    // Update the game world
    _Timer.Tick([=]() {
        UpdateGameWorld(_Timer);
    });

    // Draw the frame
    RenderScene();
}

void PonyGame::HandleInput() {
    auto kb = _Keyboard->GetState();
    if (kb.Escape) {
        ExitGame();
    }

    //auto mouse = _Mouse->GetState();

    bool facingChanged = false;

    // Handle Animation State
    if (kb.Space) {
        if (_Pony._State != SpriteMovementState::JUMPING) {
            // Give pony some velocity
            _Pony._Velocity.y = PONY_JUMP_Y_VELOCITY;
        }

        _Pony._State = SpriteMovementState::JUMPING;
    } else if (kb.Left || kb.Right) {
        _Pony._State = SpriteMovementState::RUNNING;
    } else {
        _Pony._State = SpriteMovementState::IDLE;
    }

    // Handle X-Axis movement
    if (kb.Left) {
        facingChanged = (_Pony._Facing == SpriteFacingEnum::RIGHT);
        _Pony._Facing = SpriteFacingEnum::LEFT;

        if (!facingChanged) {
            _Pony._Location.x -= PONY_X_SPEED;
        }
    } else if (kb.Right) {
        facingChanged = (_Pony._Facing == SpriteFacingEnum::LEFT);
        _Pony._Facing = SpriteFacingEnum::RIGHT;

        if (!facingChanged) {
            _Pony._Location.x += PONY_X_SPEED;
        }
    }

    // Adjust pony Y-velocity for gravity
    _Pony._Velocity.y += GRAVITY;

    // Apply velocity 
    _Pony._Location.x += _Pony._Velocity.x;
    _Pony._Location.y += _Pony._Velocity.y;

    // But don't let her go below the grass
    if (_Pony._Location.y > (7 * SPRITE_SIZE_HEIGHT_PX)) {
        _Pony._Location.y = 7 * SPRITE_SIZE_HEIGHT_PX;
    }

    if (_Pony._Location.x < 0) {
        _Pony._Location.x = 0;
    } else if (_Pony._Location.x > SCREEN_WIDTH_PX - SPRITE_SIZE_WIDTH_PX) {
        _Pony._Location.x = SCREEN_WIDTH_PX - SPRITE_SIZE_WIDTH_PX;
    }
}

void PonyGame::UpdateGameWorld(const DX::StepTimer& timer) {
    _TotalElapsedSec += float(timer.GetElapsedSeconds());

    if (_GameState == PAUSED) {
        return;
    }

    uint8_t totalFramesForCurrentState = 0;
    float timePerFrameForCurrentStateSec = 0;

    if (_Pony._State == SpriteMovementState::JUMPING) {
        totalFramesForCurrentState = PONY_JUMPING_FRAMES;
        timePerFrameForCurrentStateSec = PONY_JUMPING_TIME_PER_FRAME_SEC;
    } else if (_Pony._State == SpriteMovementState::RUNNING) {
        totalFramesForCurrentState = PONY_RUNNING_FRAMES;
        timePerFrameForCurrentStateSec = PONY_RUNNING_TIME_PER_FRAME_SEC;
    } else {
        totalFramesForCurrentState = PONY_IDLE_FRAMES;
        timePerFrameForCurrentStateSec = PONY_IDLE_TIME_PER_FRAME_SEC;
    }



    if (_TotalElapsedSec > timePerFrameForCurrentStateSec) {
        _Pony._CurrentFrame++;
        _Pony._CurrentFrame = static_cast<uint8_t>(_Pony._CurrentFrame % totalFramesForCurrentState);
        _TotalElapsedSec -= timePerFrameForCurrentStateSec;
    }
}

void PonyGame::RenderScene() {
    // Don't try to render anything before the first Update.
    if (_Timer.GetFrameCount() > 0) {
        Clear();

        DrawBackground();

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

    const float ROTATION = 0.f;
    const float SCALE = 1.f;
    const float LAYER_DEPTH = 0.f;

    _GrassLocation.x = 0;
    _GrassLocation.y = (SCREEN_HEIGHT_TILES - 1) * SPRITE_SIZE_HEIGHT_PX;

    RECT tileRectangle{};
    tileRectangle.left = 0;
    tileRectangle.top = 0;
    tileRectangle.right = SCREEN_WIDTH_TILES * SPRITE_SIZE_WIDTH_PX;
    tileRectangle.bottom = SPRITE_SIZE_HEIGHT_PX;

    _BackgroundSpriteBatch->Draw(_GrassTile.Get(),
        _GrassLocation,
        &tileRectangle,
        Colors::White,
        ROTATION,
        _OriginLocation,
        SCALE,
        DirectX::SpriteEffects_None,
        LAYER_DEPTH);

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

    RECT sourceRect = {};
    uint32_t frameWidth;
    ID3D11ShaderResourceView* ponyTexture = nullptr;

    if (_Pony._State == SpriteMovementState::JUMPING) {
        frameWidth = _Pony._JumpingSpriteSheetWidth / PONY_JUMPING_FRAMES;
        sourceRect.left = static_cast<int32_t>(frameWidth * _Pony._CurrentFrame);
        sourceRect.top = 0;
        sourceRect.right = static_cast<int32_t>(sourceRect.left + frameWidth);
        sourceRect.bottom = static_cast<int32_t>(_Pony._JumpingSpriteSheetHeight);
        ponyTexture = _PonyJumpingTile.Get();
    } else if (_Pony._State == SpriteMovementState::RUNNING) {
        frameWidth = _Pony._RunningSpriteSheetWidth / PONY_RUNNING_FRAMES;
        sourceRect.left = static_cast<int32_t>(frameWidth * _Pony._CurrentFrame);
        sourceRect.top = 0;
        sourceRect.right = static_cast<int32_t>(sourceRect.left + frameWidth);
        sourceRect.bottom = static_cast<int32_t>(_Pony._RunningSpriteSheetHeight);
        ponyTexture = _PonyRunningTile.Get();
    } else {
        frameWidth = _Pony._IdleSpriteSheetWidth / PONY_IDLE_FRAMES;
        sourceRect.left = static_cast<int32_t>(frameWidth * _Pony._CurrentFrame);
        sourceRect.top = 0;
        sourceRect.right = static_cast<int32_t>(sourceRect.left + frameWidth);
        sourceRect.bottom = static_cast<int32_t>(_Pony._IdleSpriteSheetHeight);
        ponyTexture = _PonyIdleTile.Get();
    }

    if (ponyTexture != nullptr) {
        const float ROTATION = 0.f;
        const float SCALE = 1.f;
        const DirectX::SpriteEffects PONY_TRANSFORM = (_Pony._Facing == RIGHT) ? DirectX::SpriteEffects_FlipHorizontally : DirectX::SpriteEffects_None;
        const float LAYER_DEPTH = 0.f;
        _SpriteBatch->Draw(ponyTexture,
            _Pony._Location,
            &sourceRect,
            Colors::White,
            ROTATION,
            _OriginLocation,
            SCALE,
            PONY_TRANSFORM,
            LAYER_DEPTH);
    }
    _SpriteBatch->End();
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

    static const D3D_FEATURE_LEVEL featureLevels[]{
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
        nullptr,                            // specify nullptr to use the default adapter
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        creationFlags,
        featureLevels,
        _countof(featureLevels),
        D3D11_SDK_VERSION,
        device.ReleaseAndGetAddressOf(),    // returns the Direct3D device created
        &_FeatureLevel,                    // returns feature level of device created
        context.ReleaseAndGetAddressOf()    // returns the device immediate context
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

    // Background Resources
    DX::ThrowIfFailed(CreateWICTextureFromFile(_D3dDevice.Get(), L"assets/grass_tile.png",
        nullptr, _GrassTile.ReleaseAndGetAddressOf()));

    // Pony Idle Resources
    ComPtr<ID3D11Resource> ponyIdleResource;
    hr = CreateDDSTextureFromFile(_D3dDevice.Get(), FILE_PATH_SPRITE_PONY_IDLE,
        ponyIdleResource.GetAddressOf(), _PonyIdleTile.ReleaseAndGetAddressOf());
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
        ponyRunResource.GetAddressOf(), _PonyRunningTile.ReleaseAndGetAddressOf());
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
        ponyJumpResource.GetAddressOf(), _PonyJumpingTile.ReleaseAndGetAddressOf());
    DX::ThrowIfFailed(hr);

    ComPtr<ID3D11Texture2D> ponyJumpTexture;
    DX::ThrowIfFailed(ponyJumpResource.As(&ponyJumpTexture));

    CD3D11_TEXTURE2D_DESC ponyJumpDesc;
    ponyJumpTexture->GetDesc(&ponyJumpDesc);

    // Get total sprite sheet size
    _Pony._JumpingSpriteSheetWidth = ponyJumpDesc.Width;
    _Pony._JumpingSpriteSheetHeight = ponyJumpDesc.Height;

    _TotalElapsedSec = 0.f;

    // Pony location and orientation
    _OriginLocation.x = static_cast<float>(0);
    _OriginLocation.y = static_cast<float>(0);
    _Pony._Facing = SpriteFacingEnum::RIGHT;
    _Pony._State = SpriteMovementState::IDLE;
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
    _PonyIdleTile.Reset();
    _PonyRunningTile.Reset();
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