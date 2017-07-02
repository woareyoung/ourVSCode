#pragma once
#ifndef AI2_H_INCLUDED
#define AI2_H_INCLUDED
#include "../chessBoard_Header/AI.h"
#include "AIPlayer.h"
#include "DefaultChess.h"
#include <iostream>
#include <vector>
#include <algorithm>

#define ChessInit 0
#define ChessStart 1
#define ChessEnd 10

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
// 判断棋子是否落在棋盘内
#define isOnChessBoard(x) (0 < (x) && (x) < 10)
#define OnChessBoard(x, y) (isOnChessBoard(x) && isOnChessBoard(y))  //棋子落在棋盘里

#define getRival(onTurn) (onTurn == White ? Black : White)

#define getLine(temp) (temp / 100)
#define getColumn(temp) (temp % 100)

typedef struct tagDIRECTION
{
	int x_offset;
	int y_offset;
} DIRECTION;

struct Pos {
	int line;
	int column;
};

struct goodMove {
	int line;
	int column;
	int Score;
	bool operator < (const goodMove &A) const
	{
		return A.Score < Score;// 从大到小排序
	}
};

class AI2 : public AI, public AIPlayer, public DefaultChess
{
private:
	// 填充数组进行变换
	void (AI2::*Reverse[10])(DIRECTION*);
	void reverse(DIRECTION *PatternType);
	void reverseXY(DIRECTION *PatternType);
	void reverse_Y(DIRECTION *PatternType);
	void reverse_X(DIRECTION *PatternType);
	void reverse_X_Y(DIRECTION *PatternType);
protected:
	//记录各交叉点的值，数组访问从“1”开始，访问顺序为“先行后列”，
	//“0”表示没有棋子，“1”表示黑子，“2”表示白子
	mutable unsigned int cross[10][10];

	// 分数
	int chessScore[10][10];
	// isGo2Dead标志数组
	bool isGo2DeadStatus[10][10];
	// 当前好的着子点的数组集合
	goodMove goodMoves[81];
	int MovePointer;// 当前好的着子点的数量
	int chessCount;
	bool Position[4];

	void resetGo2DeadStatus() {
		for (register int i = ChessInit; i < ChessEnd; ++i) {
			for (register int j = ChessInit; j < ChessEnd; ++j) {
				isGo2DeadStatus[i][j] = false;
			}
		}
	}

	void ScanChessBroad();
	void rollback(int line, int column, int onTurn) {
		if (onTurn == PlayerId) {
			--chessCount;
		}
		if (cross[line][column] != NoChess) {
			cross[line][column] = NoChess;
			chessScore[line][column] = getDefaultChessScore(line, column);
		}
	}
public:
	AI2() : chessCount(0), MovePointer(0)
	{
		initAllArray();
	}

	// 获取最后着子的位置
	void GetPosition(int &line, int &column, int onTurn);
	bool isFinal();

	// 判断是否是死棋位
	bool isGo2Dead(int line, int column, int type);
	bool Besieg(int RivalLine, int RivalColumn, int player, int rival);

	// 初始化数组
	void initChessScore(bool isFirst);
	void initAllArray();
	bool isContaint(goodMove move) {
		for (int i = ChessInit; i < MovePointer; ++i) {
			if (move.line == goodMoves[i].line 
				&& move.column == goodMoves[i].column
				&& move.Score == goodMoves[i].Score) {
				return true;
			}
		}
		return false;
	}
	int getMaxScoreNum(int judge);

	void Revalute();
	int maxandmin(int depth);
	int singleLayer();
	int DealWithScore(bool isEqual);
	void getMaxScore(int& tempLine, int& tempColumn);
	void getMinScore(int& tempLine, int& tempColumn);

	// 匹配函数
	void startPattern();
	void Pattern(int *PatternType);
	virtual bool checkEmptyPos(int& x, int& y, int& start, int& mainColor, Pos emptyPos[]) {
		/******************************************
		判断当前匹配到的空位是否是敌方的自杀点，
		如果是的话，就把该点的分数设置为0，跳过匹配模式
		*******************************************/
		for (int i = 0; i < start; ++i) {
			if (mainColor == Rival) {
				// 临时设置当前获得的位置为敌方着子点，判断是否是敌方的自杀点
				cross[emptyPos[i].line][emptyPos[i].column] = Rival;
				if (isGo2Dead(emptyPos[i].line, emptyPos[i].column, Rival)) {
					cross[emptyPos[i].line][emptyPos[i].column] = NoChess;
					// 如果是敌方的自杀点的话，这里就置零   -.-！！！
					chessScore[emptyPos[i].line][emptyPos[i].column] = 0;
					return false;
				}
			}
			else if (mainColor == turn2Who) {
				// 临时设置当前获得的位置为我方着子点，判断是否是我方的自杀点
				cross[x][y] = turn2Who;
				if (isGo2Dead(x, y, turn2Who)) {
					chessScore[x][y] = minLimit;
					cross[x][y] = NoChess;
					// 如果是我方的自杀点的话，就直接跳转，不用判断是否是敌方的自杀点了。
					return false;
				}
			}
			// 这里既不是我方自杀点，也不是敌方自杀点
			cross[emptyPos[i].line][emptyPos[i].column] = NoChess;
		}
		return true;
	}
	// 检查棋子是否有效，并对分析的结果进行相应的加分
	virtual bool checkStone(int& x, int& y) {
		// 对于当前匹配到的着子点的环境进行分析
		// 临时设置当前获得的位置为我方着子点，判断是否是我方的自杀点
		cross[x][y] = turn2Who;
		if (isGo2Dead(x, y, turn2Who)) {
			chessScore[x][y] = minLimit;
			cross[x][y] = NoChess;
			// 如果是我方的自杀点的话，就直接跳转，不用判断是否是敌方的自杀点了。
			return false;
		}
		// 临时设置当前获得的位置为敌方着子点，判断是否是敌方的自杀点
		if (cross[x][y] == NoChess && chessScore[x][y] == 0) return false;
		cross[x][y] = Rival;
		if (isGo2Dead(x, y, Rival)) {
			cross[x][y] = NoChess;
			// 如果是敌方的自杀点的话，这里就置零   -.-！！！
			chessScore[x][y] = 0;
			return false;
		}
		// 这里既不是我方自杀点，也不是敌方自杀点
		cross[x][y] = NoChess;
		return true;
	}
	void arraySort(int& x, int& y, int& score) {
		if (!isContaint({ x,y,score })) {
			goodMoves[MovePointer] = { x,y,score };
			// 排序
			std::sort(goodMoves, goodMoves + MovePointer);
			MovePointer++;
		}
	}

	// 当没有匹配到的时候获取一个合适的位置
	int FindPosition();
};

#endif // AI2_H_INCLUDED

