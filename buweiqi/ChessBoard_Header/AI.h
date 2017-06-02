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
	int CurrentRound;//��ǰ�غ���

	virtual void GetPosition(int &line, int &column, int onTurn) = 0;
	/*
		����line�������λ�ã��У�
		����column�������λ�ã��У�
	*/
	NEXTPACE* MatchMemory(int line, int column);//ƥ�����
	/*
		����line�������λ�ã��У�
		����column�������λ�ã��У�
	*/
	void Statistic(int line, int column);//ͳ�����̸����������������

};

#endif // AI_H_INCLUDED
