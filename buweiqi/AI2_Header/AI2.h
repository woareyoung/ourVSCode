#pragma once
#ifndef AI2_H_INCLUDED
#define AI2_H_INCLUDED
#include "../chessBoard_Header/AI.h"
#include "AIPlayer.h"

#define cornerScore 120 // �߽Ƿ���
#define cornerScoreFull 160 // �������ɱ߽ǵķ���
#define tirangleScore1 60 // ���Ƿ���
#define tirangleScore2 120 // ���Ƿ���
#define tirangleScore3 140 // ���Ƿ���
#define crossShaped1 40 // ʮ�ַ���
#define crossShaped2 80 // ʮ�ַ���
#define crossShaped3  120// ʮ�ַ���
#define crossShaped4 160 // ʮ�ַ���
#define goodTigersMouth 120  // �Ż��ڣ���ʾ����Χ�ĵз�������ֻ��1~2��
#define badTigersMouth -120 // �ӻ��ڣ���ʾ����ΧȦ�ڶ��ǵз�����
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
#define on1board(x) (1 <= (x) && (x) < 10)
#define onboard(x, y) (on1board(x) && on1board(y))  //��������������

#define getRival(onTurn) (onTurn == isWhite ? isBlack : isWhite)

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
	// �߽�����
	int cornerArray[12];
	int chessCount;
	bool Position[4];
	void resetGo2DeadStatus();
public:
	AI2()
	{
		initAllArray();
	}
	// ��ȡ������ӵ�λ��
	void GetPosition(int &line, int &column, int onTurn);

	// �Խ�
	void AcrossCorners();
	void ACScan(int line, int column, int line1, int column1, int line2, int column2);

	// ����
	void Tirangle();
	void UTirangle();
	void LTirangle();
	void DTirangle();
	void RTirangle();

	// ʮ��
	void chessStatusShaped();
	void JudgeCShape(int line, int column);

	// �Ƿ񹹳�����
	void isGo2Dead(int type);
	void AddDeadChessScore(int stack[][2], int len);
	bool IsDeadChess(int stack[][2], int len, int type);
	int isFinal();

	// �ж��Ƿ�������λ
	bool isGo2Dead(int line, int column, int type);
	void setStatus(int RivalLine, int RivalColumn);
	void reduceRecursionTimes();
	bool Besieg(int RivalLine, int RivalColumn, int player, int rival);

	int priority_score(int scoreBase, int scorePRI, int type);

	// �Ż���
	void GoodTigersMouth();

	// ��ʼ������
	void initChessScore();
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

