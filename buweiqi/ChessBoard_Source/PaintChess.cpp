#include "../ChessBoard_Header/parameter.h"
#include "../ChessBoard_Header/ChessBoard.h"
#include "../ChessBoard_Header/WinCheck.h"

///画棋子函数
void ChessBoard::PaintChess()
{
	WinCheck::ChessBoardOption option;
	option.isInterface = true;
	WinCheck::ChessInfo chessInfo(option);
	// 判断轮到哪一方
	switch (onTurn)
	{
	case isPlay1onTurn://画黑色棋子
		if (line == 0 || column == 0) return;
		if (cross[line][column] != 0) return;
		if (Player2isAI == false)
			onTurn = isPlay2onTurn;
		else
			onTurn = isAI2onTurn;
		PaintAChess(1);
		//若已分胜负，则结束游戏
		if (chessInfo.WinOrLoseCheck(line, column, onTurn, Winner, cross))
		{
			// Winner = 2;
			ReStart();
		}
		break;
	case isPlay2onTurn://画白色棋子
		if (line == 0 || column == 0) return;
		if (cross[line][column] != 0) return;
		if (Player1isAI == false)
			onTurn = isPlay1onTurn;
		else
			onTurn = isAI1onTurn;
		PaintAChess(2);
		//若已分胜负，则结束游戏
		if (chessInfo.WinOrLoseCheck(line, column, onTurn, Winner, cross))
		{
			// Winner = 1;
			ReStart();
		}
		break;
		//画黑色棋子
	case isAI1onTurn:
	{
		///在此调用AI程序，返回下棋的位置（第几行，第几列）
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
		//若对方是电脑，则先判断有没有分出胜负
		if (chessInfo.WinOrLoseCheck(line, column, onTurn, Winner, cross))
		{
			// Winner = 2;
			ReStart();
		}
	};
	break;
	//画白色棋子
	case isAI2onTurn:
	{
		///在此调用AI程序，返回下棋的位置（第几行，第几列）
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
		//若对方是电脑，则先判断有没有分出胜负
		if (chessInfo.WinOrLoseCheck(line, column, onTurn, Winner, cross))
		{
			// Winner = 1;
			ReStart();
		}
	};
	break;
	}
}
///实行画棋子
void ChessBoard::PaintAChess(int type)
{
//	ChessAmount[type]++;
	AddList(type);
	cross[line][column] = type;//将获取到的行和列数记录到line和column
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
///获取下棋位置
void ChessBoard::GetPointPosition(LPARAM lParam)
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
	}
}
