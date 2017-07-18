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

// ����ƥ��ģʽ
#define Black 1 // ����
#define White 2// ����
#define NoChess 4// û������
#define Edge 8 // �߽��
// �ж������Ƿ�����������
#define isOnChessBoard(x) (0 < (x) && (x) < 10)
#define OnChessBoard(x, y) (isOnChessBoard(x) && isOnChessBoard(y))  //��������������

#define getRival(onTurn) (onTurn == White ? Black : White)

#define getLine(temp) ( (temp) <= 0 || (temp) > 990 ? 0 : (temp / 100))
#define getColumn(temp) ( (temp) <= 0 || (temp) > 990 ? 0 : (temp % 100))
#define getMove(line, column) (OnChessBoard(line, column) ? (line * 100 + column) : 0)

typedef struct tagDIRECTION
{
	int x_offset;
	int y_offset;
} DIRECTION;

struct Pos {
	int line;
	int column;
};

//struct goodMove {
//	int line;
//	int column;
//	int Score;
//	//bool operator < (const goodMove &A) const
//	//{
//	//	return A.Score < Score;// �Ӵ�С����
//	//}
//};

class AI2 : public AI, public AIPlayer, public DefaultChess
{
private:

	// ƥ��ģʽ
	static const int pattern_Total = 17;
	// ƥ��Ĵ���
	static const int pattern_Sum = 86;
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
	//��¼��������ֵ��������ʴӡ�1����ʼ������˳��Ϊ�����к��С���
	//��0����ʾû�����ӣ���1����ʾ���ӣ���2����ʾ����
	mutable int cross[10][10];
	bool Position[4];
	// ����
	mutable int chessScore[10][10];
	// isGo2Dead��־����
	bool isGo2DeadStatus[10][10];
	// ��ǰ�õ����ӵ�����鼯��
	/*std::vector<goodMove> goodMoves;*/

	int chessCount;

	// ���������б任
	virtual int* getPatternType() {
		return Type[turn2Who - 1];
	}

	virtual void initPlayerRole(int onTurn) {
		Rival = (onTurn == Black || onTurn == isAI1onTurn) ? Black : White;
		this->turn2Who = getRival(Rival);
		this->PlayerId = turn2Who;
	}

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
	// ��ȡ������ӵ�λ��
	void GetPosition(int &line, int &column, int onTurn);
	bool isFinal();

	// �ж��Ƿ�������λ
	bool isGo2Dead(const int line, const int column, const int& type);

	// ��ʼ������
	void initChessScore(const bool& isFirst);
	void initAllArray();
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

	void Revalute();
	virtual int maxandmin(const int& depth);
	int singleLayer();
	int DealWithScore(const bool& isEqual);
	void getScore(int& tempLine, int& tempColumn, bool isGetMax);

	// ƥ�亯��
	void startPattern();
	void Pattern(const int *PatternType);
	virtual bool checkEmptyPos(const int& x, const int& y, const int& start, const int& mainColor, const Pos* emptyPos) {
		/******************************************
		�жϵ�ǰƥ�䵽�Ŀ�λ�Ƿ��ǵз�����ɱ�㣬
		����ǵĻ����ͰѸõ�ķ�������Ϊ0������ƥ��ģʽ
		*******************************************/
		for (int i = 0; i < start; ++i) {
			if (mainColor == Rival) {
				// ��ʱ���õ�ǰ��õ�λ��Ϊ�з����ӵ㣬�ж��Ƿ��ǵз�����ɱ��
				if (isGo2Dead(emptyPos[i].line, emptyPos[i].column, Rival)) {
					// ����ǵз�����ɱ��Ļ������������   -.-������
					CS[emptyPos[i].line][emptyPos[i].column] = 0;
					return false;
				}
			}
			else if (mainColor == turn2Who) {
				// ��ʱ���õ�ǰ��õ�λ��Ϊ�ҷ����ӵ㣬�ж��Ƿ����ҷ�����ɱ��
				if (isGo2Dead(x, y, turn2Who)) {
					CS[x][y] = minLimit;
					// ������ҷ�����ɱ��Ļ�����ֱ����ת�������ж��Ƿ��ǵз�����ɱ���ˡ�
					return false;
				}
			}
			// ����Ȳ����ҷ���ɱ�㣬Ҳ���ǵз���ɱ��
		}
		return true;
	}
	// ��������Ƿ���Ч�����Է����Ľ��������Ӧ�ļӷ�
	virtual bool checkStone(const int& x, const int& y, const bool& below4) {
		// ���ڵ�ǰƥ�䵽�����ӵ�Ļ������з���
		// ��ʱ���õ�ǰ��õ�λ��Ϊ�ҷ����ӵ㣬�ж��Ƿ����ҷ�����ɱ��
		if (isGo2Dead(x, y, turn2Who)) {
			CS[x][y] = minLimit;
			// ������ҷ�����ɱ��Ļ�����ֱ����ת�������ж��Ƿ��ǵз�����ɱ���ˡ�
			return false;
		}
		// ��ʱ���õ�ǰ��õ�λ��Ϊ�з����ӵ㣬�ж��Ƿ��ǵз�����ɱ��
		if (!below4) {
			return true;
		}
		if (cross[x][y] == NoChess && CS[x][y] == 0) return false;
		if (isGo2Dead(x, y, Rival)) {
			// ����ǵз�����ɱ��Ļ������������   -.-������
			CS[x][y] = 0;
			return false;
		}
		// ����Ȳ����ҷ���ɱ�㣬Ҳ���ǵз���ɱ��
		return true;
	}
	//void addtoArray(int& line, int& column, int& score) {
	//	goodMove gm = { line,column,score };
	//	if (!isContaint(gm)) {
	//		goodMoves.push_back(gm);
	//	}
	//}
	//void arraySort() {
	//	// ����
	//	if (goodMoves.size() >= 2) {
	//		std::sort(goodMoves.begin(), goodMoves.end(),
	//			[](const goodMove &v1, const goodMove &v2) -> bool {
	//			return v1.Score > v2.Score;// �Ӵ�С����  
	//		});
	//	}
	//}

	// ��û��ƥ�䵽��ʱ���ȡһ�����ʵ�λ��
	int FindPosition();
};

#endif // AI2_H_INCLUDED

