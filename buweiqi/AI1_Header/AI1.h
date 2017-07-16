#pragma once
#ifndef AI1_H_INCLUDED
#define AI1_H_INCLUDED

#include <time.h>
#include <stdlib.h>
#include <conio.h>
#include "../chessBoard_Header/AI.h"
#include <stack>
#include <set>
#include "PointStyle.h"
#include "../FileSystem_Header/FileSystem.h"
#include "../ChessBoard_Header/SIP.h"
#include "AIForFile.h"

///AI类
class AI1 : public AI, private PointStyle, private AIForFile
{
public:
	AI1();//构造函数
	void GetPosition(int &line, int &column, int onTurn);//获取下棋位置
	void InitializeD();
private:
	int OT;//不想再想名字了，在GetPosition.cpp文件中使用
	int Similar;//与过去输的局面相似的程度
	//记录各交叉点的值，数组访问从“1”开始，访问顺序为“先行后列”，
	//“0”表示没有棋子，“1”表示黑子，“2”表示白子
	bool Cross[10][10];//遍历标识
	int DoubleDontDead;//双方都不死的位置数量
	int MaxScorePosition;//最大分值的位置
	double PointStyle[18];//记录每种特殊点类型的分值
	double Score[10][10];//记录每个位置上的分值
	double MaxScore;//记录最大分值
	
	std::set<int> np;//获取下一步的可能性的集合
	/*
		param[np]:可能下棋位置的set集合
	*/
	int GetNextPace(std::set<int>& np);//从链表中选取最高胜率的结点
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

	///更新位置分值所用函数 集中在UpdateScore.cpp文件中
	//更新棋盘位置分值，参数：行，列，是否加分
	void UpdateScore(int row1, int row2, int who, bool isAddScore = true);//调用RecordSpecialPoint()函数
	//设置特殊点影响的分值
	void ResetScore(int row1, int row2, double score, double NumberPoor = 100, bool isAddScore = true);//实行加分的函数
	//添加特殊点
	void RecordSpecialPoint(int row1, int row2, int who, bool isAddScore = true);//用SetCurrentPoint()设置当前点，再调用AddRecord
	void AddRecord(int n, int n1, int n2, int n3, int n4, int amo, bool isAddScore = true);
	void SetCurrentPoint(int ThisFormatStyle, double ThisFormatScorePoor, int ThisSpecialStyle, double ThisSpecialScorePoorint, int LastFormatStyle = 0, double LastFormatScorePoore = 0, int LastSpecialPointStyle = 0, double LastSpecialScorePoor = 0);

	///在RateResetScore.cpp文件中
	void RateResetScore(double ResetRate, bool add = true);//按比例缩小分值，减少分差，精确定位
	//--------------------------------------------------------------------------------//
	/*
	param[line]:下棋位置——行
	param[column]:下棋位置——列
	*/
	double CalDeadPosNumber(int line, int column, int &deadPos, bool Cal = true);//计算死棋位置数量
	/*
	param[line]:转换后的位置——行
	param[column]：转换后的位置——列
	param[site]：需要转换的位置
	*/
	void SymmetryExchange(int &line, int &column, int site);//将记录中的位置转换为合适当前盘面的位置

};


#endif // AI1_H_INCLUDED
