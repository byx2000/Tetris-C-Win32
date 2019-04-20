#pragma once

#include <stdlib.h>
#include <assert.h>

typedef struct _IntMatrix
{
	int* arr;
	int row, col;
}IntMatrix, *PIntMatrix;

void InitIntMatrix(PIntMatrix pIntMatrix, int row, int col);
void FreeIntMatrix(PIntMatrix pIntMatrix);
void FillIntMatrixElem(PIntMatrix pIntMatrix, int val);
void SetIntMatrixElem(PIntMatrix pIntMatrix, int r, int c, int val);
int GetIntMatrixElem(const PIntMatrix pIntMatrix, int r, int c);
