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

// ����ƥ��ģʽ
#define Black 1 // ����
#define White 2// ����
#define NoChess 4// û������
#define Edge 8 // �߽��
#define PATTERN 13// ƥ�俪ʼ
#define PATTEND 10// ƥ�����
// �ж������Ƿ�����������
#define on1board(x) (0 < (x) && (x) < 10)
#define onboard(x, y) (on1board(x) && on1board(y))  //��������������

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
	//��¼��������ֵ��������ʴӡ�1����ʼ������˳��Ϊ�����к��С���
	//��0����ʾû�����ӣ���1����ʾ���ӣ���2����ʾ����
	int cross[10][10];
	// ����
	int chessScore[10][10];
	// isGo2Dead��־����
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
	// ��ȡ������ӵ�λ��
	void GetPosition(int &line, int &column, int onTurn);
	bool isFinal();

	// �ж��Ƿ�������λ
	bool isGo2Dead(int line, int column, int type);
	void setStatus(int RivalLine, int RivalColumn);
	void reduceRecursionTimes();
	bool Besieg(int RivalLine, int RivalColumn, int player, int rival);

	int priority_score(int scoreBase, int scorePRI, int type);

	// �Ż���
	void GoodTigersMouth();

	// ��ʼ������
	void initChessScore(bool isFirst);
	void initAllArray();

	// ����������
	void Revalute();
	int maxandmin(int depth);
	int singleLayer();
	int DealWithScore(bool isEqual);
	void getMaxScore(int& tempLine, int& tempColumn);
	void getMinScore(int& tempLine, int& tempColumn);

	// ƥ�亯��
	void startPattern();
	void Pattern(int *patAdd, int times);

	// ��û��ƥ�䵽��ʱ���ȡһ�����ʵ�λ��
	int FindPosition();
};

#endif // AI2_H_INCLUDED

