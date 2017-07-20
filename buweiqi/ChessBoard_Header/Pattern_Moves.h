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

	// ƥ��ģʽ
	static const int pattern_Total = 23;
	// ƥ��Ĵ���
	static const int pattern_Sum = 152;
	// ģʽ�����߶�
	int pattern_Score[pattern_Total];
	// ����˵����
	// �Է���ɱ���ҷ��ҷ���ɱ��ķ���Ϊ0���ҷ���ɱ��ΪminLitmit��

	// pattern�ӷ�λ��
	DIRECTION pattern_Score_Pos[pattern_Total];

	// ģʽ���ж����ӵ���
	int pattern_Count[pattern_Total];
	// ���������뿴��������㷨
	DIRECTION pattern_Background[pattern_Sum];
	// ������������ӵ�
	// ƥ��ģʽ�����ӷֲ�
	int pattern_White[pattern_Sum];
	// ƥ��ģʽ�����ӷֲ�
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
	// ����
	mutable int chessScore[ChessEnd][ChessEnd];

	// ���������б任
	int* getPatternType() {
		return Type[turn2Who - 1];
	}

	void initAll();

	// �ж��Ƿ�������λ
	bool isGo2Dead(const int line, const int column, const int& type);

	// ��ʼ������
	void initChessScore(const bool& isFirst);
	// ƥ�亯��
	void startPattern();
	void Pattern(const int *PatternType);
	bool checkEmptyPos(const int& x, const int& y, const int& start, const int& mainColor, const Pos* emptyPos) {
		/******************************************
		�жϵ�ǰƥ�䵽�Ŀ�λ�Ƿ��ǵз�����ɱ�㣬
		����ǵĻ����ͰѸõ�ķ�������Ϊ0������ƥ��ģʽ
		*******************************************/
		for (int i = 0; i < start; ++i) {
			if (mainColor == Rival) {
				if (chessScore[emptyPos[i].line][emptyPos[i].column] == 0) {
					return false;
				}
				// ��ʱ���õ�ǰ��õ�λ��Ϊ�з����ӵ㣬�ж��Ƿ��ǵз�����ɱ��
				if (isGo2Dead(emptyPos[i].line, emptyPos[i].column, Rival)) {
					// ����ǵз�����ɱ��Ļ������������   -.-������
					chessScore[emptyPos[i].line][emptyPos[i].column] = 0;
					return false;
				}
			}
			else if (mainColor == turn2Who) {
				if (chessScore[emptyPos[i].line][emptyPos[i].column] == minLimit) {
					return false;
				}
				// ��ʱ���õ�ǰ��õ�λ��Ϊ�ҷ����ӵ㣬�ж��Ƿ����ҷ�����ɱ��
				if (isGo2Dead(emptyPos[i].line, emptyPos[i].column, turn2Who)) {
					chessScore[emptyPos[i].line][emptyPos[i].column] = minLimit;
					// ������ҷ�����ɱ��Ļ�����ֱ����ת�������ж��Ƿ��ǵз�����ɱ���ˡ�
					return false;
				}
			}
			// ����Ȳ����ҷ���ɱ�㣬Ҳ���ǵз���ɱ��
		}
		return true;
	}
	// ��������Ƿ���Ч�����Է����Ľ��������Ӧ�ļӷ�
	bool checkStone(const int& x, const int& y, const bool& below4) {
		// ���ڵ�ǰƥ�䵽�����ӵ�Ļ������з���
		// ��ʱ���õ�ǰ��õ�λ��Ϊ�ҷ����ӵ㣬�ж��Ƿ����ҷ�����ɱ��
		if (isGo2Dead(x, y, turn2Who)) {
			chessScore[x][y] = minLimit;
			// ������ҷ�����ɱ��Ļ�����ֱ����ת�������ж��Ƿ��ǵз�����ɱ���ˡ�
			return false;
		}
		// ��ʱ���õ�ǰ��õ�λ��Ϊ�з����ӵ㣬�ж��Ƿ��ǵз�����ɱ��
		if (!below4) {
			return true;
		}
		if (cross[x][y] == NoChess && chessScore[x][y] == 0) return false;
		if (isGo2Dead(x, y, Rival)) {
			// ����ǵз�����ɱ��Ļ������������   -.-������
			chessScore[x][y] = 0;
			return false;
		}
		// ����Ȳ����ҷ���ɱ�㣬Ҳ���ǵз���ɱ��
		return true;
	}

public:

	Pattern_Moves(int onTurn) :
		turn2Who(onTurn),
		Rival(getRival(onTurn))
	{
		initAll();
	}

	// ��ȡ������ӵ�λ��
	std::list<std::pair<int, int>> getMoves(const bool& isFirst, const int cro[][ChessEnd]);
};

#endif // Pattern_Moves_H_INCLUDED

