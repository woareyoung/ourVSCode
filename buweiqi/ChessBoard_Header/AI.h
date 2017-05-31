#pragma once
#ifndef AI_H_INCLUDED
#define AI_H_INCLUDED
#include "SIP.h"
#include "../FileSystem_Header/FileSystem.h"
#include <math.h>

class AI
{
public:
	//记录各交叉点的值，数组访问从“1”开始，访问顺序为“先行后列”，
	//“0”表示没有棋子，“1”表示黑子，“2”表示白子
	int cross[10][10];

	bool link;//标记是否已打开文件
	FileSystem FS; //文件操作对象
	QUADRANT Qua; //记录走棋纪录
	int CurrentRound;//当前回合数

	virtual void GetPosition(int &line, int &column, int onTurn) = 0;
private:
	int MatchMemory();//匹配记录，返回记录中的下一步
	/*
		参数player：调用该函数的玩家编号
	*/
	void GetChessBoardStatus(int player);//获取并统计棋盘状态
	/*
		参数line：下棋的位置（行）
		参数column：下棋的位置（列）
	*/
	void Statistic(int line, int column);//统计棋盘各个区域的棋子数量

};

#endif // AI_H_INCLUDED
