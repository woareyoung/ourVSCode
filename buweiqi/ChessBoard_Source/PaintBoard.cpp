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
