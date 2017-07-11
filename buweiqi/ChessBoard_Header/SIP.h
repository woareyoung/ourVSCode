#pragma once
#include <string>
#include <memory>
#include <iostream>
///用于界面记录历史走棋
struct PACE
{
	int line;
	int column;
	int player;
	std::shared_ptr<PACE> next;
	std::shared_ptr<PACE> perior;
};
///记录当前盘面状况
struct SITUATION
{
	int Line[10] = { 0 };//9行的记录（其中数组第0个记录下棋的位置）
	std::shared_ptr<SITUATION> next;
	std::shared_ptr<SITUATION> prior;
	std::shared_ptr<SITUATION> fourPtr;//后4个结点
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
			help2 = Line[i] / 10000; //获取高位
			help1 = Line[i] % 10000; //获取低位
			Line[i] = help2 + help1 * 10000;
		}
	}
};
///
struct QUADRANT
{
	int FirstQuadrant = 0;//第一区域棋子数（对应坐标系中的第三象限）
	int SecondQuadrant = 0;//第二区域棋子数（对应坐标系中的第二象限）
	int ThirdQuadrant = 0;//第三区域棋子数（对应坐标系中的第一象限）
	int ForthQuadrant = 0;//第四区域棋子数（对应坐标系中的第四象限）
	///获取棋子数最多的象限
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