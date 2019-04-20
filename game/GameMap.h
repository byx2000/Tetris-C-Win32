#pragma once

#include <stdio.h>
#include <assert.h>
#include "Mem.h"

typedef enum _BlockState //��������
{ 
	STATE_BLANK,  //�հ�
	STATE_OCCUPY  //ռ��
}BlockState;

typedef struct _GameMap
{
//private:
	int row, col; //��ͼ�ߴ�
	IntMatrix map; //��ͼ״̬
}GameMap, *PGameMap;

//initialize and recycle
void InitGameMap(PGameMap pGameMap, int row, int col); //��ʼ��
void FreeGameMap(PGameMap pGameMap); //�ͷ���Դ

//getter and setter
int GetGameMapRow(const PGameMap pGameMap); //��ȡ����
int GetGameMapCol(const PGameMap pGameMap); //��ȡ����
BlockState GetGameMapBlockState(const PGameMap pGameMap, int r, int c); //��ȡ����״̬
void SetGameMapBlockState(PGameMap pGameMap, int r, int c, BlockState state); //���÷���״̬

//operator
void ClearGameMap(PGameMap pGameMap); //�����Ϸ��ͼ
int UpdateGameMap(PGameMap pGameMap); //�����㷨