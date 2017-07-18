#pragma once
#include <list>
#include <set>
#include <random>
#include "../ChessBoard_Header/AI.h"
#include "../ChessBoard_Header/Pattern_Moves.h"
#define GetId(x) ((x) < 0 ? (-x) : (x)) //获取玩家身份
#define HaveChess -50000 //有子的位置的分数
#define DeadChess -10 //死棋点的分数

class AI4 : public AI
{
	Pattern_Moves PM;
	int cross[10][10]; //棋盘状况
	int PlayerId; //玩家身份——玩家1还是玩家2
	int Round;//回合数
	double Score[10][10]; //记录棋盘每个位置的分值

	/*
	param[line]:下棋的位置——行
	param[column]:下棋的位置——列
	param[onTurn]:对方的身份、界面的命令
	*/
	void GetPosition(int &line, int &column, int onTurn);//AI的入口函数
	/*
	param[SpecialPoint]:存放特殊点及其分数的链表
	*/
	void UpdateScore(std::list<std::pair<int, int>> &SpecialPoint);//获取特殊点列表及其对应的分数
	/*
	param[line]:加分的中心点位置——行
	param[column]:加分的中心点位置——列
	param[score]:中心点的分数
	param[ScorePoor]:层与层之间的分差
	*/
	template<class T1, class T2>
	void AddScore(int row1, int row2, T1 score, T2 ScorePoor);//扩散加分（不对负分的位置操作）

	/*
	param[MaxLine]:分数最大的位置——行
	param[MaxColumn]:分数最大的位置——列
	*/
	void GetMaxScorePosition(int &MaxLine, int &MaxColumn)//获取分值最大的位置
	{
		std::vector<std::pair<int, int>> MaxPos;
		auto tempMax = -100.0;//临时最大值
		for (int i = 1; i < 10; ++i)
		{
			for (int j = 1; j < 10; ++j)
			{
				if (Score[i][j] > tempMax)
				{
					MaxPos.clear();
					tempMax = Score[i][j];
				}
				else if (Score[i][j] == tempMax)
				{
					MaxPos.emplace_back(std::make_pair(i, j));
				}
			}
		}
		std::mt19937_64 random_engine(1000);
		std::uniform_int_distribution<std::size_t> movesDist(0, MaxPos.size() - 1);
		auto move = MaxPos[movesDist(random_engine)];
		MaxLine = move.first;
		MaxColumn = move.second;
	}
	//初始化数组（一局游戏一次）
	void Initialize()
	{
		for (int i = 0; i < 10; ++i)
			for (int j = 0; j < 10; ++j)
				cross[i][j] = 0;
		InitScore();
		Round = 0;
	}
	//初始化分值（一回合一次）
	void InitScore()
	{
		for (int i = 0; i < 10; ++i)
			for (int j = 0; j < 10; ++j)
				if(Score[i][j] > 0) Score[i][j] = 1;//小于1的位置不是对方的死棋就是自己的死棋或者不是空位
	}
};