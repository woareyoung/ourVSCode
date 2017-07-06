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
	int Line[10] = { 0 };//9�еļ�¼�����������0����¼�����λ�ã�
	std::shared_ptr<SITUATION> next;
	std::shared_ptr<SITUATION> prior;
	SITUATION &operator= (SITUATION &param)
	{
		for (int i = 0; i < 10; i++) Line[i] = param.Line[i];
		return *this;
	}
	void DigitalChange()
	{
		int help1;
		int help2;
		for (int i = 1; i < 10; i++)
		{
			help2 = Line[i] / 10000; //��ȡ��λ
			help1 = Line[i] % 10000; //��ȡ��λ
			Line[i] = help2 + help1 * 10000;
		}
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