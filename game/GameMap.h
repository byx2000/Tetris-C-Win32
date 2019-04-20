#pragma once

#include <stdio.h>
#include <assert.h>
#include "Mem.h"

typedef enum _BlockState //方块属性
{ 
	STATE_BLANK,  //空白
	STATE_OCCUPY  //占用
}BlockState;

typedef struct _GameMap
{
//private:
	int row, col; //地图尺寸
	IntMatrix map; //地图状态
}GameMap, *PGameMap;

//initialize and recycle
void InitGameMap(PGameMap pGameMap, int row, int col); //初始化
void FreeGameMap(PGameMap pGameMap); //释放资源

//getter and setter
int GetGameMapRow(const PGameMap pGameMap); //获取行数
int GetGameMapCol(const PGameMap pGameMap); //获取列数
BlockState GetGameMapBlockState(const PGameMap pGameMap, int r, int c); //获取方块状态
void SetGameMapBlockState(PGameMap pGameMap, int r, int c, BlockState state); //设置方块状态

//operator
void ClearGameMap(PGameMap pGameMap); //清空游戏地图
int UpdateGameMap(PGameMap pGameMap); //消行算法