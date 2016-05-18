// Comsci.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "Comsci.h"

#include "ObjectCode.h"
#include "Game.h"
#include "Position.h"


#define MAX_LOADSTRING 100

int WINAPI WinMain(
	HINSTANCE /* hInstance */,
	HINSTANCE /* hPrevInstance */,
	LPSTR /* lpCmdLine */,
	int /* nCmdShow */
	)

{
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

	if (SUCCEEDED(CoInitialize(NULL)))
	{
		{
			Comsci app;

			if (SUCCEEDED(app.Initialize()))
			{
				app.RunMessageLoop();
			}
		}
		CoUninitialize();
	}

	return 0;
}


void Comsci::RenderSprite(UINT left, UINT top, ObjectCode spriteId)
{
	D2D1_RECT_F sprite = D2D1::RectF(left, top, left + SPRITE_DIM, top + SPRITE_DIM);
	D2D1_RECT_F source = D2D1::RectF(spriteId * SPRITE_DIM, 0, SPRITE_DIM * (spriteId + 1), SPRITE_DIM);
	m_pRenderTarget->DrawBitmap(m_pSpriteSheet, sprite, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, source);
}


HRESULT Comsci::OnRender()
{
	HRESULT hr = S_OK;
	hr = CreateDeviceResources();

	if (SUCCEEDED(hr))
	{
		m_pRenderTarget->BeginDraw();
		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::BlanchedAlmond));

		D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();

		int width = static_cast<int>(rtSize.width);
		int height = static_cast<int>(rtSize.height);
		/*
		for (int x = 0; x < width; x += 10)
		{
			m_pRenderTarget->DrawLine(
				D2D1::Point2F(static_cast<FLOAT>(x), 0.0f),
				D2D1::Point2F(static_cast<FLOAT>(x), rtSize.height),
				m_pLightSlateGrayBrush,
				0.5f
				);
		}

		for (int y = 0; y < height; y += 10)
		{
			m_pRenderTarget->DrawLine(
				D2D1::Point2F(0.0f, static_cast<FLOAT>(y)),
				D2D1::Point2F(rtSize.width, static_cast<FLOAT>(y)),
				m_pLightSlateGrayBrush,
				0.5f
				);
		}
		*/
		D2D1_RECT_F screen = D2D1::RectF(0, 0, rtSize.width, rtSize.height);
        /*
		D2D1_RECT_F rectangle1 = D2D1::RectF(
			rtSize.width / 2 - 50.0f,
			rtSize.height / 2 - 50.0f,
			rtSize.width / 2 + 50.0f,
			rtSize.height / 2 + 50.0f
			);

		D2D1_RECT_F rectangle2 = D2D1::RectF(
			rtSize.width / 2 - 100.0f,
			rtSize.height / 2 - 100.0f,
			rtSize.width / 2 + 100.0f,
			rtSize.height / 2 + 100.0f
			);
        */
		//m_pRenderTarget->FillRectangle(&rectangle1, m_pLightSlateGrayBrush);

		//m_pRenderTarget->DrawRectangle(&rectangle2, m_pCornflowerBlueBrush);
        
		RenderSprite(0, 0, ObjectCode::WALL_MARIO);
        RenderSprite(0, 0, ObjectCode::INDICATOR_RED);
        RenderSprite(50, 50, ObjectCode::INDICATOR_GREEN);

		hr = m_pRenderTarget->EndDraw();

	}

	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardDeviceResources();
	}

	return hr;
}

LRESULT CALLBACK Comsci::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	if (message == WM_CREATE)
	{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		Comsci *pComsci = (Comsci *)pcs->lpCreateParams;

		::SetWindowLongPtrW(
			hwnd,
			GWLP_USERDATA,
			PtrToUlong(pComsci)
			);

		result = 1;
	}
	else
	{
		Comsci *pComsci = reinterpret_cast<Comsci *>(static_cast<LONG_PTR>(
			::GetWindowLongPtrW(
				hwnd,
				GWLP_USERDATA
				)));

		bool wasHandled = false;

		if (pComsci)
		{
			switch (message)
			{
			case WM_SIZE:
			{
				UINT width = LOWORD(lParam);
				UINT height = HIWORD(lParam);
				pComsci->OnResize(width, height);
			}
			result = 0;
			wasHandled = true;
			break;

			case WM_DISPLAYCHANGE:
			{
				InvalidateRect(hwnd, NULL, FALSE);
			}
			result = 0;
			wasHandled = true;
			break;

			case WM_PAINT:
			{
				pComsci->OnRender();
				ValidateRect(hwnd, NULL);
			}
			result = 0;
			wasHandled = true;
			break;

			case WM_DESTROY:
			{
				PostQuitMessage(0);
			}
			result = 1;
			wasHandled = true;
			break;
            case WM_LBUTTONDOWN:
            {
                UINT xCoord = GET_X_LPARAM(lParam);
                UINT yCoord = GET_Y_LPARAM(lParam);
                Position pos = { xCoord / SPRITE_DIM, yCoord / SPRITE_DIM };
                pComsci->game->MaybeSendPosition(pos);
            }
            result = 0;
            wasHandled = true;
            break;
			}
		}

		if (!wasHandled)
		{
			result = DefWindowProc(hwnd, message, wParam, lParam);
		}
	}

	return result;
}
