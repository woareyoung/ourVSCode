#include "../stdafx.h"
#include "../ChessBoard_Header/ChessBoard.h"

void ChessBoard::PaintBoard()
{
	int i, j;
	SelectObject(hdcBuffer, Board);
	StretchBlt(hdc, 0, 0, Base, Base, hdcBuffer, 0, 0, 500, 500, SRCCOPY);//�������豸���Ƶ���ʾ�豸��
	//�����ػ�
	//������
	for (i = 1; i < 10; ++i)
	{
		for (j = 1; j < 10; ++j)
		{
			// ����
			if (cross[i][j] == isBlack)
			{
				SelectObject(hdcBuffer, BlackChess);
				StretchBlt(hdc, CrossCross[j] - ChessDiameter / 2, CrossCross[i] - ChessDiameter / 2, ChessDiameter, ChessDiameter, hdcBuffer, 0, 0, 200, 200, SRCCOPY);//�������豸���Ƶ���ʾ�豸��
			}
			// ����
			else if (cross[i][j] == isWhite)
			{
				SelectObject(hdcBuffer, WhiteChess);
				StretchBlt(hdc, CrossCross[j] - ChessDiameter / 2, CrossCross[i] - ChessDiameter / 2, ChessDiameter, ChessDiameter, hdcBuffer, 0, 0, 200, 200, SRCCOPY);//�������豸���Ƶ���ʾ�豸��
			}
		}
	}
	if (Start == true && line != 0 && column != 0)
	{
		SelectObject(hdcBuffer, Tips);
		StretchBlt(hdc, CrossCross[column] - ChessDiameter / 5, CrossCross[line] - ChessDiameter / 5, ChessDiameter * 2 / 5, ChessDiameter * 2 / 5, hdcBuffer, 0, 0, 126, 126, SRCCOPY);//�������豸���Ƶ���ʾ�豸��
	}
}

///����һ�����ܺ���
void ChessBoard::BackPace()
{
	if (MemPace.empty()) return;
	PACE pac;
	auto isParamRight = [&]() {
		return Player1isAI || Player2isAI;
	};
	switch (isParamRight())
	{
		//�����һ����AI�����������
	case true:
		if (MemPace.empty()) return;
		if (Player1isAI && MemPace.size() == 1) return;
		pac = MemPace.back();
		cross[pac.line][pac.column] = 0;
		if (Player1isAI) Player1AI->GetPosition(line, column, 100);
		if (Player2isAI) Player2AI->GetPosition(line, column, 200);
		MemPace.pop_back();
		if (MemPace.empty()) line = 0, column = 0;
		else
		{
			pac = MemPace.back();
			line = pac.line, column = pac.column;
		}
		MemBattle.pop_back();
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
		//�������������ң������һ��
	case false:
		pac = MemPace.back();
		cross[pac.line][pac.column] = 0;
		if (Player1isAI) Player1AI->GetPosition(line, column, 100);
		if (Player2isAI) Player2AI->GetPosition(line, column, 200);
		MemPace.pop_back();
		if (MemPace.empty()) line = 0, column = 0;
		else
		{
			pac = MemPace.back();
			line = pac.line, column = pac.column;
		}
		MemBattle.pop_back();
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
