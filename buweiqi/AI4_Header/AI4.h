#pragma once
#include <list>
#include <set>
#include <random>
#include "../ChessBoard_Header/AI.h"
#define GetId(x) ((x) < 0 ? 3 + x : 3 - (x)) //获取玩家身份
#define HaveChess -20 //有子的位置的分数
#define MyDeadChess -10 //我的死棋点的分数
#define RivalDeadChess -5 //对方死棋点的分数
#define GetLine(x) (x / 100)
#define GetColumn(y) (y % 100)

class AI4 : public AI
{
public:
	/*
	param[line]:下棋的位置——行
	param[column]:下棋的位置——列
	param[onTurn]:对方的身份、界面的命令
	*/
	void GetPosition(int &line, int &column, int onTurn);//AI的入口函数
private:
	int cross[10][10]; //棋盘状况
	int PlayerId; //玩家身份——玩家1还是玩家2
	int Round;//回合数
	int Score[10][10] = { 0 }; //记录棋盘每个位置的分值
	int level[10][10];//记录每个位置的级别（先设5级，有棋子的位置设0级，分值分别是6、4、3、2、1、0）
	int MaxScore;
	std::set<std::pair<int, int>> ForeFiveLevel;//前五级别的位置
	/*
	param[SpecialPoint]:存放特殊点及其分数的链表
	*/
	void UpdateBoardScore(std::list<std::pair<int, int>> &SpecialPoint);//获取特殊点列表及其对应的分数
	/*
	param[line]:加分的中心点位置——行
	param[column]:加分的中心点位置——列
	param[score]:中心点的分数
	param[ScorePoor]:层与层之间的分差
	*/
	template<class T1, class T2>
	void AddScore(int row1, int row2, T1 score, T2 ScorePoor);//扩散加分（不对负分的位置操作）
	/*
	参数line:下棋的位置——行
	参数column:下棋的位置——列
	参数who:谁 下的棋
	参数CROSS:当前棋盘状况（用于解决多线程的冲突）
	*/
	bool DeadCheck(int line, int column, int who, int CROSS[][10]);

	/*
	param[MaxLine]:分数最大的位置——行
	param[MaxColumn]:分数最大的位置——列
	*/
	void GetMaxScorePosition(int &MaxLine, int &MaxColumn);//获取分值最大的位置
	//初始化数组（一局游戏一次）
	void Initialize()
	{
		for (int i = 0; i < 10; ++i)
			for (int j = 0; j < 10; ++j)
			{
				cross[i][j] = 0;
				Score[i][j] = 0;
			}
		InitScore();
		Round = 0;
	}
	//初始化分值（一回合一次）
	void InitScore()
	{
		for (int i = 0; i < 10; ++i)
			for (int j = 0; j < 10; ++j)
				if(Score[i][j] > RivalDeadChess) Score[i][j] = 0;//小于1的位置不是对方的死棋就是自己的死棋或者不是空位
	}
	//执黑子时，第一步棋
	void FirstPaceScore()
	{
		Score[1][1] = 2;
		Score[1][2] = 2;
		Score[2][1] = 2;
		Score[1][8] = 2;
		Score[1][9] = 2;
		Score[2][9] = 2;
		Score[8][1] = 2;
		Score[9][1] = 2;
		Score[9][2] = 2;
		Score[8][9] = 2;
		Score[9][9] = 2;
	}
};