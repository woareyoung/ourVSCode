#include "../stdafx.h"
#include "../ChessBoard_Header/ChessBoard.h"
///画棋子函数
void ChessBoard::PaintChess()
{
	// 判断轮到哪一方
	switch (onTurn)
	{
	case isPlay1onTurn://画黑色棋子
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
			//若已分胜负，则结束游戏
			if (WinOrLose() == true)
			{
				Winner = 2;
				ReStart();
			}
			else if (Player2isAI == true) PaintChess();
		};
		break;
	case isPlay2onTurn://画白色棋子
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
			//若已分胜负，则结束游戏
			if (WinOrLose() == true)
			{
				Winner = 1;
				ReStart();
			}
			else if (Player1isAI == true) PaintChess();
		};
		break;
		//画黑色棋子
	case isAI1onTurn:
	{
		if (Player2isAI == true)
			onTurn = isAI2onTurn;
		else
			onTurn = isPlay2onTurn;

		///在此调用AI程序，返回下棋的位置（第几行，第几列）
		Player1AI->GetPosition(line, column, onTurn);

		PaintAChess(isBlack);
		//若对方是电脑，则先判断有没有分出胜负
		bool win = WinOrLose();
		if (Player2isAI == true && !win && line != 0 && column != 0)
			PaintChess();//若对方是电脑，则递归
		else if (win || line == 0 || column == 0)
		{
			Winner = 2;
			ReStart();
		}
	};
	break;
	//画白色棋子
	case isAI2onTurn:
	{
		if (Player1isAI == true)
			onTurn = isAI1onTurn;
		else
			onTurn = isPlay1onTurn;

		///在此调用AI程序，返回下棋的位置（第几行，第几列）
		Player2AI->GetPosition(line, column, onTurn);

		PaintAChess(isWhite);
		//若对方是电脑，则先判断有没有分出胜负
		bool win = WinOrLose();
		if (Player1isAI == true && !win && line != 0 && column != 0)
			PaintChess();//若对方是电脑，则递归
		else if (win || line == 0 || column == 0)
		{
			Winner = 1;
			ReStart();
		}
	};
	break;
	}
}
///实行画棋子
void ChessBoard::PaintAChess(int type)
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
	cross[line][column] = type;//将从AI程序获取到的行和列数记录到line和column
	TipPlayer(type);
	RePaint();
}
///获取下棋位置
bool ChessBoard::GetPointPosition(DWORD lParam)
{
	point.x = LOWORD(lParam);// 得到一个32bit数的低16bit
	point.y = HIWORD(lParam);// 得到一个32bit数的高16bit
	int a, b;
	b = (point.x - ChessDiameter / 2) / ChessDiameter + 1;
	a = (point.y - ChessDiameter / 2) / ChessDiameter + 1;
	// 判断是否越界
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
