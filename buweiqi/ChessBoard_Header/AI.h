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
	SITUATION NowStatus;//当前盘面状况
	int CurrentRound;//当前回合数
	int CurrentNull;//空位数量（备用）

	virtual void GetPosition(int &line, int &column, int onTurn) = 0;
	/*
		参数line：下棋的位置（行）
		参数column：下棋的位置（列）
		参数None：是否没有一模一样的记录
	*/
	bool MatchMemory(int line, int column, std::set<int> &res);//匹配记忆
	/*
		参数line：下棋的位置（行）
		参数column：下棋的位置（列）
	*/
	void Statistic(int line, int column);//统计棋盘各个区域的棋子数量
	/*
	    参数maxQuadrant：最大的棋子数
	*/
	void GetCurrentStatus(int maxQuadrant, SITUATION &CurrentStatus);//获取当前盘面压缩数据
	/*
	    参数site：模拟下棋位置
	*/
	double ProbabilityCount(int site);//计算概率
	/*
		参数line：下棋的位置（行）
		参数column：下棋的位置（列）
	*/
	void BackQua(int line, int column)//统计棋盘各个区域的棋子数量
	{
		if (line < 5 && column > 5) Qua.FirstQuadrant--;
		else if (line < 5 && column < 5) Qua.SecondQuadrant--;
		else if (line > 5 && column < 5) Qua.ThirdQuadrant--;
		else if (line > 5 && column > 5) Qua.ForthQuadrant--;
	}
};

#endif // AI_H_INCLUDED
