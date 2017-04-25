#include "../stdafx.h"
#include "../ChessBoard_Header/ChessBoard.h"
///�����Ӻ���
void ChessBoard::PaintChess()
{
	// �ж��ֵ���һ��
	switch (onTurn)
	{
	case isPlay1onTurn://����ɫ����
		for (int i = 0; i < 10; i++)
			for (int j = 0; j < 10; j++)
				Cross[i][j] = false;
		if (cross[line][column] == 0)
		{
			if (Player2isAI == false)
				onTurn = isPlay2onTurn;
			else
				onTurn = isAI2onTurn;
			PaintAChess(1);
			//���ѷ�ʤ�����������Ϸ
			if (WinOrLose() == true)
			{
				Winner = 2;
				ReStart();
			}
			else if (Player2isAI == true) PaintChess();
		};
		break;
	case isPlay2onTurn://����ɫ����
		for (int i = 0; i < 10; i++)
			for (int j = 0; j < 10; j++)
				Cross[i][j] = false;
		if (cross[line][column] == 0)
		{
			if (Player1isAI == false)
				onTurn = isPlay1onTurn;
			else
				onTurn = isAI1onTurn;
			PaintAChess(2);
			//���ѷ�ʤ�����������Ϸ
			if (WinOrLose() == true)
			{
				Winner = 1;
				ReStart();
			}
			else if (Player1isAI == true) PaintChess();
		};
		break;
		//����ɫ����
	case isAI1onTurn:
	{
		if (Player2isAI == true)
			onTurn = isAI2onTurn;
		else
			onTurn = isPlay2onTurn;

		///�ڴ˵���AI���򣬷��������λ�ã��ڼ��У��ڼ��У�
		Player1AI->GetPosition(line, column, onTurn);

		PaintAChess(isBlack);
		//���Է��ǵ��ԣ������ж���û�зֳ�ʤ��
		bool win = WinOrLose();
		if (Player2isAI == true && !win && line != 0 && column != 0)
			PaintChess();//���Է��ǵ��ԣ���ݹ�
		else if (win || line == 0 || column == 0)
		{
			Winner = 2;
			ReStart();
		}
	};
	break;
	//����ɫ����
	case isAI2onTurn:
	{
		if (Player1isAI == true)
			onTurn = isAI1onTurn;
		else
			onTurn = isPlay1onTurn;

		///�ڴ˵���AI���򣬷��������λ�ã��ڼ��У��ڼ��У�
		Player2AI->GetPosition(line, column, onTurn);

		PaintAChess(isWhite);
		//���Է��ǵ��ԣ������ж���û�зֳ�ʤ��
		bool win = WinOrLose();
		if (Player1isAI == true && !win && line != 0 && column != 0)
			PaintChess();//���Է��ǵ��ԣ���ݹ�
		else if (win || line == 0 || column == 0)
		{
			Winner = 1;
			ReStart();
		}
	};
	break;
	}
}
///ʵ�л�����
void ChessBoard::PaintAChess(int type)
{
	cross[line][column] = type;//����AI�����ȡ�����к�������¼��line��column
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
	RePaint();
}
///��ȡ����λ��
bool ChessBoard::GetPointPosition(DWORD lParam)
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
		return true;
	}
	return false;
}
