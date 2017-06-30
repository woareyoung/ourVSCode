#pragma once
#ifndef AI1_H_INCLUDED
#define AI1_H_INCLUDED

#include <time.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include "../chessBoard_Header/AI.h"
#include "PointStyle.h"
#include "../FileSystem_Header/FileSystem.h"
#include "../ChessBoard_Header/SIP.h"

///AI类
class AI1 : public AI, private PointStyle
{
public:
	AI1();//构造函数
	void GetPosition(int &line, int &column, int onTurn);//获取下棋位置
	void InitializeD();
private:
	int OT;//不想再想名字了，在GetPosition.cpp文件中使用

	//记录各交叉点的值，数组访问从“1”开始，访问顺序为“先行后列”，
	//“0”表示没有棋子，“1”表示黑子，“2”表示白子
	bool Cross[10][10];//遍历标识
	int MaxScorePosition;//最大分值的位置
	double PointStyle[18];//记录每种特殊点类型的分值
	double Score[10][10];//记录每个位置上的分值
	double MaxScore;//记录最大分值
	std::shared_ptr<NEXTPACE> np;//获取下一步的可能性的链表

	int GetNextPace(std::shared_ptr<NEXTPACE> np);
	///各功能函数 集中在FunctionPart.cpp文件中
	/*
		param[row1]:行
		param[row2]:列
		param[who]:玩家编号
		param[num]:周围对方棋子的数量
		return:方位编号
	*/
	int GetChessAmount(int row1, int row2, int who, int &num);//查看特定位置的四个方向有多少个棋子
	void GetMaxScorePosition();//获取最大分值的位置
	/*
		param[line]:刚下棋的行
		param[column]:刚下棋的列
	*/
	void MemoryData(int line, int column);//记录数据

	///更新位置分值所用函数 集中在UpdateScore.cpp文件中
	//更新棋盘位置分值，参数：行，列，是否加分
	void UpdateScore(int row1, int row2, int who, bool isAddScore = true);//调用RecordSpecialPoint()函数
	//设置特殊点影响的分值
	void ResetScore(int row1, int row2, int score, int NumberPoor = 100, bool isAddScore = true);//实行加分的函数
	//添加特殊点
	void RecordSpecialPoint(int row1, int row2, int who, bool isAddScore = true);//用SetCurrentPoint()设置当前点，再调用AddRecord
	void AddRecord(int n, int n1, int n2, int n3, int n4, int amo, bool isAddScore = true);
	void SetCurrentPoint(int ThisFormatStyle, int ThisFormatScorePoor, int ThisSpecialStyle, int ThisSpecialScorePoorint, int LastFormatStyle = 0, int LastFormatScorePoore = 0, int LastSpecialPointStyle = 0, int LastSpecialScorePoor = 0);

	///判断是否是死棋位置   集中在DeadCheck.cpp文件中
	bool DeadCheck(int line, int column, int who);
	bool CheckPosition(int OriLine, int OriColumn, int CLine, int CColumn, int player, bool &Position);
	bool Besieg(int RivalLine, int RivalColumn, int player, int rival);
	bool BesiegePosition(int RivalLine, int RivalColumn, int player, int rival, bool &tie);

	///在RateResetScore.cpp文件中
	void RateResetScore(float ResetRate, bool add = true);//按比例缩小分值，减少分差，精确定位
};


#endif // AI1_H_INCLUDED
