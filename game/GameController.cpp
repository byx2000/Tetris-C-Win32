#include "GameController.h"

void InitGameController(PGameController pGameController, int row, int col)
{
	assert(pGameController != NULL);
	assert(row > 0);
	assert(col > 0);

	pGameController->gameStatus = GAMESTATUS_PAUSE;

	//��ʼ����Ϸ��ͼ
	InitGameMap(&pGameController->gameMap, row, col);
	
	//��ʼ��������״
	for (int i = 0; i < 7; ++i)
	{
		InitShape(&pGameController->shapes[i], (Type)i);
	}

	//������ɵ�ǰ����
	srand((unsigned int)time(NULL));
	RandomCurrentShape(pGameController);

	//��Ϸ�÷�Ϊ0
	pGameController->score = 0;
}

void FreeGameController(PGameController pGameController)
{
	assert(pGameController != NULL);

	FreeGameMap(&pGameController->gameMap);
}

GameStatus GetGameStatus(const PGameController pGameController)
{
	return pGameController->gameStatus;
}

const PGameMap GetGameMap(const PGameController pGameController)
{
	return &pGameController->gameMap;
}

const PShape GetCurrentShape(const PGameController pGameController)
{
	return &pGameController->currentShape;
}

int GetGameScore(const PGameController pGameController)
{
	return pGameController->score;
}

void SetGameStatus(PGameController pGameController, GameStatus status)
{
	pGameController->gameStatus = status;
}

void SetGameScore(PGameController pGameController, int score)
{
	pGameController->score = score;
}

void Start(PGameController pGameController)
{
	pGameController->gameStatus = GAMESTATUS_RUNNING;
}

void Pause(PGameController pGameController)
{
	pGameController->gameStatus = GAMESTATUS_PAUSE;
}

int GetMapRow(const PGameController pGameController)
{
	return GetGameMapRow(&pGameController->gameMap);
}

int GetMapCol(const PGameController pGameController)
{
	return GetGameMapCol(&pGameController->gameMap);
}

//������ɵ�ǰ��״
void RandomCurrentShape(PGameController pGameController)
{
	assert(pGameController != NULL);

	//���ѡ����״
	Shape shape = pGameController->shapes[rand() % 7];

	//�����ת
	int t = rand() % 3;
	for (int i = 0; i < t; ++i)
	{
		RotateShape(&shape);
	}

	//���ָ��λ��
	SetShapeGlobalPos(&shape, 0, rand() % (GetGameMapCol(&pGameController->gameMap) - 4));

	//����λ��
	int minRow = GetMinRow(&shape);
	MoveShapeUp(&shape, minRow);

	pGameController->currentShape = shape;
}

//��ת��ǰ��״
void RotateCurrentShape(PGameController pGameController)
{
	assert(pGameController != NULL);

	if (pGameController->gameStatus != GAMESTATUS_RUNNING)
	{
		return;
	}

	if (IsCurrentShapeReachBottom(pGameController))
	{
		return;
	}

	PShape pShape = &pGameController->currentShape;
	int col = GetMapCol(pGameController);

	RotateShape(pShape);

	int minRow = GetMinRow(pShape);
	if (GetShapeGlobalRow(pShape) + minRow < 0)
	{
		MoveShapeDown(pShape, -(GetShapeGlobalRow(pShape) + minRow));
	}

	int minCol = GetMinCol(pShape);
	if (GetShapeGlobalCol(pShape) + minCol < 0)
	{
		MoveShapeRight(pShape, -(GetShapeGlobalCol(pShape) + minCol));
	}

	int maxCol = GetMaxCol(pShape);
	if (GetShapeGlobalCol(pShape) + maxCol > col - 1)
	{
		MoveShapeLeft(pShape, -(col - 1 - GetShapeGlobalCol(pShape) - maxCol));
	}

	while (IsCurrentShapeReachBottom(pGameController))
	{
		MoveShapeUp(pShape, 1);
	}
}

//�жϵ�ǰ��״�Ƿ񴥵�
bool IsCurrentShapeReachBottom(const PGameController pGameController)
{
	assert(pGameController != NULL);

	PShape pShape = &pGameController->currentShape;
	int row = GetMapRow(pGameController);

	for (int i = 0; i < 4; ++i)
	{
		int cRow = GetShapeBlockGlobalRow(pShape, i);
		int cCol = GetShapeBlockGlobalCol(pShape, i);

		if (cRow == row - 1)
		{
			return true;
		}

		if (GetGameMapBlockState(&pGameController->gameMap, cRow, cCol) == STATE_OCCUPY)
		{
			return true;
		}

		if (GetGameMapBlockState(&pGameController->gameMap, cRow + 1, cCol) == STATE_OCCUPY)
		{
			return true;
		}
	}

	return false;
}

//��ǰ��״����
void CurrentShapeLeft(PGameController pGameController)
{
	assert(pGameController != NULL);

	if (pGameController->gameStatus != GAMESTATUS_RUNNING)
	{
		return;
	}

	PShape pShape = &pGameController->currentShape;
	PGameMap pGameMap = &pGameController->gameMap;
	int row = GetGameMapRow(pGameMap);

	bool flag = true;
	for (int i = 0; i < 4; ++i)
	{
		int cRow = GetShapeBlockGlobalRow(pShape, i);
		int cCol = GetShapeBlockGlobalCol(pShape, i);
		if (cCol == 0
			|| GetGameMapBlockState(pGameMap, cRow, cCol - 1) == STATE_OCCUPY
			|| cRow == row - 1
			|| GetGameMapBlockState(pGameMap, cRow + 1, cCol) == STATE_OCCUPY)
		{
			flag = false;
			break;
		}
	}

	if (flag)
	{
		MoveShapeLeft(pShape, 1);
	}
}

//��ǰ��״����
void CurrentShapeRight(PGameController pGameController)
{
	assert(pGameController != NULL);

	if (pGameController->gameStatus != GAMESTATUS_RUNNING)
	{
		return;
	}

	PShape pShape = &pGameController->currentShape;
	PGameMap pGameMap = &pGameController->gameMap;
	int row = GetGameMapRow(pGameMap);
	int col = GetGameMapCol(pGameMap);

	bool flag = true;
	for (int i = 0; i < 4; ++i)
	{
		int cRow = GetShapeBlockGlobalRow(pShape, i);
		int cCol = GetShapeBlockGlobalCol(pShape, i);
		if (cCol == col - 1
			|| GetGameMapBlockState(pGameMap, cRow, cCol + 1) == STATE_OCCUPY
			|| cRow == row - 1
			|| GetGameMapBlockState(pGameMap, cRow + 1, cCol) == STATE_OCCUPY)
		{
			flag = false;
			break;
		}
	}

	if (flag)
	{
		MoveShapeRight(pShape, 1);
	}
}

//��ǰ��״����
void CurrentShapeDown(PGameController pGameController)
{
	assert(pGameController != NULL);

	if (pGameController->gameStatus != GAMESTATUS_RUNNING)
	{
		return;
	}

	PShape pShape = &pGameController->currentShape;
	if (!IsCurrentShapeReachBottom(pGameController))
	{
		MoveShapeDown(pShape, 1);
	}
}

//��ǰ��״��������
void CurrentShapeFall(PGameController pGameController)
{
	assert(pGameController != NULL);

	if (pGameController->gameStatus != GAMESTATUS_RUNNING)
	{
		return;
	}

	PShape pShape = &pGameController->currentShape;
	while (!IsCurrentShapeReachBottom(pGameController))
	{
		MoveShapeDown(pShape, 1);
	}
}

//������Ϸ
void Run(PGameController pGameController)
{
	assert(pGameController != NULL);

	if (pGameController->gameStatus != GAMESTATUS_RUNNING)
	{
		return;
	}

	PShape pShape = &pGameController->currentShape;
	PGameMap pGameMap = &pGameController->gameMap;

	//����Ƿ�ﵽ�ײ�
	if (IsCurrentShapeReachBottom(pGameController))
	{
		//������Ϸ��ͼ
		for (int i = 0; i < 4; ++i)
		{
			SetGameMapBlockState(pGameMap, GetShapeBlockGlobalRow(pShape, i), GetShapeBlockGlobalCol(pShape, i), STATE_OCCUPY);
		}

		//����
		int cLine = UpdateGameMap(pGameMap);

		//������Ϸ�÷�
		pGameController->score += cLine;

		//�������ɵ�ǰ��״
		RandomCurrentShape(pGameController);
	}
	else
	{
		//��ǰ��״����
		CurrentShapeDown(pGameController);
	}

	//��Ϸ����
	if (IsGameOver(pGameController))
	{
		pGameController->gameStatus = GAMESTATUS_OVER;
	}
}

bool IsGameOver(PGameController pGameController)
{
	assert(pGameController != NULL);

	PGameMap pGameMap = &pGameController->gameMap;
	int col = GetGameMapCol(pGameMap);

	for (int i = 0; i < col; ++i)
	{
		if (GetGameMapBlockState(pGameMap, 0, i) == STATE_OCCUPY)
		{
			return true;
		}
	}

	return false;
}

Shape GetShapePreview(PGameController pGameController)
{
	Shape restore = pGameController->currentShape;
	PShape pShape = &pGameController->currentShape;
	while (!IsCurrentShapeReachBottom(pGameController))
	{
		MoveShapeDown(pShape, 1);
	}
	Shape res = pGameController->currentShape;
	pGameController->currentShape = restore;
	return res;
}
