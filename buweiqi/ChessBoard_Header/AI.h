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
	int PlayerId;// 当前AI的身份是黑子还是白子

	FileSystem FS; //文件操作对象
	QUADRANT Qua; //记录走棋纪录
	int CurrentRound;//当前回合数

	virtual void GetPosition(int &line, int &column, int onTurn) = 0;
	/*
		参数line：下棋的位置（行）
		参数column：下棋的位置（列）
	*/
	NEXTPACE* MatchMemory(int line, int column);//匹配记忆
	/*
		参数line：下棋的位置（行）
		参数column：下棋的位置（列）
	*/
	void Statistic(int line, int column);//统计棋盘各个区域的棋子数量

};

#endif // AI_H_INCLUDED
