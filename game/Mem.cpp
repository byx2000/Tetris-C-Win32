#include "Mem.h"

void InitIntMatrix(PIntMatrix pIntMatrix, int row, int col)
{
	assert(pIntMatrix != NULL);
	assert(row > 0);
	assert(col > 0);

	pIntMatrix->row = row;
	pIntMatrix->col = col;
	pIntMatrix->arr = (int*)malloc(sizeof(int) * row * col);

	assert(pIntMatrix->arr != NULL);
}

void SetIntMatrixElem(PIntMatrix pIntMatrix, int r, int c, int val)
{
	assert(pIntMatrix != NULL);
	assert(pIntMatrix->arr != NULL);
	assert(r >= 0 && r < pIntMatrix->row);
	assert(c >= 0 && c < pIntMatrix->col);

	*(pIntMatrix->arr + r * pIntMatrix->col + c) = val;
}

int GetIntMatrixElem(const PIntMatrix pIntMatrix, int r, int c)
{
	assert(pIntMatrix != NULL);
	assert(pIntMatrix->arr != NULL);
	assert(r >= 0 && r < pIntMatrix->row);
	assert(c >= 0 && c < pIntMatrix->col);

	return *(pIntMatrix->arr + r * pIntMatrix->col + c);
}

void FreeIntMatrix(PIntMatrix pIntMatrix)
{
	assert(pIntMatrix != NULL);
	assert(pIntMatrix->arr != NULL);

	free(pIntMatrix->arr);
	pIntMatrix->arr = NULL;
}

void FillIntMatrixElem(PIntMatrix pIntMatrix, int val)
{
	assert(pIntMatrix != NULL);
	assert(pIntMatrix->arr != NULL);

	for (int i = 0; i < pIntMatrix->row; ++i)
	{
		for (int j = 0; j < pIntMatrix->col; ++j)
		{
			SetIntMatrixElem(pIntMatrix, i, j, val);
		}
	}
}
