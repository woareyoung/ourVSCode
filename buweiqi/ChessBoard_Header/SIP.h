#pragma once
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