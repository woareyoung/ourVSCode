#pragma once
#ifndef AI2_H_INCLUDED
#define AI2_H_INCLUDED
#include "../chessBoard_Header/AI.h"
#include "AIPlayer.h"

#define cornerScore 120 // 边角分数
#define cornerScoreFull 160 // 完整构成边角的分数
#define tirangleScore1 60 // 三角分数
#define tirangleScore2 120 // 三角分数
#define tirangleScore3 140 // 三角分数
#define crossShaped1 40 // 十字分数
#define crossShaped2 80 // 十字分数
#define crossShaped3  120// 十字分数
#define crossShaped4 160 // 十字分数
#define goodTigersMouth 120  // 优虎口，表示被包围的敌方棋子中只有1~2个
#define badTigersMouth -120 // 劣虎口，表示被包围圈内都是敌方棋子
#define maxLimit 32767
#define minLimit -32767

#define Black 1;
#define White 2;
#define NoChess 4;
#define Edge 8;

#define isParent 1
#define isSiblings 2

#define isMax 0
#define isMin 1

#define getRival(onTurn) (onTurn == isWhite ? isBlack : isWhite)

typedef struct tagDIRECTION
{
	int x_offset;
	int y_offset;
} DIRECTION;

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
	// 边角数组
	int cornerArray[12];
	int chessCount;
	bool Position[4];
	void resetGo2DeadStatus();
public:
	AI2()
	{
		initAllArray();
	}
	// 获取最后着子的位置
	void GetPosition(int &line, int &column, int onTurn);

	// 对角
	void AcrossCorners();
	void ACScan(int line, int column, int line1, int column1, int line2, int column2);

	// 三角
	void Tirangle();
	void UTirangle();
	void LTirangle();
	void DTirangle();
	void RTirangle();

	// 十字
	void chessStatusShaped();
	void JudgeCShape(int line, int column);

	// 是否构成死棋
	void isGo2Dead(int type);
	void AddDeadChessScore(int stack[][2], int len);
	bool IsDeadChess(int stack[][2], int len, int type);
	int isFinal();
	
	// 判断是否是死棋位
	bool isGo2Dead(int line, int column, int type);
	void setStatus(int RivalLine, int RivalColumn);
	void reduceRecursionTimes();
	bool Besieg(int RivalLine, int RivalColumn, int player, int rival);

	int priority_score(int scoreBase, int scorePRI, int type);

	// 优虎口
	void GoodTigersMouth();

	// 初始化数组
	void initChessScore();
	void initAllArray();

	// 博弈树部分
	void Revalute();
	int maxandmin(int depth);
	int MaxScore();
	void getMaxScore(int& tempLine, int& tempColumn);
	int MinScore();
	int getMinScore(int& tempLine, int& tempColumn);
	int singleLayer();

};

#endif // AI2_H_INCLUDED

