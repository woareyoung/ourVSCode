#pragma once
#ifndef Pattern_Moves_H_INCLUDED
#define Pattern_Moves_H_INCLUDED
#include "../stdafx.h"
#include "../AI2_Header/DefaultChess.h"
#include <iostream>
#include <list>
#include <vector>

class Pattern_Moves : public DefaultChess
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

	void (Pattern_Moves::*Reverse[10])(DIRECTION*);
	void reverse(DIRECTION *PatternType);
	void reverseXY(DIRECTION *PatternType);
	void reverse_Y(DIRECTION *PatternType);
	void reverse_X(DIRECTION *PatternType);
	void reverse_X_Y(DIRECTION *PatternType);

	bool isGo2DeadStatus[10][10];
	void resetGo2DeadStatus();
	std::list<std::pair<int, int>> patternMoves;
protected:
	int turn2Who;
	int Rival;
	int cross[ChessEnd][ChessEnd];
	int *Type[2];
	// 分数
	mutable int chessScore[ChessEnd][ChessEnd];

	// 填充数组进行变换
	int* getPatternType() {
		return Type[turn2Who - 1];
	}

	void initAll();

	// 判断是否是死棋位
	bool isGo2Dead(const int line, const int column, const int& type);

	// 初始化数组
	void initChessScore(const bool& isFirst);
	// 匹配函数
	void startPattern();
	void Pattern(const int *PatternType);
	bool checkEmptyPos(const int& x, const int& y, const int& start, const int& mainColor, const Pos* emptyPos) {
		/******************************************
		判断当前匹配到的空位是否是敌方的自杀点，
		如果是的话，就把该点的分数设置为0，跳过匹配模式
		*******************************************/
		for (int i = 0; i < start; ++i) {
			if (mainColor == Rival) {
				if (chessScore[emptyPos[i].line][emptyPos[i].column] == 0) {
					return false;
				}
				// 临时设置当前获得的位置为敌方着子点，判断是否是敌方的自杀点
				if (isGo2Dead(emptyPos[i].line, emptyPos[i].column, Rival)) {
					// 如果是敌方的自杀点的话，这里就置零   -.-！！！
					chessScore[emptyPos[i].line][emptyPos[i].column] = 0;
					return false;
				}
			}
			else if (mainColor == turn2Who) {
				if (chessScore[emptyPos[i].line][emptyPos[i].column] == minLimit) {
					return false;
				}
				// 临时设置当前获得的位置为我方着子点，判断是否是我方的自杀点
				if (isGo2Dead(emptyPos[i].line, emptyPos[i].column, turn2Who)) {
					chessScore[emptyPos[i].line][emptyPos[i].column] = minLimit;
					// 如果是我方的自杀点的话，就直接跳转，不用判断是否是敌方的自杀点了。
					return false;
				}
			}
			// 这里既不是我方自杀点，也不是敌方自杀点
		}
		return true;
	}
	// 检查棋子是否有效，并对分析的结果进行相应的加分
	bool checkStone(const int& x, const int& y, const bool& below4) {
		// 对于当前匹配到的着子点的环境进行分析
		// 临时设置当前获得的位置为我方着子点，判断是否是我方的自杀点
		if (isGo2Dead(x, y, turn2Who)) {
			chessScore[x][y] = minLimit;
			// 如果是我方的自杀点的话，就直接跳转，不用判断是否是敌方的自杀点了。
			return false;
		}
		// 临时设置当前获得的位置为敌方着子点，判断是否是敌方的自杀点
		if (!below4) {
			return true;
		}
		if (cross[x][y] == NoChess && chessScore[x][y] == 0) return false;
		if (isGo2Dead(x, y, Rival)) {
			// 如果是敌方的自杀点的话，这里就置零   -.-！！！
			chessScore[x][y] = 0;
			return false;
		}
		// 这里既不是我方自杀点，也不是敌方自杀点
		return true;
	}

public:

	Pattern_Moves(int onTurn) :
		turn2Who(onTurn),
		Rival(getRival(onTurn))
	{
		initAll();
	}

	// 获取最后着子的位置
	std::list<std::pair<int, int>> getMoves(const bool& isFirst, const int cro[][ChessEnd]);
};

#endif // Pattern_Moves_H_INCLUDED

