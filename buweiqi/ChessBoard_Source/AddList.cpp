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
		SituaHead = TempTail;
	}
	else
	{
		std::shared_ptr<SITUATION> s = std::shared_ptr<SITUATION>(new SITUATION);
		TempTail->next = s;
		s->prior = TempTail;
		TempTail = s;
		TempTail->next = nullptr;
	}
	CurrentRoundNumber++;
	int maxQuadrant = Qua.GetMaxQuadrant();//获取最多棋子的象限
	int i, j;
	//如果最多棋子的是第一象限
	if (maxQuadrant == Qua.FirstQuadrant)
	{
		TempTail->ChessPosition = line * 10 + (10 - column);//记录本次走棋位置
		for (i = 1; i < 10; ++i)
			for (j = 9; j > 0; j--)
				TempTail->BoardStatus += std::to_string(cross[i][j]);
	}
	//如果最多棋子的是第二象限
	else if (maxQuadrant == Qua.SecondQuadrant)
	{
		TempTail->ChessPosition = line * 10 + column;//记录本次走棋位置
		for (i = 1; i < 10; ++i)
			for (j = 1; j < 10; ++j)
				TempTail->BoardStatus += std::to_string(cross[i][j]);
	}
	//如果最多棋子的是第三象限
	else if (maxQuadrant == Qua.ThirdQuadrant)
	{
		TempTail->ChessPosition = (10 - line) * 10 + column;//记录本次走棋位置
		for (i = 9; i > 0; i--)
			for (j = 1; j < 10; ++j)
				TempTail->BoardStatus += std::to_string(cross[i][j]);
	}
	//如果最多棋子的是第四象限
	else if (maxQuadrant == Qua.ForthQuadrant)
	{
		TempTail->ChessPosition = (10 - line) * 10 + (10 - column);//记录本次走棋位置
		for (i = 9; i > 0; i--)
			for (j = 9; j > 0; j--)
				TempTail->BoardStatus += std::to_string(cross[i][j]);
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