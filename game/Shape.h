#pragma once

#include <stdio.h>
#include <assert.h>

//��״����
typedef enum _Type
{
	SHAPE_L,
	SHAPE_RL,
	SHAPE_Z,
	SHAPE_RZ,
	SHAPE_T,
	SHAPE_LINE,
	SHAPE_SQUARE
}Type;

typedef struct _Pos
{
	int r, c;
}Pos, *PPos;

typedef struct _Shape
{
//private:
	Type type; //����
	int gr, gc; //ȫ������
	Pos p[4]; //�ĸ�������������
}Shape, *PShape;

//initialize
void InitShape(PShape pShape, Type type); //��ʼ��

//setter and getter
void SetShapeType(PShape pShape, Type type); //������״����
void SetShapeGlobalPos(PShape pShape, int gr, int gc); //����ȫ������
void SetShapeBlockPos(PShape pShape, int index, int r, int c); //���÷�������
Type GetShapeType(const PShape pShape); //��ȡ��״����
int GetShapeGlobalRow(const PShape pShape); //��ȡȫ��������
int GetShapeGlobalCol(const PShape pShape); //��ȡȫ��������
int GetShapeBlockRow(const PShape pShape, int index); //��ȡ����������
int GetShapeBlockCol(const PShape pShape, int index); //��ȡ����������
int GetShapeBlockGlobalRow(const PShape pShape, int index); //��ȡ����������
int GetShapeBlockGlobalCol(const PShape pShape, int index); //��ȡ����������
int GetMinCol(const PShape pShape); //��ȡ������С������
int GetMaxCol(const PShape pShape); //��ȡ�������������
int GetMinRow(const PShape pShape); //��ȡ������С������
int GetMaxRow(const PShape pShape); //��ȡ�������������

//operation
void RotateShape(PShape pShape); //������ת90��
void MoveShapeLeft(PShape pShape, int step); //����
void MoveShapeRight(PShape pShape, int step); //����
void MoveShapeUp(PShape pShape, int step); //����
void MoveShapeDown(PShape pShape, int step); //����