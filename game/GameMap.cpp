#include "GameMap.h"

void InitGameMap(PGameMap pGameMap, int row, int col)
{
	assert(pGameMap != NULL);
	assert(row > 0);
	assert(col > 0);

	pGameMap->row = row;
	pGameMap->col = col;
	InitIntMatrix(&pGameMap->map, row, col);

	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			SetIntMatrixElem(&pGameMap->map, i, j, STATE_BLANK);
		}
	}
}

void FreeGameMap(PGameMap pGameMap)
{
	assert(pGameMap != NULL);

	FreeIntMatrix(&pGameMap->map);
}

int GetGameMapRow(const PGameMap pGameMap)
{
	return pGameMap->row;
}

int GetGameMapCol(const PGameMap pGameMap)
{
	return pGameMap->col;
}

BlockState GetGameMapBlockState(const PGameMap pGameMap, int r, int c)
{
	assert(pGameMap != NULL);
	assert(r >= 0 && r < pGameMap->row);
	assert(c >= 0 && c < pGameMap->col);

	return (BlockState)GetIntMatrixElem(&pGameMap->map, r, c);
}

void SetGameMapBlockState(PGameMap pGameMap, int r, int c, BlockState state)
{
	assert(pGameMap != NULL);
	assert(r >= 0 && r < pGameMap->row);
	assert(c >= 0 && c < pGameMap->col);

	SetIntMatrixElem(&pGameMap->map, r, c, state);
}

void ClearGameMap(PGameMap pGameMap)
{
	assert(pGameMap != NULL);

	for (int i = 0; i < pGameMap->row; ++i)
	{
		for (int j = 0; j < pGameMap->col; ++j)
		{
			SetGameMapBlockState(pGameMap, i, j, STATE_BLANK);
		}
	}
}

//消行算法
int UpdateGameMap(PGameMap pGameMap)
{
	assert(pGameMap != NULL);

	int row = pGameMap->row, col = pGameMap->col;

	int cLine = 0; //消除的行数
	for (int i = row - 1; i >= 0; --i)
	{
		bool flag = true;
		for (int j = 0; j < col; ++j)
		{
			if (GetGameMapBlockState(pGameMap, i, j) == STATE_BLANK)
			{
				flag = false;
				break;
			}
		}

		if (flag)
		{
			cLine++;

			for (int t = i; t >= 1; --t)
			{
				for (int j = 0; j < col; ++j)
				{
					SetGameMapBlockState(pGameMap, t, j, GetGameMapBlockState(pGameMap, t - 1, j));
				}
			}

			for (int t = 0; t < col; ++t)
			{
				SetGameMapBlockState(pGameMap, 0, t, STATE_BLANK);
			}

			i++;
		}
	}

	return cLine;
}
