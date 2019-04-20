#pragma once

#include <Windows.h>
#include "../game/GameController.h"
#include "Debug.h"

void ClearHdc(HDC hdc, int cxClient, int cyClient); //清空绘图区域
void DrawSquare(HDC hdc, int r, int c, int unit, COLORREF color); //绘制方格
void DrawBorder(HDC hdc, PGameController pGameController, int unit); //绘制边框
void DrawGameMap(HDC hdc, PGameController pGame, int unit); //绘制游戏地图
void DrawCurrentShape(HDC hdc, PGameController pGame, int unit); //绘制当前形状
void DrawShapePreview(HDC hdc, PGameController pGame, int unit); //绘制形状预览图
void DrawGame(HWND hwnd, HDC hdc, int cxClient, int cyClient, PGameController pGame, int unit); //绘制所有游戏元素