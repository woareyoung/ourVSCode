#pragma once
#include "../chessBoard_Header/AI.h"
#include "AIPlayer.h"
#include "DefaultChess.h"
#include "../ChessBoard_Header/parameter.h"
#include <iostream>
#include <vector>

class AI2 : public AI, public AIPlayer, public DefaultChess
{
private:

	// ƥ��ģʽ
	static const int pattern_Total = 27;
	// ƥ��Ĵ���
	static const int pattern_Sum = 168;
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
		CS = chessScore;
	}
	int *Type[2];
	//��¼��������ֵ��������ʴӡ�1����ʼ������˳��Ϊ�����к��С���
	//��0����ʾû�����ӣ���1����ʾ���ӣ���2����ʾ����
	mutable int cross[10][10];
	// ����
	mutable int chessScore[10][10];

	int chessCount;

	// ���������б任
	virtual int* getPatternType() {
		if (turn2Who == Black || turn2Who == White) {
			return Type[turn2Who - 1];
		}
		else {
			return nullptr;
		}
	}

	virtual void initPlayerRole(int onTurn) {
		Rival = (onTurn == Black || onTurn == isAI1onTurn) ? Black : White;
		this->turn2Who = getRival(Rival);
		this->PlayerId = turn2Who;
	}

	virtual void ScanChessBroad();
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

	AI2() : chessCount(0), CS(chessScore)
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

	void Revalute();
	virtual int maxandmin(const int& depth);
	int singleLayer();
	int DealWithScore(const bool& isEqual);
	void getScore(int& tempLine, int& tempColumn, bool isGetMax);

	// ƥ�亯��
	void startPattern();
	void Pattern(const int *PatternType);
	virtual bool checkEmptyPos(
		const int& x, 
		const int& y, 
		const int& start, 
		const int& mainColor, 
		const Pos* emptyPos);
	// ��������Ƿ���Ч�����Է����Ľ��������Ӧ�ļӷ�
	virtual bool checkStone(
		const int& x, 
		const int& y, 
		const bool& below4);

	// ��û��ƥ�䵽��ʱ���ȡһ�����ʵ�λ��
	int FindPosition();
};


