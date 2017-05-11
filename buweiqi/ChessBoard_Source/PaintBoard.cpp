#include "../stdafx.h"
#include "../ChessBoard_Header/ChessBoard.h"

void ChessBoard::PaintBoard()
{
	int i, j;
	SelectObject(hdcBuffer, Board);
	StretchBlt(hdc, 0, 0, Base, Base, hdcBuffer, 0, 0, 500, 500, SRCCOPY);//将兼容设备复制到显示设备上
																		  //用于重绘
																		  //画棋子
	for (i = 1; i < 10; i++)
	{
		for (j = 1; j < 10; j++)
		{
			// 黑子
			if (cross[i][j] == isBlack)
			{
				SelectObject(hdcBuffer, BlackChess);
				StretchBlt(hdc, CrossCross[j] - ChessDiameter / 2, CrossCross[i] - ChessDiameter / 2, ChessDiameter, ChessDiameter, hdcBuffer, 0, 0, 200, 200, SRCCOPY);//将兼容设备复制到显示设备上
			}
			// 白子
			else if (cross[i][j] == isWhite)
			{
				SelectObject(hdcBuffer, WhiteChess);
				StretchBlt(hdc, CrossCross[j] - ChessDiameter / 2, CrossCross[i] - ChessDiameter / 2, ChessDiameter, ChessDiameter, hdcBuffer, 0, 0, 200, 200, SRCCOPY);//将兼容设备复制到显示设备上
			}
		}
	}
	if (Start == true && line != 0 && column != 0)
	{
		SelectObject(hdcBuffer, Tips);
		StretchBlt(hdc, CrossCross[column] - ChessDiameter / 5, CrossCross[line] - ChessDiameter / 5, ChessDiameter * 2 / 5, ChessDiameter * 2 / 5, hdcBuffer, 0, 0, 126, 126, SRCCOPY);//将兼容设备复制到显示设备上
	}
}

void ChessBoard::showChessBroadInfoOnDOS()
{
	int countB = 0;
	int countW = 0;
	_cprintf("------------------Chess Broad---------------------\n");
	for (int i = 1; i < 10; i++)
	{
		for (int j = 1; j < 10; j++)
		{
			_cprintf("%d\t", cross[i][j]);
			if (cross[i][j] == isWhite)
			{
				countW++;
			}
			else if (cross[i][j] == isBlack)
			{
				countB++;
			}
		}
		_cprintf("\n");
	}
	_cprintf("----Black:%d,White:%d----\n", countB, countW);
}
///回退一步功能函数
void ChessBoard::BackPace()
{
	if (!Start) return;//如果未开始游戏，按钮无效
	if (Tail == NULL) return;
	PACE *p;
	switch (Player1isAI || Player2isAI)
	{
		//如果有一方是AI，则回退两步
	case true:
		cross[Tail->line][Tail->column] = 0;
		p = Tail;
		Tail = Tail->perior;
		delete p;
		p = NULL;
		Tail->next = NULL;
		if (Player1AI == &ai1) Player1AI->GetPosition(line, column, 100);
		else if(Player2AI == &ai1) Player1AI->GetPosition(line, column, 200);
		line = Tail->line;
		column = Tail->column;
		//如果两方都是玩家，则回退一步
	case false:
		cross[Tail->line][Tail->column] = 0;
		if (Player1AI == &ai1) Player1AI->GetPosition(line, column, 100);
		else if (Player2AI == &ai1) Player1AI->GetPosition(line, column, 200);
		if (Tail->perior == NULL)
		{
			delete Tail;
			Tail = NULL;
			line = 0;
			column = 0;
			TipPlayer(isWhite);
		}
		else
		{
			p = Tail;
			Tail = Tail->perior;
			delete p;
			p = NULL;
			Tail->next = NULL;
			line = Tail->line;
			column = Tail->column;
			TipPlayer(Tail->player);
		}
		break;
	}
	RePaint();
}
void ChessBoard::TipPlayer(int Type)
{
	if (Type == isBlack)
	{
		ShowWindow(TurnToBlack, SW_HIDE);
		ShowWindow(TurnToWhite, SW_SHOW);
		UpdateWindow(TurnToWhite);
		Round2 = 60;
	}
	else if (Type == isWhite)
	{
		ShowWindow(TurnToWhite, SW_HIDE);
		ShowWindow(TurnToBlack, SW_SHOW);
		UpdateWindow(TurnToBlack);
		Round1 = 60;
	}
}