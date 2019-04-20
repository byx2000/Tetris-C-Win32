#pragma once

#include <stdio.h>
#include <assert.h>

//形状类型
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
	Type type; //类型
	int gr, gc; //全局坐标
	Pos p[4]; //四个方块的相对坐标
}Shape, *PShape;

//initialize
void InitShape(PShape pShape, Type type); //初始化

//setter and getter
void SetShapeType(PShape pShape, Type type); //设置形状类型
void SetShapeGlobalPos(PShape pShape, int gr, int gc); //设置全局坐标
void SetShapeBlockPos(PShape pShape, int index, int r, int c); //设置方块坐标
Type GetShapeType(const PShape pShape); //获取形状类型
int GetShapeGlobalRow(const PShape pShape); //获取全局行坐标
int GetShapeGlobalCol(const PShape pShape); //获取全局列坐标
int GetShapeBlockRow(const PShape pShape, int index); //获取方块行坐标
int GetShapeBlockCol(const PShape pShape, int index); //获取方块列坐标
int GetShapeBlockGlobalRow(const PShape pShape, int index); //获取方块行坐标
int GetShapeBlockGlobalCol(const PShape pShape, int index); //获取方块列坐标
int GetMinCol(const PShape pShape); //获取方块最小列坐标
int GetMaxCol(const PShape pShape); //获取方块最大列坐标
int GetMinRow(const PShape pShape); //获取方块最小行坐标
int GetMaxRow(const PShape pShape); //获取方块最大行坐标

//operation
void RotateShape(PShape pShape); //向右旋转90度
void MoveShapeLeft(PShape pShape, int step); //左移
void MoveShapeRight(PShape pShape, int step); //右移
void MoveShapeUp(PShape pShape, int step); //上移
void MoveShapeDown(PShape pShape, int step); //下移