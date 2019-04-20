#include "Shape.h"

void InitShape(PShape pShape, Type type)
{
	assert(pShape != NULL);

	//pShape->type = type;
	SetShapeType(pShape, type);

	switch (type)
	{
	case SHAPE_L:
		/*pShape->p[0].r = 0;
		pShape->p[0].c = 1;
		pShape->p[1].r = 1;
		pShape->p[1].c = 1;
		pShape->p[2].r = 2;
		pShape->p[2].c = 1;
		pShape->p[3].r = 2;
		pShape->p[3].c = 2;*/
		SetShapeBlockPos(pShape, 0, 0, 1);
		SetShapeBlockPos(pShape, 1, 1, 1);
		SetShapeBlockPos(pShape, 2, 2, 1);
		SetShapeBlockPos(pShape, 3, 2, 2);
		break;
	case SHAPE_RL:
		/*pShape->p[0].r = 0;
		pShape->p[0].c = 2;
		pShape->p[1].r = 1;
		pShape->p[1].c = 2;
		pShape->p[2].r = 2;
		pShape->p[2].c = 2;
		pShape->p[3].r = 2;
		pShape->p[3].c = 1;*/
		SetShapeBlockPos(pShape, 0, 0, 2);
		SetShapeBlockPos(pShape, 1, 1, 2);
		SetShapeBlockPos(pShape, 2, 2, 2);
		SetShapeBlockPos(pShape, 3, 2, 1);
		break;
	case SHAPE_Z:
		/*pShape->p[0].r = 1;
		pShape->p[0].c = 0;
		pShape->p[1].r = 1;
		pShape->p[1].c = 1;
		pShape->p[2].r = 2;
		pShape->p[2].c = 1;
		pShape->p[3].r = 2;
		pShape->p[3].c = 2;*/
		SetShapeBlockPos(pShape, 0, 1, 0);
		SetShapeBlockPos(pShape, 1, 1, 1);
		SetShapeBlockPos(pShape, 2, 2, 1);
		SetShapeBlockPos(pShape, 3, 2, 2);
		break;
	case SHAPE_RZ:
		/*pShape->p[0].r = 1;
		pShape->p[0].c = 3;
		pShape->p[1].r = 1;
		pShape->p[1].c = 2;
		pShape->p[2].r = 2;
		pShape->p[2].c = 2;
		pShape->p[3].r = 2;
		pShape->p[3].c = 1;*/
		SetShapeBlockPos(pShape, 0, 1, 3);
		SetShapeBlockPos(pShape, 1, 1, 2);
		SetShapeBlockPos(pShape, 2, 2, 2);
		SetShapeBlockPos(pShape, 3, 2, 1);
		break;
	case SHAPE_T:
		/*pShape->p[0].r = 1;
		pShape->p[0].c = 1;
		pShape->p[1].r = 1;
		pShape->p[1].c = 2;
		pShape->p[2].r = 1;
		pShape->p[2].c = 3;
		pShape->p[3].r = 2;
		pShape->p[3].c = 2;*/
		SetShapeBlockPos(pShape, 0, 1, 1);
		SetShapeBlockPos(pShape, 1, 1, 2);
		SetShapeBlockPos(pShape, 2, 1, 3);
		SetShapeBlockPos(pShape, 3, 2, 2);
		break;
	case SHAPE_LINE:
		/*pShape->p[0].r = 0;
		pShape->p[0].c = 1;
		pShape->p[1].r = 1;
		pShape->p[1].c = 1;
		pShape->p[2].r = 2;
		pShape->p[2].c = 1;
		pShape->p[3].r = 3;
		pShape->p[3].c = 1;*/
		SetShapeBlockPos(pShape, 0, 0, 1);
		SetShapeBlockPos(pShape, 1, 1, 1);
		SetShapeBlockPos(pShape, 2, 2, 1);
		SetShapeBlockPos(pShape, 3, 3, 1);
		break;
	case SHAPE_SQUARE:
		/*pShape->p[0].r = 1;
		pShape->p[0].c = 1;
		pShape->p[1].r = 1;
		pShape->p[1].c = 2;
		pShape->p[2].r = 2;
		pShape->p[2].c = 1;
		pShape->p[3].r = 2;
		pShape->p[3].c = 2;*/
		SetShapeBlockPos(pShape, 0, 1, 1);
		SetShapeBlockPos(pShape, 1, 1, 2);
		SetShapeBlockPos(pShape, 2, 2, 1);
		SetShapeBlockPos(pShape, 3, 2, 2);
		break;
	default:
		assert(1 < 0);
		break;
	}

	SetShapeGlobalPos(pShape, 0, 0);
}

void SetShapeGlobalPos(PShape pShape, int gr, int gc)
{
	assert(pShape != NULL);
	assert(gr >= 0);
	assert(gc >= 0);

	pShape->gr = gr;
	pShape->gc = gc;
}

/*Pos GetShapeGlobalPos(const PShape pShape)
{
	assert(pShape != NULL);

	Pos p = { pShape->gr, pShape->gc };
	return p;
}*/

void SetShapeType(PShape pShape, Type type)
{
	pShape->type = type;
}

Type GetShapeType(const PShape pShape)
{
	return pShape->type;
}

int GetShapeGlobalRow(const PShape pShape)
{
	return pShape->gr;
}

int GetShapeGlobalCol(const PShape pShape)
{
	return pShape->gc;
}

void SetShapeBlockPos(PShape pShape, int index, int r, int c)
{
	pShape->p[index].r = r;
	pShape->p[index].c = c;
}

int GetShapeBlockRow(const PShape pShape, int index)
{
	return pShape->p[index].r;
}

int GetShapeBlockCol(const PShape pShape, int index)
{
	return pShape->p[index].c;
}

int GetShapeBlockGlobalRow(const PShape pShape, int index)
{
	return pShape->gr + pShape->p[index].r;
}

int GetShapeBlockGlobalCol(const PShape pShape, int index)
{
	return pShape->gc + pShape->p[index].c;
}

//ÏòÓÒÐý×ª90¶È
void RotateShape(PShape pShape)
{
	assert(pShape != NULL);

	for (int i = 0; i < 4; ++i)
	{
		int t = pShape->p[i].r;
		pShape->p[i].r = pShape->p[i].c;
		pShape->p[i].c = 3 - t;
	}
}

void MoveShapeLeft(PShape pShape, int step)
{
	assert(pShape != NULL);

	pShape->gc -= step;
}

void MoveShapeRight(PShape pShape, int step)
{
	assert(pShape != NULL);

	pShape->gc += step;
}

void MoveShapeUp(PShape pShape, int step)
{
	assert(pShape != NULL);

	pShape->gr -= step;
}

void MoveShapeDown(PShape pShape, int step)
{
	assert(pShape != NULL);

	pShape->gr += step;
}

int GetMinCol(const PShape pShape)
{
	assert(pShape != NULL);

	int ret = 100;
	for (int i = 0; i < 4; ++i)
	{
		if (pShape->p[i].c < ret)
		{
			ret = pShape->p[i].c;
		}
	}

	return ret;
}

int GetMaxCol(const PShape pShape)
{
	assert(pShape != NULL);

	int ret = -1;
	for (int i = 0; i < 4; ++i)
	{
		if (pShape->p[i].c > ret)
		{
			ret = pShape->p[i].c;
		}
	}

	return ret;
}

int GetMinRow(const PShape pShape)
{
	assert(pShape != NULL);

	int ret = 100;
	for (int i = 0; i < 4; ++i)
	{
		if (pShape->p[i].r < ret)
		{
			ret = pShape->p[i].r;
		}
	}

	return ret;
}

int GetMaxRow(const PShape pShape)
{
	assert(pShape != NULL);

	int ret = -1;
	for (int i = 0; i < 4; ++i)
	{
		if (pShape->p[i].r > ret)
		{
			ret = pShape->p[i].r;
		}
	}

	return ret;
}