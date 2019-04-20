#include "Graph.h"

//正常颜色
static COLORREF colors[] = { RGB(239, 83, 80), RGB(239, 83, 80), RGB(239, 108, 0), RGB(239, 108, 0), RGB(77, 182, 172), RGB(118, 204, 222), RGB(253, 216, 53) };

//半透明颜色
static COLORREF colors_op[] = { RGB(240, 209, 209), RGB(240, 209, 209), RGB(240, 214, 193), RGB(240, 214, 193), RGB(208, 229, 227), RGB(216, 233, 237), RGB(243, 235, 203) };

void ClearHdc(HDC hdc, int cxClient, int cyClient)
{
	HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
	SelectObject(hdc, hBrush);
	RECT rect = { 0, 0, cxClient, cyClient };
	FillRect(hdc, &rect, hBrush);
	DeleteObject(hBrush);
}

void DrawSquare(HDC hdc, int r, int c, int unit, COLORREF color)
{
	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	HBRUSH hBrush = CreateSolidBrush(color);
	SelectObject(hdc, hPen);
	SelectObject(hdc, hBrush);

	Rectangle(hdc, c * unit, r * unit, (c + 1) * unit + 1, (r + 1) * unit + 1);

	DeleteObject(hBrush);
	DeleteObject(hPen);
}

void DrawBorder(HDC hdc, PGameController pGame, int unit)
{
	int row = GetMapRow(pGame);
	int col = GetMapCol(pGame);

	for (int i = 0; i < col + 2; ++i)
	{
		DrawSquare(hdc, 0, i, unit, RGB(161, 136, 127));
		DrawSquare(hdc, row + 1, i, unit, RGB(161, 136, 127));
	}

	for (int i = 0; i < row + 2; ++i)
	{
		DrawSquare(hdc, i, 0, unit, RGB(161, 136, 127));
		DrawSquare(hdc, i, col + 1, unit, RGB(161, 136, 127));
	}
}

void DrawGameMap(HDC hdc, PGameController pGame, int unit)
{
	int row = GetMapRow(pGame);
	int col = GetMapCol(pGame);

	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			if (GetGameMapBlockState(GetGameMap(pGame), i, j) == STATE_OCCUPY)
			{
				DrawSquare(hdc, i + 1, j + 1, unit, RGB(120, 144, 156));
			}
		}
	}
}

void DrawCurrentShape(HDC hdc, PGameController pGame, int unit)
{
	PShape pShape = GetCurrentShape(pGame);

	for (int i = 0; i < 4; ++i)
	{
		DrawSquare(hdc, GetShapeBlockGlobalRow(pShape, i) + 1, GetShapeBlockGlobalCol(pShape, i) + 1, unit, colors[(int)GetShapeType(pShape)]);
	}
}

void DrawShapePreview(HDC hdc, PGameController pGame, int unit)
{
	Shape preview = GetShapePreview(pGame);

	for (int i = 0; i < 4; ++i)
	{
		DrawSquare(hdc, GetShapeBlockGlobalRow(&preview, i) + 1, GetShapeBlockGlobalCol(&preview, i) + 1, unit, colors_op[(int)GetShapeType(&preview)]);
	}
}

void DrawGame(HWND hwnd, HDC hdc, int cxClient, int cyClient, PGameController pGame, int unit)
{
	ClearHdc(hdc, cxClient, cyClient);

	DrawBorder(hdc, pGame, unit);
	DrawGameMap(hdc, pGame, unit);
	DrawCurrentShape(hdc, pGame, unit);
	DrawShapePreview(hdc, pGame, unit);

	InvalidateRect(hwnd, NULL, FALSE);
}
