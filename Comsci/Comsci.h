#pragma once

#include "Resource.h"
//Windows Header Files:
#include <windows.h>

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
	HWND m_hwnd;
	ID2D1Factory* m_pDirect2dFactory;
	ID2D1HwndRenderTarget* m_pRenderTarget;
	ID2D1SolidColorBrush* m_pLightSlateGrayBrush;
	ID2D1SolidColorBrush* m_pCornflowerBlueBrush;
	unsigned char* m_pRawImageData;
	ID2D1Bitmap* m_pSpriteSheet;
};

Comsci::Comsci() :
	m_hwnd(NULL),
	m_pDirect2dFactory(NULL),
	m_pRenderTarget(NULL),
	m_pLightSlateGrayBrush(NULL),
	m_pCornflowerBlueBrush(NULL),
	m_pSpriteSheet(NULL)
{
}

Comsci::~Comsci()
{
	SafeRelease(&m_pDirect2dFactory);
	SafeRelease(&m_pRenderTarget);
	SafeRelease(&m_pLightSlateGrayBrush);
	SafeRelease(&m_pCornflowerBlueBrush);
	SafeRelease(&m_pSpriteSheet);
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
		wcex.lpszMenuName = NULL;
		wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
		wcex.lpszClassName = L"D2DComsci";

		RegisterClassEx(&wcex);

		FLOAT dpiX, dpiY;
		m_pDirect2dFactory->GetDesktopDpi(&dpiX, &dpiY);

		m_hwnd = CreateWindow(
			L"D2DComsci",
			L"Direct2D Demo App",
			WS_OVERLAPPED,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			static_cast<UINT>(ceil(640.f * dpiX / 96.f)),
			static_cast<UINT>(ceil(480.f * dpiY / 96.f)),
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
	}

	return hr;
}

HRESULT Comsci::CreateDeviceIndependentResources()
{
	HRESULT hr = S_OK;
	unsigned int width, height;

	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);
	lodepng_decode32_file(&m_pRawImageData, &width, &height, "C:\\Users\\Reyn\\Documents\\Visual Studio 2015\\Projects\\Comsci\\Comsci\\textures\\BasicLogo.png");

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
