#pragma once
#ifndef AI2_H_INCLUDED
#define AI2_H_INCLUDED
#include "../chessBoard_Header/AI.h"
#include "AIPlayer.h"

#define maxLimit 32767
#define minLimit -32767

#define isParent 1
#define isSiblings 2

#define isMax 0
#define isMin 1

// 用于匹配模式
#define Black 1 // 黑子
#define White 2// 白子
#define NoChess 4// 没有棋子
#define Edge 8 // 边界点
#define PATTERN 13// 匹配开始
#define PATTEND 10// 匹配结束
// 判断棋子是否落在棋盘内
#define on1board(x) (0 < (x) && (x) < 10)
#define onboard(x, y) (on1board(x) && on1board(y))  //棋子落在棋盘里

#define getRival(onTurn) (onTurn == White ? Black : White)

typedef struct tagDIRECTION
{
	int x_offset;
	int y_offset;
} DIRECTION;

struct Pos {
	int line;
	int column;
};

class AI2 : public AI, public AIPlayer
{
private:
	//记录各交叉点的值，数组访问从“1”开始，访问顺序为“先行后列”，
	//“0”表示没有棋子，“1”表示黑子，“2”表示白子
	int cross[10][10];
	// 分数
	int chessScore[10][10];
	// isGo2Dead标志数组
	bool isGo2DeadStatus[10][10];
	int chessCount;
	bool Position[4];
	void resetGo2DeadStatus();
	void ScanChessBroad();
public:
	AI2()
	{
		initAllArray();
	}
	// 获取最后着子的位置
	void GetPosition(int &line, int &column, int onTurn);
	bool isFinal();

	// 判断是否是死棋位
	bool isGo2Dead(int line, int column, int type);
	void setStatus(int RivalLine, int RivalColumn);
	void reduceRecursionTimes();
	bool Besieg(int RivalLine, int RivalColumn, int player, int rival);

	int priority_score(int scoreBase, int scorePRI, int type);

	// 优虎口
	void GoodTigersMouth();

	// 初始化数组
	void initChessScore(bool isFirst);
	void initAllArray();

	// 博弈树部分
	void Revalute();
	int maxandmin(int depth);
	int singleLayer();
	int DealWithScore(bool isEqual);
	void getMaxScore(int& tempLine, int& tempColumn);
	void getMinScore(int& tempLine, int& tempColumn);

	// 匹配函数
	void startPattern();
	void Pattern(int *patAdd, int times);

	// 当没有匹配到的时候获取一个合适的位置
	int FindPosition();
};

#endif // AI2_H_INCLUDED

