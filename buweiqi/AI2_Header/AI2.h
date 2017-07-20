#pragma once
#ifndef AI2_H_INCLUDED
#define AI2_H_INCLUDED
#include "../chessBoard_Header/AI.h"
#include "AIPlayer.h"
#include "DefaultChess.h"
#include <iostream>
#include <vector>
#include <algorithm>

//struct goodMove {
//	int line;
//	int column;
//	int Score;
//	//bool operator < (const goodMove &A) const
//	//{
//	//	return A.Score < Score;// 从大到小排序
//	//}
//};

class AI2 : public AI, public AIPlayer, public DefaultChess
{
private:

	// 匹配模式
	static const int pattern_Total = 23;
	// 匹配的次数
	static const int pattern_Sum = 152;
	// 模式分数尺度
	int pattern_Score[pattern_Total];
	// 分数说明：
	// 对方自杀点且非我方自杀点的分数为0，我方自杀点为minLitmit。

	// pattern加分位置
	DIRECTION pattern_Score_Pos[pattern_Total];

	// 模式内判断棋子点数
	int pattern_Count[pattern_Total];
	// 看不懂的请看种子填充算法
	DIRECTION pattern_Background[pattern_Sum];
	// 利用与或处理棋子点
	// 匹配模式中棋子分布
	int pattern_White[pattern_Sum];
	// 匹配模式中棋子分布
	int pattern_Black[pattern_Sum];

	void (AI2::*Reverse[10])(DIRECTION*);
	void reverse(DIRECTION *PatternType);
	void reverseXY(DIRECTION *PatternType);
	void reverse_Y(DIRECTION *PatternType);
	void reverse_X(DIRECTION *PatternType);
	void reverse_X_Y(DIRECTION *PatternType);
protected:
	mutable int(*CS)[10];
	virtual void initCSPoint() {
		if (CS == nullptr) {
			CS = chessScore;
		}
	}
	int *Type[2];
	//记录各交叉点的值，数组访问从“1”开始，访问顺序为“先行后列”，
	//“0”表示没有棋子，“1”表示黑子，“2”表示白子
	mutable int cross[10][10];
	// bool Position[4];
	// 分数
	mutable int chessScore[10][10];
	
	// 当前好的着子点的数组集合
	/*std::vector<goodMove> goodMoves;*/

	int chessCount;

	// 填充数组进行变换
	virtual int* getPatternType() {
		return Type[turn2Who - 1];
	}

	virtual void initPlayerRole(int onTurn) {
		Rival = (onTurn == Black || onTurn == isAI1onTurn) ? Black : White;
		this->turn2Who = getRival(Rival);
		this->PlayerId = turn2Who;
	}

	void ScanChessBroad();
	void rollback(int line, int column, int onTurn) {
		if (onTurn == PlayerId) {
			--chessCount;
		}
		if (cross[line][column] != NoChess) {
			cross[line][column] = NoChess;
			CS[line][column] = getDefaultChessScore(line, column);
		}
	}
public:

	int patternAllScore[9][17] = {
		{ 60, 50, 40, 35, 35, 30, 30, 25, 25, 20, 20, 65, 65, 65, 55, 55, 55 },// 6
		{ 55, 50, 40, 35, 35, 30, 30, 25, 25, 20, 20, 60, 60, 60, 52, 52, 52 },
		{ 54, 50, 40, 35, 35, 30, 30, 25, 25, 20, 20, 57, 57 ,57, 50, 50, 50 },
		{ 53, 50, 40, 35, 35, 30, 30, 25, 25, 20, 20, 55, 55, 55, 48, 48, 48 },
		{ 52, 50, 40, 35, 35, 30, 30, 25, 25, 20, 20, 54, 54, 54, 45, 45, 45 },
		{ 51, 50, 40, 35, 35, 30, 30, 25, 25, 20, 20, 53, 53, 53, 43, 43, 43 },

		{ 45, 40, 35, 30, 30, 25, 25, 22, 22, 20, 20, 48, 48, 48, 35, 35, 35 },// 1

		{ 50, 60, 40, 35, 35, 30, 30, 25, 25, 20, 20, 55, 55, 55, 45, 45, 45 },
		{ 40, 50, 60, 35, 35, 30, 30, 25, 25, 20, 20, 45, 45, 45, 35, 35, 35 }
	};

	AI2() : chessCount(0), CS(nullptr)
	{
		initAllArray();
	}

	void setPatternScore(int& hang) {
		for (int i = 0; i < pattern_Total; ++i) {
			pattern_Score[i] = patternAllScore[hang][i];
		}
	}

	void initAll();
	// 获取最后着子的位置
	void GetPosition(int &line, int &column, int onTurn);
	bool isFinal();

	// 判断是否是死棋位
	bool isGo2Dead(const int line, const int column, const int& type);

	// 初始化数组
	void initChessScore(const bool& isFirst);
	void initAllArray();

	void Revalute();
	virtual int maxandmin(const int& depth);
	int singleLayer();
	int DealWithScore(const bool& isEqual);
	void getScore(int& tempLine, int& tempColumn, bool isGetMax);

	// 匹配函数
	void startPattern();
	void Pattern(const int *PatternType);
	virtual bool checkEmptyPos(
		const int& x, 
		const int& y, 
		const int& start, 
		const int& mainColor, 
		const Pos* emptyPos);
	// 检查棋子是否有效，并对分析的结果进行相应的加分
	virtual bool checkStone(
		const int& x, 
		const int& y, 
		const bool& below4);

	/*bool isContaint(goodMove move) {
	int size = goodMoves.size();
	for (int i = ChessInit; i < size; ++i) {
	if (move.line == goodMoves[i].line && move.column == goodMoves[i].column) {
	goodMoves[i].Score += move.Score;
	return true;
	}
	}
	return false;
	}*/
	//void addtoArray(int& line, int& column, int& score) {
	//	goodMove gm = { line,column,score };
	//	if (!isContaint(gm)) {
	//		goodMoves.push_back(gm);
	//	}
	//}
	//void arraySort() {
	//	// 排序
	//	if (goodMoves.size() >= 2) {
	//		std::sort(goodMoves.begin(), goodMoves.end(),
	//			[](const goodMove &v1, const goodMove &v2) -> bool {
	//			return v1.Score > v2.Score;// 从大到小排序  
	//		});
	//	}
	//}

	// 当没有匹配到的时候获取一个合适的位置
	int FindPosition();
};

#endif // AI2_H_INCLUDED

