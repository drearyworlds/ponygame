#pragma once

#include "Pony.h"
#include "StepTimer.h"
#include "LevelScreen.h"
#include "InputState.h"
#include <nowarn/SimpleMath.h>
#include <nowarn/SpriteBatch.h>
#include <nowarn/Mouse.h>
#include <nowarn/wrl/client.h>
#include <nowarn/algorithm>
#include <memory>

namespace ParticleHomeEntertainment {
    enum GameStateEnum {
        TITLE_SCREEN, IN_LEVEL, PAUSED, CREDITS
    };

    class PonyGame {
    private:
        PonyGame() noexcept;

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
        BackgroundTile _GrassTile;
        BackgroundTile _SkyTileNw, _SkyTileNe, _SkyTileSw, _SkyTileSe;
        BackgroundTile _MoonTileNw, _MoonTileNe, _MoonTileSw, _MoonTileSe;
        std::unique_ptr<DirectX::SpriteBatch> _BackgroundSpriteBatch;
        LevelScreen _CurrentScreen;

        // Sprites
        std::shared_ptr<DirectX::SpriteBatch> _SpriteBatch;

        GameStateEnum _GameState = TITLE_SCREEN;
        uint32_t _CurrentLevel;
        LevelScreen _CurrentLevelScreen;

        // Other
        DirectX::SimpleMath::Vector2 _OriginLocationPx;

        Pony _Pony;
        //Pony _ZombiePony;

        // InputState
        InputState _InputState;

        void CreateDevice();

        void InitializeTextures();

        void CreateResources();

        void RenderScene();

        /// <summary>Helper method to clear the back buffers.</summary>
        void Clear();

        void DrawBackground();

        void HighlightCollisionTiles();

        void DrawObstacles();

        void DrawEnemies();

        void DrawPony();

        void DrawPonyBoundingBox();

        void Present();

        void OnDeviceLost();

    public:
        PonyGame(PonyGame& copy) = delete;
        PonyGame& operator=(PonyGame& copy) = delete;

        static PonyGame& Instance();

        // Initialization and management
        void Initialize(HWND window, int width, int height);

        void LoadLevel(uint32_t level);

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

        const LevelScreen& GetScreen() const;

        InputState& GetInputState();
    };
}