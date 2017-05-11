#pragma once
#ifndef CHESS_H_INCLUDED
#define CHESS_H_INCLUDED
struct PACE
{
	int line;
	int column;
	int player;
	PACE *next;
	PACE *perior;
};
class Chess
{
public:
	bool Cross[10][10];
	int cross[10][10];
	///设置状态 在recursion.cpp文件中
	void reduceRecursionTimes();
	void setStatus(int RivalLine, int RivalColumn);
	PACE *Tail;
};
#endif // CHESS_H_INCLUDED
