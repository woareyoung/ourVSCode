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
	int CurrentNull;//��λ����

	virtual void GetPosition(int &line, int &column, int onTurn) = 0;
	/*
		����line�������λ�ã��У�
		����column�������λ�ã��У�
		����None���Ƿ�û��һģһ���ļ�¼
	*/
	std::shared_ptr<NEXTPACE> MatchMemory(int line, int column, bool& None);//ƥ�����
	/*
		����line�������λ�ã��У�
		����column�������λ�ã��У�
	*/
	void Statistic(int line, int column);//ͳ�����̸����������������
	/*
		����head�������ͷ���
	*/
	void ClearList(std::shared_ptr<NEXTPACE> head);//�������
	/*
	    ����maxQuadrant������������
	*/
	void GetCurrentStatus(int maxQuadrant);//��ȡ��ǰ����ѹ������
};

#endif // AI_H_INCLUDED
