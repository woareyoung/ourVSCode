#include "../stdafx.h"
#include "../ChessBoard_Header/ChessBoard.h"

void ChessBoard::PaintBoard()
{
	int i, j;
	SelectObject(hdcBuffer, Board);
	StretchBlt(hdc, 0, 0, Base, Base, hdcBuffer, 0, 0, 500, 500, SRCCOPY);//将兼容设备复制到显示设备上
	//用于重绘
	//画棋子
	for (i = 1; i < 10; ++i)
	{
		for (j = 1; j < 10; ++j)
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

///回退一步功能函数
void ChessBoard::BackPace()
{
	if (Tail == nullptr) return;//如果已经没棋了，就不能再回退了
	std::shared_ptr<SITUATION> s = nullptr;
	std::shared_ptr<PACE> p = nullptr;
	switch (Player1isAI || Player2isAI)
	{
		//如果有一方是AI，则回退两步
	case true:
		if (Player1isAI && Tail->perior == nullptr) return;
		cross[Tail->line][Tail->column] = 0;
		p = Tail;
		Tail = Tail->perior;
		p = nullptr;
		Tail->next = nullptr;
		if (Player1isAI) Player1AI->GetPosition(line, column, 100);
		if (Player2isAI) Player2AI->GetPosition(line, column, 200);
		line = Tail->line;
		column = Tail->column;
		s = TempTail;
		TempTail = TempTail->prior;
		s = nullptr;
		TempTail->next = nullptr;
		if (onTurn == isPlay1onTurn)
		{
			onTurn = isAI2onTurn;
			DisplayOnTurn(1);
		}
		else if (onTurn == isPlay2onTurn)
		{
			onTurn = isAI1onTurn;
			DisplayOnTurn(2);
		}
		//如果两方都是玩家，则回退一步
	case false:
		cross[Tail->line][Tail->column] = 0;
		if (Player1isAI) Player1AI->GetPosition(line, column, 100);
		if (Player2isAI) Player2AI->GetPosition(line, column, 200);
		if (Tail->perior == nullptr)
		{
			Tail = nullptr;
			line = 0;
			column = 0;
			TempTail = nullptr;
			SituaHead = nullptr;
		}
		else
		{
			p = Tail;
			Tail = Tail->perior;
			p = nullptr;
			Tail->next = nullptr;
			line = Tail->line;
			column = Tail->column;
			s = TempTail;
			TempTail = TempTail->prior;
			s = nullptr;
			TempTail->next = nullptr;
		}
		if (onTurn == isAI1onTurn)
		{
			onTurn = isPlay2onTurn;
			DisplayOnTurn(1);
		}
		else if (onTurn == isAI2onTurn)
		{
			onTurn = isPlay1onTurn;
			DisplayOnTurn(2);
		}
		else if (onTurn == isPlay1onTurn)
		{
			if (Player2isAI) onTurn = isAI2onTurn;
			else onTurn = isPlay2onTurn;
			DisplayOnTurn(1);
		}
		else if (onTurn == isPlay2onTurn)
		{
			if (Player1isAI) onTurn = isAI1onTurn;
			else onTurn = isPlay1onTurn;
			DisplayOnTurn(2);
		}
		break;
	}
	RePaint();
}