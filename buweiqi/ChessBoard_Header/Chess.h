#pragma once
#ifndef CHESS_H_INCLUDED
#define CHESS_H_INCLUDED
#include "SIP.h"
#include "../FileSystem_Header/FileSystem.h"
#include <list>
#include <stack>

class Chess
{
public:
	int cross[10][10];
//	int ChessAmount[3];//棋子数量
	FileSystem FileS;
	std::list<SITUATION> MemBattle;
	std::list<PACE> MemPace;

	QUADRANT Qua;

	/*
	param[line]:下棋位置——行
	param[column]:下棋位置——列
	param[who]:谁下的棋子
	*/
	bool GoDie(int line, int column, int who)
	{
		//为了适应多线程，另定义一个变量，解决访问冲突
//		int BoardCross[10][10];
//		int i, j;//遍历变量
		int rival = 3 - who;//对方
		int saveLine = line, saveColumn = column;
		/*
		for (i = 0; i < 10; i++)
			for (j = 0; j < 10; j++)
				BoardCross[i][j] = cross[i][j];*/
		bool VisitStatus[10][10] = { false };//标记是否已遍历过
		std::stack<std::pair<int, int>> position;
		std::pair<int, int> p;
		/*
		auto InitVisit = [&]() {
			for(i = 0; i < 10; i++)
				for(j = 0; j < 10; j++)
					VisitStatus[i][j] = false;
		};*/
		//弹出数据
		auto POPdata = [&]() {
			if (position.empty()) return false;
			p = position.top();
			line = p.first;
			column = p.second;
			position.pop();
			return true;
		};
		//压栈，并设置遍历
		auto PUSHdata = [&](int l, int c) {
			std::pair<int, int> tempP(line, column);
			position.push(tempP);
			line = l;
			column = c;
		};
		//还原数据
		auto BackData = [&]() {
			line = saveLine;
			column = saveColumn;
			while (!position.empty()) position.pop();
		};
		//跟踪遍历（判断有没有被围死）
		//param[Player]:检查 谁 的棋子被围死
		auto VisitAll = [&](int Player)
		{
			int Rival = 3 - Player;
			while (true)
			{
				VisitStatus[line][column] = true;
				//遇到空位，活下去了
				if (cross[line][column] == 0 && line > 0 && column > 0)
				{
					BackData();
					return false;
				}
				//如果上面有自己的棋子且未遍历过
				if (cross[line - 1][column] == Player && VisitStatus[line - 1][column] == false && line > 1)
				{
					PUSHdata(line - 1, column);
					continue;
				}
				else if (cross[line - 1][column] == 0 && line > 1)
				{
					line = line - 1;
					continue;
				}
				//如果下面有自己的棋子且未遍历过
				if (cross[line + 1][column] == Player && VisitStatus[line + 1][column] == false && line < 9)
				{
					PUSHdata(line + 1, column);
					continue;
				}
				else if (cross[line + 1][column] == 0 && line < 9)
				{
					line = line + 1;
					continue;
				}
				//如果左边有自己的棋子且未遍历过
				if (cross[line][column - 1] == Player && VisitStatus[line][column - 1] == false && column > 1)
				{
					PUSHdata(line, column - 1);
					continue;
				}
				else if (cross[line][column - 1] == 0 && column > 1)
				{
					column = column - 1;
					continue;
				}
				//如果右边有自己的棋子且未遍历过
				if (cross[line][column + 1] == Player && VisitStatus[line][column + 1] == false && column < 9)
				{
					PUSHdata(line, column + 1);
					continue;
				}
				else if (cross[line][column + 1] == 0 && column < 9)
				{
					column = column + 1;
					continue;
				}
				if (POPdata()) continue;
				BackData();
				return true;//能到这里证明挂了
			}
		};
		if (cross[line - 1][column] == rival && line > 1)
		{
			line = line - 1;
			if (VisitAll(rival)) return true;
		}
		if (cross[line + 1][column] == rival && line < 9)
		{
			line = line + 1;
			if (VisitAll(rival)) return true;
		}
		if (cross[line][column - 1] == rival && column > 1)
		{
			column = column - 1;
			if (VisitAll(rival)) return true;
		}
		if (cross[line][column + 1] == rival && column < 9)
		{
			column = column + 1;
			if (VisitAll(rival)) return true;
		}
		if (VisitAll(who)) return true;
		return false;
	}
};
#endif // CHESS_H_INCLUDED
