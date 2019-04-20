#pragma once

#include <time.h>
#include "Mem.h"
#include "GameMap.h"
#include "Shape.h"

typedef enum _GameStatus
{
	GAMESTATUS_PAUSE,
	GAMESTATUS_RUNNING,
	GAMESTATUS_OVER
}GameStatus;

typedef struct _GameController
{
//private:
	GameStatus gameStatus; //��Ϸ״̬
	GameMap gameMap; //��Ϸ��ͼ
	Shape shapes[7]; //������״
	Shape currentShape; //��ǰ����
	int score; //��Ϸ�÷�
}GameController, *PGameController;

//initialize and recycle
void InitGameController(PGameController pGameController, int row, int col); //��ʼ��
void FreeGameController(PGameController pGameController); //�ͷ���Դ

//getter and setter
GameStatus GetGameStatus(const PGameController pGameController); //��ȡ��ǰ��Ϸ״̬
const PGameMap GetGameMap(const PGameController pGameController); //��ȡ��Ϸ��ͼ
const PShape GetCurrentShape(const PGameController pGameController); //��ȡ��ǰ��״
int GetGameScore(const PGameController pGameController); //��ȡ����
int GetMapRow(const PGameController pGameController); //��ȡ��ͼ����
int GetMapCol(const PGameController pGameController); //��ȡ��ͼ����
void SetGameStatus(PGameController pGameController, GameStatus status); //���õ�ǰ��Ϸ״̬
void SetGameScore(PGameController pGameController, int score); //������Ϸ����

//operator
void Start(PGameController pGameController); //��ʼ��Ϸ
void Pause(PGameController pGameController); //��ͣ��Ϸ
void Run(PGameController pGameController); //������Ϸ
void RandomCurrentShape(PGameController pGameController); //������ɵ�ǰ��״
void RotateCurrentShape(PGameController pGameController); //��ת��ǰ��״
bool IsCurrentShapeReachBottom(const PGameController pGameController); //�жϵ�ǰ��״�Ƿ񴥵�
void CurrentShapeLeft(PGameController pGameController); //��ǰ��״����
void CurrentShapeRight(PGameController pGameController); //��ǰ��״����
void CurrentShapeDown(PGameController pGameController); //��ǰ��״����
void CurrentShapeFall(PGameController pGameController); //��ǰ��״��������
bool IsGameOver(PGameController pGameController); //�ж���Ϸ�Ƿ����
Shape GetShapePreview(PGameController pGameController); //��ȡ��״Ԥ��