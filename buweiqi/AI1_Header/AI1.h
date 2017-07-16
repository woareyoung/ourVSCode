#pragma once
#ifndef AI1_H_INCLUDED
#define AI1_H_INCLUDED

#include <time.h>
#include <stdlib.h>
#include <conio.h>
#include "../chessBoard_Header/AI.h"
#include <stack>
#include <set>
#include "PointStyle.h"
#include "../FileSystem_Header/FileSystem.h"
#include "../ChessBoard_Header/SIP.h"
#include "AIForFile.h"

///AI��
class AI1 : public AI, private PointStyle, private AIForFile
{
public:
	AI1();//���캯��
	void GetPosition(int &line, int &column, int onTurn);//��ȡ����λ��
	void InitializeD();
private:
	int OT;//�������������ˣ���GetPosition.cpp�ļ���ʹ��
	int Similar;//���ȥ��ľ������Ƶĳ̶�
	//��¼��������ֵ��������ʴӡ�1����ʼ������˳��Ϊ�����к��С���
	//��0����ʾû�����ӣ���1����ʾ���ӣ���2����ʾ����
	bool Cross[10][10];//������ʶ
	int DoubleDontDead;//˫����������λ������
	int MaxScorePosition;//����ֵ��λ��
	double PointStyle[18];//��¼ÿ����������͵ķ�ֵ
	double Score[10][10];//��¼ÿ��λ���ϵķ�ֵ
	double MaxScore;//��¼����ֵ
	
	std::set<int> np;//��ȡ��һ���Ŀ����Եļ���
	/*
		param[np]:��������λ�õ�set����
	*/
	int GetNextPace(std::set<int>& np);//��������ѡȡ���ʤ�ʵĽ��
	///�����ܺ��� ������FunctionPart.cpp�ļ���
	/*
		param[row1]:��
		param[row2]:��
		param[who]:��ұ��
		param[num]:��Χ�Է����ӵ�����
		return:��λ���
	*/
	int GetChessAmount(int row1, int row2, int who, int &num);//�鿴�ض�λ�õ��ĸ������ж��ٸ�����
	void GetMaxScorePosition();//��ȡ����ֵ��λ��

	///����λ�÷�ֵ���ú��� ������UpdateScore.cpp�ļ���
	//��������λ�÷�ֵ���������У��У��Ƿ�ӷ�
	void UpdateScore(int row1, int row2, int who, bool isAddScore = true);//����RecordSpecialPoint()����
	//���������Ӱ��ķ�ֵ
	void ResetScore(int row1, int row2, double score, double NumberPoor = 100, bool isAddScore = true);//ʵ�мӷֵĺ���
	//��������
	void RecordSpecialPoint(int row1, int row2, int who, bool isAddScore = true);//��SetCurrentPoint()���õ�ǰ�㣬�ٵ���AddRecord
	void AddRecord(int n, int n1, int n2, int n3, int n4, int amo, bool isAddScore = true);
	void SetCurrentPoint(int ThisFormatStyle, double ThisFormatScorePoor, int ThisSpecialStyle, double ThisSpecialScorePoorint, int LastFormatStyle = 0, double LastFormatScorePoore = 0, int LastSpecialPointStyle = 0, double LastSpecialScorePoor = 0);

	///��RateResetScore.cpp�ļ���
	void RateResetScore(double ResetRate, bool add = true);//��������С��ֵ�����ٷֲ��ȷ��λ
	//--------------------------------------------------------------------------------//
	/*
	param[line]:����λ�á�����
	param[column]:����λ�á�����
	*/
	double CalDeadPosNumber(int line, int column, int &deadPos, bool Cal = true);//��������λ������
	/*
	param[line]:ת�����λ�á�����
	param[column]��ת�����λ�á�����
	param[site]����Ҫת����λ��
	*/
	void SymmetryExchange(int &line, int &column, int site);//����¼�е�λ��ת��Ϊ���ʵ�ǰ�����λ��

};


#endif // AI1_H_INCLUDED
