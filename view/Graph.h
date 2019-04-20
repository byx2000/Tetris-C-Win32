#pragma once

#include <Windows.h>
#include "../game/GameController.h"
#include "Debug.h"

void ClearHdc(HDC hdc, int cxClient, int cyClient); //��ջ�ͼ����
void DrawSquare(HDC hdc, int r, int c, int unit, COLORREF color); //���Ʒ���
void DrawBorder(HDC hdc, PGameController pGameController, int unit); //���Ʊ߿�
void DrawGameMap(HDC hdc, PGameController pGame, int unit); //������Ϸ��ͼ
void DrawCurrentShape(HDC hdc, PGameController pGame, int unit); //���Ƶ�ǰ��״
void DrawShapePreview(HDC hdc, PGameController pGame, int unit); //������״Ԥ��ͼ
void DrawGame(HWND hwnd, HDC hdc, int cxClient, int cyClient, PGameController pGame, int unit); //����������ϷԪ��