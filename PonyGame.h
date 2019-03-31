#pragma once

// Windows Header Files:
#include <windows.h>

// C RunTime Header Files:
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <wchar.h>
#include <math.h>

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>

#include <d2d1.h>
#include <d2d1_1.h>

namespace ParticleHomeEntertainment {
    class PonyGame {
    private:
        HWND _Window;
        ID2D1Factory* pDirect2dFactory;
        ID2D1HwndRenderTarget* pRenderTarget;
        ID2D1SolidColorBrush* pLightSlateGrayBrush;
        ID2D1SolidColorBrush* pCornflowerBlueBrush;


        template<class Interface>
        inline void SafeRelease(Interface** ppInterfaceToRelease) {
            if (*ppInterfaceToRelease != nullptr) {
                (*ppInterfaceToRelease)->Release();
                (*ppInterfaceToRelease) = nullptr;
            }
        }

    public:
        PonyGame();

        void Start();
    };
}