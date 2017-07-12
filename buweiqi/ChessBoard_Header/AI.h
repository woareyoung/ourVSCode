#pragma once
#ifndef AI_H_INCLUDED
#define AI_H_INCLUDED
#include "SIP.h"
#include "../FileSystem_Header/FileSystem.h"
#include <math.h>

class AI
{
public:
	//��¼��������ֵ��������ʴӡ�1����ʼ������˳��Ϊ�����к��С���
	//��0����ʾû�����ӣ���1����ʾ���ӣ���2����ʾ����
	int cross[10][10];
	int PlayerId;// ��ǰAI������Ǻ��ӻ��ǰ���

	FileSystem FS; //�ļ���������
	QUADRANT Qua; //��¼�����¼
	SITUATION NowStatus;//��ǰ����״��
	int CurrentRound;//��ǰ�غ���
	int CurrentNull;//��λ���������ã�

	virtual void GetPosition(int &line, int &column, int onTurn) = 0;
	/*
		����line�������λ�ã��У�
		����column�������λ�ã��У�
		����None���Ƿ�û��һģһ���ļ�¼
	*/
	bool MatchMemory(int line, int column, std::set<int> &res);//ƥ�����
	/*
		����line�������λ�ã��У�
		����column�������λ�ã��У�
	*/
	void Statistic(int line, int column);//ͳ�����̸����������������
	/*
	    ����maxQuadrant������������
	*/
	void GetCurrentStatus(int maxQuadrant, SITUATION &CurrentStatus);//��ȡ��ǰ����ѹ������
	/*
	    ����site��ģ������λ��
	*/
	double ProbabilityCount(int site);//�������
	/*
		����line�������λ�ã��У�
		����column�������λ�ã��У�
	*/
	void BackQua(int line, int column)//ͳ�����̸����������������
	{
		if (line < 5 && column > 5) Qua.FirstQuadrant--;
		else if (line < 5 && column < 5) Qua.SecondQuadrant--;
		else if (line > 5 && column < 5) Qua.ThirdQuadrant--;
		else if (line > 5 && column > 5) Qua.ForthQuadrant--;
	}
};

#endif // AI_H_INCLUDED
