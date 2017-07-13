#include "../stdafx.h"
#include "../ChessBoard_Header/ChessBoard.h"
#include "../ChessBoard_Header/WinCheck.h"
///�����Ӻ���
void ChessBoard::PaintChess()
{
	// �ж��ֵ���һ��
	switch (onTurn)
	{
	case isPlay1onTurn://����ɫ����
		if (line == 0 || column == 0) return;
		if (cross[line][column] != 0) return;
		if (Player2isAI == false)
			onTurn = isPlay2onTurn;
		else
			onTurn = isAI2onTurn;
		PaintAChess(1);
		//���ѷ�ʤ�����������Ϸ
		if (GoDie(line, column, 1))
		{
			Winner = 2;
			ReStart();
		}
		/*
		if (chessInfo.WinOrLose(line, column, onTurn, Winner, cross))
		{
			ReStart();
		};*/
		break;
	case isPlay2onTurn://����ɫ����
		if (line == 0 || column == 0) return;
		if (cross[line][column] != 0) return;
		if (Player1isAI == false)
			onTurn = isPlay1onTurn;
		else
			onTurn = isAI1onTurn;
		PaintAChess(2);
		//���ѷ�ʤ�����������Ϸ
		if (GoDie(line, column, 2))
		{
			Winner = 1;
			ReStart();
		}/*
		if (chessInfo.WinOrLose(line, column, onTurn, Winner, cross))
		{
			ReStart();
		}*/
		break;
		//����ɫ����
	case isAI1onTurn:
	{
		///�ڴ˵���AI���򣬷��������λ�ã��ڼ��У��ڼ��У�
		if (Player2isAI == true)
		{
			Player1AI->GetPosition(line, column, isAI2onTurn);
			onTurn = isAI2onTurn;
		}
		else
		{
			Player1AI->GetPosition(line, column, isPlay2onTurn);
			onTurn = isPlay2onTurn;
		}

		PaintAChess(isBlack);
		//���Է��ǵ��ԣ������ж���û�зֳ�ʤ��
		if (GoDie(line, column, 1))
		{
			Winner = 2;
			ReStart();
		}/*
		bool win = chessInfo.WinOrLose(line, column, onTurn, Winner, cross);
		if (win || line == 0 || column == 0)
		{
			Winner = 2;
			ReStart();
		}*/
	};
	break;
	//����ɫ����
	case isAI2onTurn:
	{
		///�ڴ˵���AI���򣬷��������λ�ã��ڼ��У��ڼ��У�
		if (Player1isAI == true)
		{
			Player2AI->GetPosition(line, column, isAI1onTurn);
			onTurn = isAI1onTurn;
		}
		else
		{
			Player2AI->GetPosition(line, column, isPlay1onTurn);
			onTurn = isPlay1onTurn;
		}
		PaintAChess(isWhite);
		//���Է��ǵ��ԣ������ж���û�зֳ�ʤ��
		if (GoDie(line, column, 2))
		{
			Winner = 1;
			ReStart();
		}
		/*
		bool win = chessInfo.WinOrLose(line, column, onTurn, Winner, cross);
		if (win || line == 0 || column == 0)
		{
			Winner = 1;
			ReStart();
		}*/
	};
	break;
	}
}
///ʵ�л�����
void ChessBoard::PaintAChess(int type)
{
	AddList(type);
	cross[line][column] = type;//����ȡ�����к�������¼��line��column
	DisplayOnTurn(type);
	Repaint = false;
	RePaint();
	Repaint = true;
}
void ChessBoard::DisplayOnTurn(int type)
{
	if (type == isBlack)
	{
		ShowWindow(TurnToBlack, SW_HIDE);
		ShowWindow(TurnToWhite, SW_SHOW);
		UpdateWindow(TurnToWhite);
		Round2 = 60;
	}
	else if (type == isWhite)
	{
		ShowWindow(TurnToWhite, SW_HIDE);
		ShowWindow(TurnToBlack, SW_SHOW);
		UpdateWindow(TurnToBlack);
		Round1 = 60;
	}
}
///��ȡ����λ��
void ChessBoard::GetPointPosition(DWORD lParam)
{
	point.x = LOWORD(lParam);// �õ�һ��32bit���ĵ�16bit
	point.y = HIWORD(lParam);// �õ�һ��32bit���ĸ�16bit
	int a, b;
	b = (point.x - ChessDiameter / 2) / ChessDiameter + 1;
	a = (point.y - ChessDiameter / 2) / ChessDiameter + 1;
	// �ж��Ƿ�Խ��
	if (a < 10 && b < 10 && a > 0 && b > 0)
	{
		if (cross[a][b] == 0)
		{
			line = a;
			column = b;
		}
	}
}
