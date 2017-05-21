#pragma once
#ifndef AI1_H_INCLUDED
#define AI1_H_INCLUDED

#include <time.h>
#include <stdlib.h>
#include <conio.h>
#include "../chessBoard_Header/AI.h"
#include "PointStyle.h"
///AI��
class AI1 : public AI, private PointStyle
{
public:
	AI1();//���캯��
	void GetPosition(int &line, int &column, int onTurn);//��ȡ����λ��
	void InitializeD();

private:
	int OT;//�������������ˣ���GetPosition.cpp�ļ���ʹ��

		   //��¼��������ֵ��������ʴӡ�1����ʼ������˳��Ϊ�����к��С���
		   //��0����ʾû�����ӣ���1����ʾ���ӣ���2����ʾ����
	int cross[10][10];//����״̬
	bool Cross[10][10];//������ʶ
	double PointStyle[18];//��¼ÿ����������͵ķ�ֵ
	int PlayerNumber;//��¼��AI�����1�������2
	double Score[10][10];//��¼ÿ��λ���ϵķ�ֵ
	int MaxScorePosition;//����ֵ��λ��
	double MaxScore;//��¼����ֵ

	void Display(int n, int line, int column);
	///�����ܺ��� ������FunctionPart.cpp�ļ���
	int GetChessAmount(int row1, int row2, int who, int &num);//�鿴�ض�λ�õ��ĸ������ж��ٸ�����
	void GetMaxScorePosition();//��ȡ����ֵ��λ��

							   ///����λ�÷�ֵ���ú��� ������UpdateScore.cpp�ļ���
							   //��������λ�÷�ֵ���������У��У����Ӳ���
	void UpdateScore(int row1, int row2, int who, bool isAddScore = true);//����RecordSpecialPoint()����
																		  //���������Ӱ��ķ�ֵ
	void ResetScore(int row1, int row2, int score, int NumberPoor = 100, bool isAddScore = true);//ʵ�мӷֵĺ���
																								 //��������
	void RecordSpecialPoint(int row1, int row2, int who, bool isAddScore = true);//��SetCurrentPoint()���õ�ǰ�㣬�ٵ���AddRecord
	void AddRecord(int n, int n1, int n2, int n3, int n4, int amo, bool isAddScore = true);
	void SetCurrentPoint(int ThisFormatStyle, int ThisFormatScorePoor, int ThisSpecialStyle, int ThisSpecialScorePoorint, int LastFormatStyle = 0, int LastFormatScorePoore = 0, int LastSpecialPointStyle = 0, int LastSpecialScorePoor = 0);

	///�ж��Ƿ�������λ��   ������DeadCheck.cpp�ļ���
	bool DeadCheck(int line, int column, int who);
	bool CheckPosition(int OriLine, int OriColumn, int CLine, int CColumn, int player, bool &Position);
	bool Besieg(int RivalLine, int RivalColumn, int player, int rival);
	bool BesiegePosition(int RivalLine, int RivalColumn, int player, int rival, bool &tie);
	void reduceRecursionTimes();

	///��RateResetScore.cpp�ļ���
	void RateResetScore(float ResetRate, bool add = true);//��������С��ֵ�����ٷֲ��ȷ��λ
};


#endif // AI1_H_INCLUDED
