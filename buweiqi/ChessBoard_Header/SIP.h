#pragma once
#include <string>
///���ڽ����¼��ʷ����
struct PACE
{
	int line;
	int column;
	int player;
	PACE *next;
	PACE *perior;
};
///��¼��ǰ����״��
struct SITUATION
{
	double Line[10] = { 0 };//9�еļ�¼�����������0����¼�����λ�ã�
	SITUATION *next;
	SITUATION *prior;
};
///
struct QUADRANT
{
	int FirstQuadrant = 0;//��һ����������
	int SecondQuadrant = 0;//�ڶ�����������
	int ThirdQuadrant = 0;//��������������
	int ForthQuadrant = 0;//��������������
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
	std::string AIMemory = "AIMemory.txt";
	std::string TXT = ".txt";
	std::string ForeName = "AIMemory\\Round";
};

struct NEXTPACE
{
	int site;
	NEXTPACE *next;
};