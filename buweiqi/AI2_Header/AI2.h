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
		return A.Score < Score;// �Ӵ�С����
	}
};

class AI2 : public AI, public AIPlayer, public DefaultChess
{
private:
	// ���������б任
	void (AI2::*Reverse[10])(DIRECTION*);
	void reverse(DIRECTION *PatternType);
	void reverseXY(DIRECTION *PatternType);
	void reverse_Y(DIRECTION *PatternType);
	void reverse_X(DIRECTION *PatternType);
	void reverse_X_Y(DIRECTION *PatternType);
protected:
	//��¼��������ֵ��������ʴӡ�1����ʼ������˳��Ϊ�����к��С���
	//��0����ʾû�����ӣ���1����ʾ���ӣ���2����ʾ����
	mutable unsigned int cross[10][10];

	// ����
	int chessScore[10][10];
	// isGo2Dead��־����
	bool isGo2DeadStatus[10][10];
	// ��ǰ�õ����ӵ�����鼯��
	goodMove goodMoves[81];
	int MovePointer;// ��ǰ�õ����ӵ������
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

	// ��ȡ������ӵ�λ��
	void GetPosition(int &line, int &column, int onTurn);
	bool isFinal();

	// �ж��Ƿ�������λ
	bool isGo2Dead(int line, int column, int type);
	bool Besieg(int RivalLine, int RivalColumn, int player, int rival);

	// ��ʼ������
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

	// ƥ�亯��
	void startPattern();
	void Pattern(int *PatternType);
	virtual bool checkEmptyPos(int& x, int& y, int& start, int& mainColor, Pos emptyPos[]) {
		/******************************************
		�жϵ�ǰƥ�䵽�Ŀ�λ�Ƿ��ǵз�����ɱ�㣬
		����ǵĻ����ͰѸõ�ķ�������Ϊ0������ƥ��ģʽ
		*******************************************/
		for (int i = 0; i < start; ++i) {
			if (mainColor == Rival) {
				// ��ʱ���õ�ǰ��õ�λ��Ϊ�з����ӵ㣬�ж��Ƿ��ǵз�����ɱ��
				cross[emptyPos[i].line][emptyPos[i].column] = Rival;
				if (isGo2Dead(emptyPos[i].line, emptyPos[i].column, Rival)) {
					cross[emptyPos[i].line][emptyPos[i].column] = NoChess;
					// ����ǵз�����ɱ��Ļ������������   -.-������
					chessScore[emptyPos[i].line][emptyPos[i].column] = 0;
					return false;
				}
			}
			else if (mainColor == turn2Who) {
				// ��ʱ���õ�ǰ��õ�λ��Ϊ�ҷ����ӵ㣬�ж��Ƿ����ҷ�����ɱ��
				cross[x][y] = turn2Who;
				if (isGo2Dead(x, y, turn2Who)) {
					chessScore[x][y] = minLimit;
					cross[x][y] = NoChess;
					// ������ҷ�����ɱ��Ļ�����ֱ����ת�������ж��Ƿ��ǵз�����ɱ���ˡ�
					return false;
				}
			}
			// ����Ȳ����ҷ���ɱ�㣬Ҳ���ǵз���ɱ��
			cross[emptyPos[i].line][emptyPos[i].column] = NoChess;
		}
		return true;
	}
	// ��������Ƿ���Ч�����Է����Ľ��������Ӧ�ļӷ�
	virtual bool checkStone(int& x, int& y) {
		// ���ڵ�ǰƥ�䵽�����ӵ�Ļ������з���
		// ��ʱ���õ�ǰ��õ�λ��Ϊ�ҷ����ӵ㣬�ж��Ƿ����ҷ�����ɱ��
		cross[x][y] = turn2Who;
		if (isGo2Dead(x, y, turn2Who)) {
			chessScore[x][y] = minLimit;
			cross[x][y] = NoChess;
			// ������ҷ�����ɱ��Ļ�����ֱ����ת�������ж��Ƿ��ǵз�����ɱ���ˡ�
			return false;
		}
		// ��ʱ���õ�ǰ��õ�λ��Ϊ�з����ӵ㣬�ж��Ƿ��ǵз�����ɱ��
		if (cross[x][y] == NoChess && chessScore[x][y] == 0) return false;
		cross[x][y] = Rival;
		if (isGo2Dead(x, y, Rival)) {
			cross[x][y] = NoChess;
			// ����ǵз�����ɱ��Ļ������������   -.-������
			chessScore[x][y] = 0;
			return false;
		}
		// ����Ȳ����ҷ���ɱ�㣬Ҳ���ǵз���ɱ��
		cross[x][y] = NoChess;
		return true;
	}
	void arraySort(int& x, int& y, int& score) {
		if (!isContaint({ x,y,score })) {
			goodMoves[MovePointer] = { x,y,score };
			// ����
			std::sort(goodMoves, goodMoves + MovePointer);
			MovePointer++;
		}
	}

	// ��û��ƥ�䵽��ʱ���ȡһ�����ʵ�λ��
	int FindPosition();
};

#endif // AI2_H_INCLUDED

