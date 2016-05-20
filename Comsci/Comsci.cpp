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
    if (spriteId == ObjectCode::NONE)
        return;
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

        //static int i = 0;    
        //RenderSprite(25 * ((i++)%ObjectCode::MAX), 0, (ObjectCode)(i%(int)ObjectCode::MAX));
        //RenderSprite(100, 100, ObjectCode::PLAYER);

        // We should query the status of the level and render the contents
        // Pseudocode:
        // Get level map and dimensions.
        // For each layer, render each tile by getting the ObjectCode of each object
            unsigned int gameWidth = game->GetWidth();
        unsigned int gameHeight = game->GetHeight();
        // surfaces, then furnishings, then entities, then overlays
        for (unsigned int row = 0; row < gameHeight; row++)
        {
            for (unsigned int col = 0; col < gameWidth; col++)
            {
                RenderSprite(row * SPRITE_DIM, col * SPRITE_DIM, game->GetSurfaceAt(Position{ row, col })->getCode());
                RenderSprite(row * SPRITE_DIM, col * SPRITE_DIM, game->GetFurnishingAt(Position{ row, col })->getCode());
                RenderSprite(row * SPRITE_DIM, col * SPRITE_DIM, game->GetEntityAt(Position{ row, col })->getCode());
                RenderSprite(row * SPRITE_DIM, col * SPRITE_DIM, game->GetOverlayAt(Position{ row, col })->getCode());
            }
        }

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

void CALLBACK RedrawTimerProc(HWND timerHwnd, UINT msg, UINT_PTR timerId, DWORD dwTime)
{
    RedrawWindow(timerHwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}