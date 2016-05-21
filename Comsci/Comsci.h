#pragma once

#include "Resource.h"
//Windows Header Files:
#include <windows.h>
#include <windowsx.h>

//C RunTIme Header FIles:
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <wchar.h>
#include <math.h>

#pragma comment(lib, "d2d1")
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include "lodepng.h"
#include "texpath.h"
#include "Game.h"
#include "GameObject.h"
#include "ObjectCode.h"
#include "Position.h"

#define SPRITE_DIM 25
#define TARGET_FRAMERATE 60
#define GAMEBOARD_ORIGIN_X 0
#define GAMEBOARD_ORIGIN_Y 100
void CALLBACK RedrawTimerProc(HWND timerHwnd, UINT msg, UINT_PTR timerId, DWORD dwTime);

template <class Interface>
inline void SafeRelease(
	Interface **ppInterfaceToRelease
	)
{
	if (*ppInterfaceToRelease != NULL)
	{
		(*ppInterfaceToRelease)->Release();
		(*ppInterfaceToRelease) = NULL;
	}
}

#ifndef Assert
#if defined( DEBUG ) || defined( _DEBUG )
#define Assert(b) do {if (!(b)) {OutputDebugStringA("Assert: " #b "\n");}} while(0)
#else
#define Assert(b)
#endif //DEBUG || _DEBUG
#endif


#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif


class Comsci
{
public:
	Comsci();
	~Comsci();

	HRESULT Initialize();

	void RunMessageLoop();


private:
	HRESULT CreateDeviceIndependentResources();

	HRESULT CreateDeviceResources();

	void DiscardDeviceResources();

	HRESULT OnRender();

	void OnResize(
		UINT width,
		UINT height
		);

	static LRESULT CALLBACK WndProc(
		HWND hWnd,
		UINT message,
		WPARAM wParam,
		LPARAM lParam
		);
	void RenderSprite(UINT left, UINT top, ObjectCode spriteId);

	HWND m_hwnd;
	ID2D1Factory* m_pDirect2dFactory;
	ID2D1HwndRenderTarget* m_pRenderTarget;
	ID2D1SolidColorBrush* m_pLightSlateGrayBrush;
	ID2D1SolidColorBrush* m_pCornflowerBlueBrush;
	unsigned char* m_pRawImageData;
	ID2D1Bitmap* m_pSpriteSheet;
	unsigned int m_spriteSheetWidth, m_spriteSheetHeight;

    Game* game;
    HANDLE gameTextHandle;
};

Comsci::Comsci() :
    m_hwnd(NULL),
    m_pDirect2dFactory(NULL),
    m_pRenderTarget(NULL),
    m_pLightSlateGrayBrush(NULL),
    m_pCornflowerBlueBrush(NULL),
    m_pSpriteSheet(NULL)
{
    game = new Game(3, &DefaultInputFunc);
    gameTextHandle = OpenEvent(SYNCHRONIZE | EVENT_MODIFY_STATE, false, TEXT_HANDLE_NAME);
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&GameThreadEntryProc, game, 0, NULL);
}

Comsci::~Comsci()
{
	SafeRelease(&m_pDirect2dFactory);
	SafeRelease(&m_pRenderTarget);
	SafeRelease(&m_pLightSlateGrayBrush);
	SafeRelease(&m_pCornflowerBlueBrush);
	SafeRelease(&m_pSpriteSheet);
    delete game;
    game = nullptr;
}

void Comsci::RunMessageLoop()
{
	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

HRESULT Comsci::Initialize()
{
	HRESULT hr;

	hr = CreateDeviceIndependentResources();

	if (SUCCEEDED(hr)) 
	{
		//Register the window class
		WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = Comsci::WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = sizeof(LONG_PTR);
		wcex.hInstance = HINST_THISCOMPONENT;
		wcex.hbrBackground = NULL;
		wcex.lpszMenuName = /*MAKEINTRESOURCE(IDC_COMSCI)*/ NULL;
		wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
		wcex.lpszClassName = L"D2DComsci";

		RegisterClassEx(&wcex);

		FLOAT dpiX, dpiY;
		m_pDirect2dFactory->GetDesktopDpi(&dpiX, &dpiY);

		m_hwnd = CreateWindow(
			L"D2DComsci",
			L"Comsci",
			WS_OVERLAPPED,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			static_cast<UINT>(ceil(1680.f * dpiX / 96.f)),
			static_cast<UINT>(ceil(1050.f * dpiY / 96.f)),
			NULL,
			NULL,
			HINST_THISCOMPONENT,
			this
			);
		hr = m_hwnd ? S_OK : E_FAIL;
		if (SUCCEEDED(hr))
		{
			ShowWindow(m_hwnd, SW_SHOWNORMAL);
			UpdateWindow(m_hwnd);
		}
        SetTimer(m_hwnd, 0, 1000 / TARGET_FRAMERATE, &RedrawTimerProc);
	}

	return hr;
}

HRESULT Comsci::CreateDeviceIndependentResources()
{
	HRESULT hr = S_OK;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);
	lodepng_decode32_file(&m_pRawImageData, &m_spriteSheetWidth, &m_spriteSheetHeight, TEXTURES_DIR "spritesheet.png");
	for (unsigned int i = 0; i < (4 * m_spriteSheetWidth * m_spriteSheetHeight); i += 4)
	{
        /*
        From: RGBA
        To:   BGRA
        */
        unsigned char tmp = m_pRawImageData[i];
        m_pRawImageData[i] = m_pRawImageData[i + 2];
        m_pRawImageData[i + 2] = tmp;

		m_pRawImageData[i + 0] = (char)((double)m_pRawImageData[i + 3] * m_pRawImageData[i + 0] / 255);
		m_pRawImageData[i + 1] = (char)((double)m_pRawImageData[i + 3] * m_pRawImageData[i + 1] / 255);
		m_pRawImageData[i + 2] = (char)((double)m_pRawImageData[i + 3] * m_pRawImageData[i + 2] / 255);
	}

	return hr;
}

HRESULT Comsci::CreateDeviceResources()
{
	HRESULT hr = S_OK;
	if (!m_pRenderTarget)
	{
		RECT rc;
		GetClientRect(m_hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(
			rc.right - rc.left,
			rc.bottom - rc.top
			);

		hr = m_pDirect2dFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hwnd, size),
			&m_pRenderTarget
			);

		if (SUCCEEDED(hr))
		{
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::LightSlateGray),
				&m_pLightSlateGrayBrush
				);
		}
		if (SUCCEEDED(hr))
		{
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::CornflowerBlue),
				&m_pCornflowerBlueBrush
				);
		}
		if (SUCCEEDED(hr))
		{
			D2D1_SIZE_U spriteSheetSize = D2D1::SizeU(m_spriteSheetWidth, m_spriteSheetHeight);
			D2D1_PIXEL_FORMAT pFormat = D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED);
			D2D1_BITMAP_PROPERTIES bitmapProps = D2D1::BitmapProperties(pFormat);
			hr = m_pRenderTarget->CreateBitmap(spriteSheetSize, m_pRawImageData, m_spriteSheetWidth * 4, bitmapProps, &m_pSpriteSheet);
		}
	}

	return hr;
}

void Comsci::DiscardDeviceResources()
{
	SafeRelease(&m_pRenderTarget);
	SafeRelease(&m_pLightSlateGrayBrush);
	SafeRelease(&m_pCornflowerBlueBrush);
}

void Comsci::OnResize(UINT width, UINT height)
{
	if (m_pRenderTarget)
	{
		m_pRenderTarget->Resize(D2D1::SizeU(width, height));
	}
}
