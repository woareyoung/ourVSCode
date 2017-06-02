#pragma once
#include <string>
///用于界面记录历史走棋
struct PACE
{
	int line;
	int column;
	int player;
	PACE *next;
	PACE *perior;
};
///记录当前盘面状况
struct SITUATION
{
	double Line[10] = { 0 };//9行的记录（其中数组第0个记录下棋的位置）
	SITUATION *next;
	SITUATION *prior;
};
///
struct QUADRANT
{
	int FirstQuadrant = 0;//第一象限棋子数
	int SecondQuadrant = 0;//第二象限棋子数
	int ThirdQuadrant = 0;//第三象限棋子数
	int ForthQuadrant = 0;//第四象限棋子数
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
	std::string AIMemory = "AIMemory.txt";
	std::string TXT = ".txt";
	std::string ForeName = "AIMemory\\Round";
};

struct NEXTPACE
{
	int site;
	NEXTPACE *next;
};