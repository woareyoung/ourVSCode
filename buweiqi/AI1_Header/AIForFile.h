#pragma once

#ifndef AIFORFILE_H_INCLUDED
#define AIFORFILE_H_INCLUDED
#include "../ChessBoard_Header/SIP.h"
#include "../FileSystem_Header/FileSystem.h"
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
	bool ForeReadFinish2;//��¼Ԥ���ļ��Ƿ����
	bool ForeReadHaveMem;//���Ԥ���ļ����Ƿ�������
	bool ForeReadHaveMem2;//���Ԥ���ļ����Ƿ�������
	std::unordered_multimap<std::string, std::pair<int, int>> ForeReadContent;//���Ԥ�����ݣ���һ�غϣ�
	std::unordered_multimap<std::string, std::pair<int, int>> ForeReadContent2;//���Ԥ�����ݣ��������غϣ�

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
	void Statistic(int line, int column, QUADRANT &q)//ͳ�����̸����������������
	{
		if (line < 5 && column > 5) q.FirstQuadrant++;
		else if (line < 5 && column < 5) q.SecondQuadrant++;
		else if (line > 5 && column < 5) q.ThirdQuadrant++;
		else if (line > 5 && column > 5) q.ForthQuadrant++;
	}
	/*
	����maxQuadrant������������
	*/
	void GetCurrentStatus(int maxQuadrant, SITUATION &CurrentStatus, QUADRANT &qua, int CROSS[][10]);//��ȡ��ǰ����ѹ������
	/*
	����line�������λ�ã��У�
	����column�������λ�ã��У�
	*/
	void BackQua(int line, int column, QUADRANT &q)//ͳ�����̸����������������
	{
		if (line < 5 && column > 5) q.FirstQuadrant--;
		else if (line < 5 && column < 5) q.SecondQuadrant--;
		else if (line > 5 && column < 5) q.ThirdQuadrant--;
		else if (line > 5 && column > 5) q.ForthQuadrant--;
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
	param[stl]:���ݴ�ŵ�����
	*/
	bool ForeReadFileToMemory(int round, std::unordered_multimap<std::string, std::pair<int, int>> &stl);//Ԥ���ļ�
	void ForeRead(bool &finish, int round, std::unordered_multimap<std::string, std::pair<int, int>> &Content, bool &HaveMem) 
	{
		finish = false;
		if (ForeReadFileToMemory(round, Content)) HaveMem = true;
		else HaveMem = false;
		finish = true;
	}
};

#endif //AIFORFILE_H_INCLUDED