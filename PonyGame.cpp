#include <d2d1.h>
#include <d2d1_1.h>
#include "PonyGame.h"

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

using namespace ParticleHomeEntertainment;

#pragma comment(lib, "d2d1.lib")

PonyGame::PonyGame() {
    _Window = nullptr;
    this->pCornflowerBlueBrush = nullptr;
    this->pDirect2dFactory = nullptr;
    this->pLightSlateGrayBrush = nullptr;
    this->pRenderTarget = nullptr;
}

void PonyGame::Start() {
    ID2D1Factory* pD2DFactory = NULL;
    HRESULT hr = D2D1CreateFactory(
        D2D1_FACTORY_TYPE_SINGLE_THREADED,
        &pD2DFactory
    );

    // Obtain the size of the drawing area.
    RECT rc;
    GetClientRect(_Window, &rc);

    // Create a Direct2D render target          
    ID2D1HwndRenderTarget* pRT = NULL;
    hr = pD2DFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(
            _Window,
            D2D1::SizeU(
                rc.right - rc.left,
                rc.bottom - rc.top)
        ),
        &pRT
    );

    ID2D1SolidColorBrush* pBlackBrush = NULL;
    if (SUCCEEDED(hr)) {

        pRT->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::Black),
            &pBlackBrush
        );
    }

    pRT->BeginDraw();

    pRT->DrawRectangle(
        D2D1::RectF(
            rc.left + 100.0f,
            rc.top + 100.0f,
            rc.right - 100.0f,
            rc.bottom - 100.0f),
        pBlackBrush);

    hr = pRT->EndDraw();

    SafeRelease(&pRT);
    
    SafeRelease(&pBlackBrush);

    SafeRelease(&pD2DFactory);
}