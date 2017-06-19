#include "../ChessBoard_Header/Chess.h"
#include "../ChessBoard_Header/ChessBoard.h"
#include <math.h>

void ChessBoard::AddSituationList()
{
	if (line < 5 && column > 5) Qua.FirstQuadrant++;
	else if (line < 5 && column < 5) Qua.SecondQuadrant++;
	else if (line > 5 && column < 5) Qua.ThirdQuadrant++;
	else if (line > 5 && column > 5) Qua.ForthQuadrant++;
	///新建结点
	if (SituaHead == NULL)
	{
		TempTail = new SITUATION;
		TempTail->next = NULL;
		TempTail->prior = NULL;
		SituaHead = TempTail;
	}
	else
	{
		SITUATION *s = new SITUATION;
		TempTail->next = s;
		s->prior = TempTail;
		TempTail = s;
		TempTail->next = NULL;
	}
	TempTail->Line[0] = line * 10 + column;//记录本次走棋位置

	int maxQuadrant = Qua.GetMaxQuadrant();//获取最多棋子的象限
	int i, j;
	double value;
	//如果最多棋子的是第一象限
	if (maxQuadrant == Qua.FirstQuadrant)
	{
		for (i = 1; i < 10; i++)
		{
			value = 0;
			for (j = 9; j > 0; j--)
			{
				if (cross[i][j] == 1) value += pow(2, 10 - j) * 10000;
				else if (cross[i][j] == 2) value += pow(2, 10 - j);
			}
			TempTail->Line[i] = value;
		}
	}
	//如果最多棋子的是第二象限
	else if (maxQuadrant == Qua.SecondQuadrant)
	{
		for (i = 1; i < 10; i++)
		{
			value = 0;
			for (j = 1; j < 10; j++)
			{
				if (cross[i][j] == 1) value += pow(2, j) * 10000;
				else if (cross[i][j] == 2) value += pow(2, j);
			}
			TempTail->Line[i] = value;
		}
	}
	//如果最多棋子的是第三象限
	else if (maxQuadrant == Qua.ThirdQuadrant)
	{
		for (i = 9; i > 0; i--)
		{
			value = 0;
			for (j = 1; j < 10; j++)
			{
				if (cross[i][j] == 1) value += pow(2, j) * 10000;
				else if (cross[i][j] == 2) value += pow(2, j);
			}
			TempTail->Line[i] = value;
		}
	}
	//如果最多棋子的是第四象限
	else if (maxQuadrant == Qua.ForthQuadrant)
	{
		for (i = 9; i > 0; i--)
		{
			value = 0;
			for (j = 9; j > 0; j--)
			{
				if (cross[i][j] == 1) value += pow(2, 10 - j) * 10000;
				else if (cross[i][j] == 2) value += pow(2, 10 - j);
			}
			TempTail->Line[i] = value;
		}
	}
}

void ChessBoard::AddList(int type)
{
	//添加到记录
	if (Tail == NULL)
	{
		Tail = new PACE;
		Tail->line = line;
		Tail->column = column;
		Tail->player = type;
		Tail->next = NULL;
		Tail->perior = NULL;
	}
	else
	{
		PACE *p = new PACE;
		p->line = line;
		p->column = column;
		p->player = type;
		p->next = NULL;
		p->perior = Tail;
		Tail->next = p;
		Tail = p;
	}
	AddSituationList();
}