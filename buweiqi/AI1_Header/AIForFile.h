#pragma once

#ifndef AIFORFILE_H_INCLUDED
#define AIFORFILE_H_INCLUDED
#include "../ChessBoard_Header/SIP.h"
#include "../FileSystem_Header/FileSystem.h"
#include <unordered_map>

class AIForFile
{
public :
	//��¼��������ֵ��������ʴӡ�1����ʼ������˳��Ϊ�����к��С���
	//��0����ʾû�����ӣ���1����ʾ���ӣ���2����ʾ����
	int cross[10][10];
	int PlayerId;// ��ǰAI������Ǻ��ӻ��ǰ���

	FileSystem FS; //�ļ���������
	QUADRANT Qua; //��¼�����¼
	SITUATION NowStatus;//��ǰ����״��
	int CurrentRound;//��ǰ�غ���
	int CurrentNull;//��λ���������ã�
	bool ForeReadFinish;//��¼Ԥ���ļ��Ƿ����
	bool ForeReadHaveMem;//���Ԥ���ļ����Ƿ�������
	std::unordered_multimap<std::string, std::pair<int, int>> ForeReadContent;//���Ԥ������

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
	void Statistic(int line, int column)//ͳ�����̸����������������
	{
		if (line < 5 && column > 5) Qua.FirstQuadrant++;
		else if (line < 5 && column < 5) Qua.SecondQuadrant++;
		else if (line > 5 && column < 5) Qua.ThirdQuadrant++;
		else if (line > 5 && column > 5) Qua.ForthQuadrant++;
	}
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
	///�ж��Ƿ�������λ��   ������DeadCheck.cpp�ļ���
	/*
	����line:�����λ�á�����
	����column:�����λ�á�����
	����who:˭ �µ���
	����CROSS:��ǰ����״�������ڽ�����̵߳ĳ�ͻ��
	*/
	bool DeadCheck(int line, int column, int who, int CROSS[][10]);
	/*
	param[round]:��ҪԤ���Ļغ���
	*/
	void ForeReadFileToMemory(int round);//Ԥ���ļ�
};

#endif //AIFORFILE_H_INCLUDED