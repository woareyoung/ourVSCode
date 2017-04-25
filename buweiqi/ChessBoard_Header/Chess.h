#pragma once
#ifndef CHESS_H_INCLUDED
#define CHESS_H_INCLUDED

class Chess
{
public:
	bool Cross[10][10];
	int cross[10][10];
	///设置状态 在recursion.cpp文件中
	void reduceRecursionTimes();
	void setStatus(int RivalLine, int RivalColumn);
};
#endif // CHESS_H_INCLUDED
