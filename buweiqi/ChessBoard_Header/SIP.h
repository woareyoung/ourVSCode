#pragma once
#include <string>
#include <memory>
#include <iostream>
///���ڽ����¼��ʷ����
struct PACE
{
	int line;
	int column;
	int player;
	std::shared_ptr<PACE> next;
	std::shared_ptr<PACE> perior;
};
///��¼��ǰ����״��
struct SITUATION
{
	std::string BoardStatus = "";//����״��
	int ChessPosition;//����λ��
	SITUATION &operator= (SITUATION &param)
	{
		BoardStatus = param.BoardStatus;
		ChessPosition = param.ChessPosition;
		return *this;
	}
};
///
struct QUADRANT
{
	int FirstQuadrant = 0;//��һ��������������Ӧ����ϵ�еĵ������ޣ�
	int SecondQuadrant = 0;//�ڶ���������������Ӧ����ϵ�еĵڶ����ޣ�
	int ThirdQuadrant = 0;//������������������Ӧ����ϵ�еĵ�һ���ޣ�
	int ForthQuadrant = 0;//������������������Ӧ����ϵ�еĵ������ޣ�
	///��ȡ��������������
	int GetMaxQuadrant()
	{
		int max = FirstQuadrant;
		if (SecondQuadrant > max) max = SecondQuadrant;
		if (ThirdQuadrant > max) max = ThirdQuadrant;
		if (ForthQuadrant > max) max = ForthQuadrant;
		return max;
	}
};

struct FileName
{
	std::string TXT = ".txt";
	std::string ForeName = "AIMemory\\Round";
};

struct NEXTPACE
{
	int site;
	std::shared_ptr<NEXTPACE> next;
};

struct DISKSTATUS
{
	int Value[10];
	int count;
	std::shared_ptr<DISKSTATUS> next;
	std::shared_ptr<DISKSTATUS> prior;
	std::shared_ptr<DISKSTATUS> JumpPtr;
};