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
	GameStatus gameStatus; //游戏状态
	GameMap gameMap; //游戏地图
	Shape shapes[7]; //七种形状
	Shape currentShape; //当前方块
	int score; //游戏得分
}GameController, *PGameController;

//initialize and recycle
void InitGameController(PGameController pGameController, int row, int col); //初始化
void FreeGameController(PGameController pGameController); //释放资源

//getter and setter
GameStatus GetGameStatus(const PGameController pGameController); //获取当前游戏状态
const PGameMap GetGameMap(const PGameController pGameController); //获取游戏地图
const PShape GetCurrentShape(const PGameController pGameController); //获取当前形状
int GetGameScore(const PGameController pGameController); //获取分数
int GetMapRow(const PGameController pGameController); //获取地图行数
int GetMapCol(const PGameController pGameController); //获取地图列数
void SetGameStatus(PGameController pGameController, GameStatus status); //设置当前游戏状态
void SetGameScore(PGameController pGameController, int score); //设置游戏分数

//operator
void Start(PGameController pGameController); //开始游戏
void Pause(PGameController pGameController); //暂停游戏
void Run(PGameController pGameController); //运行游戏
void RandomCurrentShape(PGameController pGameController); //随机生成当前形状
void RotateCurrentShape(PGameController pGameController); //旋转当前形状
bool IsCurrentShapeReachBottom(const PGameController pGameController); //判断当前形状是否触底
void CurrentShapeLeft(PGameController pGameController); //当前形状左移
void CurrentShapeRight(PGameController pGameController); //当前形状右移
void CurrentShapeDown(PGameController pGameController); //当前形状下移
void CurrentShapeFall(PGameController pGameController); //当前形状快速下移
bool IsGameOver(PGameController pGameController); //判断游戏是否结束
Shape GetShapePreview(PGameController pGameController); //获取形状预览