#include "../ChessBoard_Header/Chess.h"
#include "../ChessBoard_Header/ChessBoard.h"
#include <memory>
#include <iostream>

void ChessBoard::AddSituationList()
{
	///新建结点
	if (SituaHead == nullptr)
	{
		TempTail = std::shared_ptr<SITUATION>(new SITUATION);
		TempTail->next = nullptr;
		TempTail->prior = nullptr;
		TempTail->fourPtr = nullptr;
		SituaHead = TempTail;
	}
	else
	{
		std::shared_ptr<SITUATION> s = std::shared_ptr<SITUATION>(new SITUATION);
		TempTail->next = s;
		s->prior = TempTail;
		TempTail = s;
		TempTail->next = nullptr;
		TempTail->fourPtr = nullptr;
	}
	TempTail->Line[0] = line * 10 + column;//记录本次走棋位置
	if (CurrentRoundNumber > 4 && CurrentRoundNumber % 5 == 0)
	{
		TempTail->prior->prior->prior->prior->fourPtr = TempTail;
	}
	CurrentRoundNumber++;
	int maxQuadrant = Qua.GetMaxQuadrant();//获取最多棋子的象限
	int i, j;
	int nnn;
	//如果最多棋子的是第一象限
	if (maxQuadrant == Qua.FirstQuadrant)
	{
		for (i = 1; i < 10; ++i)
		{
			nnn = 1;
			for (j = 9; j > 0; j--)
			{
				nnn *= 2;
				if (cross[i][j] == 1) TempTail->Line[i] += nnn * 10000;
				else if (cross[i][j] == 2) TempTail->Line[i] += nnn;
			}
		}
	}
	//如果最多棋子的是第二象限
	else if (maxQuadrant == Qua.SecondQuadrant)
	{
		for (i = 1; i < 10; ++i)
		{
			nnn = 1;
			for (j = 1; j < 10; ++j)
			{
				nnn *= 2;
				if (cross[i][j] == 1) TempTail->Line[i] += nnn * 10000;
				else if (cross[i][j] == 2) TempTail->Line[i] += nnn;
			}
		}
	}
	//如果最多棋子的是第三象限
	else if (maxQuadrant == Qua.ThirdQuadrant)
	{
		for (i = 9; i > 0; i--)
		{
			nnn = 1;
			for (j = 1; j < 10; ++j)
			{
				nnn *= 2;
				if (cross[i][j] == 1) TempTail->Line[i] += nnn * 10000;
				else if (cross[i][j] == 2) TempTail->Line[i] += nnn;
			}
		}
	}
	//如果最多棋子的是第四象限
	else if (maxQuadrant == Qua.ForthQuadrant)
	{
		for (i = 9; i > 0; i--)
		{
			nnn = 1;
			for (j = 9; j > 0; j--)
			{
				nnn *= 2;
				if (cross[i][j] == 1) TempTail->Line[i] += nnn * 10000;
				else if (cross[i][j] == 2) TempTail->Line[i] += nnn;
			}
		}
	}
	if (line < 5 && column > 5) Qua.FirstQuadrant++;
	else if (line < 5 && column < 5) Qua.SecondQuadrant++;
	else if (line > 5 && column < 5) Qua.ThirdQuadrant++;
	else if (line > 5 && column > 5) Qua.ForthQuadrant++;
}

void ChessBoard::AddList(int type)
{
	//添加到记录
	if (Tail == nullptr)
	{
		Tail = std::shared_ptr<PACE>(new PACE);
		Tail->line = line;
		Tail->column = column;
		Tail->player = type;
		Tail->next = nullptr;
		Tail->perior = nullptr;
	}
	else
	{
		std::shared_ptr<PACE> p = std::shared_ptr<PACE>(new PACE);
		p->line = line;
		p->column = column;
		p->player = type;
		p->next = nullptr;
		p->perior = Tail;
		Tail->next = p;
		Tail = p;
	}
	AddSituationList();
}