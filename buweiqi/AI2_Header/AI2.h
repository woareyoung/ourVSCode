#pragma once
#ifndef AI2_H_INCLUDED
#define AI2_H_INCLUDED
#include "../chessBoard_Header/AI.h"
#include "AIPlayer.h"
#include "DefaultChess.h"
#include <iostream>
#include <vector>
#include <set>

#define ChessInit 0
#define ChessStart 1
#define ChessEnd 10

#define maxLimit 32767
#define minLimit -32767

#define isParent 1
#define isSiblings 2

#define isMax 0
#define isMin 1

#define pass (-1)
#define no_move (-2)

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
	//��¼��������ֵ��������ʴӡ�1����ʼ������˳��Ϊ�����к��С���
	//��0����ʾû�����ӣ���1����ʾ���ӣ���2����ʾ����
	mutable unsigned int cross[10][10];
	mutable int depth;
	unsigned int previous_board_hash_value;
	std::set<unsigned int> all_hash_values;

	// ����
	int chessScore[10][10];
	// isGo2Dead��־����
	bool isGo2DeadStatus[10][10];
	// ��ǰ�õ����ӵ�����鼯��
	goodMove goodMoves[81];
	int MovePointer;// ��ǰ�õ����ӵ������
	int chessCount;
	bool Position[4];
	void resetGo2DeadStatus();
	void ScanChessBroad();
	void rollback(int line, int column, int onTurn);
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
	bool isContaint(goodMove move);
	int getMaxScoreNum(int judge);
	int random(double start, double end);

	// ����������
	void Revalute();
	int maxandmin(int depth);
	int singleLayer();
	int DealWithScore(bool isEqual);
	void getMaxScore(int& tempLine, int& tempColumn);
	void getMinScore(int& tempLine, int& tempColumn);

	template<typename RandomEngine>
	void do_random_move(RandomEngine* engine)
	{
		auto moves = get_moves();
		attest(! moves.empty());
		std::uniform_int_distribution<std::size_t> move_ind(0, moves.size() - 1);
		auto move = moves[move_ind(*engine)];
		do_move(move);
	}

	virtual std::vector<int> get_moves() const
	{
		std::vector<int> moves;
		if (depth > 1000) {
			return moves;
		}

		for (int i = ChessStart; i < ChessEnd; ++i) {
		for (int j = ChessStart; j < ChessEnd; ++j) {
				
		}}

//		if (moves.empty() && opponent_has_move) {
//			moves.push_back(pass);
//		}

		return moves;
	}

	// ƥ�亯��
	void startPattern();
	void Pattern(int *PatternType);
	void (AI2::*Reverse[10])(DIRECTION*);
	void reverse(DIRECTION *PatternType);
	void reverseXY(DIRECTION *PatternType);
	void reverse_Y(DIRECTION *PatternType);
	void reverse_X(DIRECTION *PatternType);
	void reverse_X_Y(DIRECTION *PatternType);

	// ��û��ƥ�䵽��ʱ���ȡһ�����ʵ�λ��
	int FindPosition();
};

#endif // AI2_H_INCLUDED

