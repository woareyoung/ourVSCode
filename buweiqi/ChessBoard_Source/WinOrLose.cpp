#include "../stdafx.h"
#include "../ChessBoard_Header/ChessBoard.h"

/*胜负条件
* 1、超时。该条件判断由窗口过程函数实现
* 2、空手。先忽略
* 3、围死。基本思想：假设黑子下一个棋子，则判断该黑子周围的四个位置上有没有白子被围死，还有该黑子是否被围死
*/
//注：调用该函数（即下棋的玩家）的那个人或AI是“己方”
bool ChessBoard::WinOrLose()
{
	int i;
	for (i = 0; i < 4; i++) Position[i] = false;
	int player;//记录己方的编号（是1还是2）
	int rival;//记录对方的编号（是1还是2）
	//onTurn的值是先变化了再调用该函数（WinOrLose）的
	if (onTurn == isPlay1onTurn || onTurn == isAI1onTurn)
	{
		player = isWhite;
		rival = isBlack;
	}
	else if (onTurn == isPlay2onTurn || onTurn == isAI2onTurn)
	{
		player = isBlack;
		rival = isWhite;
	}

	//判断棋子四周有没有棋子
	//检查左边
	if (Check(line, column, line, column - 1, player, 0))
	{
		Winner = rival;
		return true;
	}
	//检查右边
	if (Check(line, column, line, column + 1, player, 1))
	{
		Winner = rival;
		return true;
	}
	//检查上边
	if (Check(line, column, line - 1, column, player, 2))
	{
		Winner = rival;
		return true;
	}
	//检查下边
	if (Check(line, column, line + 1, column, player, 3))
	{
		Winner = rival;
		return true;
	}
	if (Position[0] && Position[1] && Position[2] && Position[3])
	{
		Winner = rival;
		return true;
	}
	return false;
}
///参数：OriLine  下棋位置的行
///参数：OriColumn  下棋位置的列
///参数：CLine  需要检查的位置的行
///参数：CColumn   需要检查的位置的列
///参数：Player  哪个玩家下的棋子
///参数：i  下标（Position）
bool ChessBoard::Check(int OriLine, int OriColumn, int CLine, int CColumn, int player, int i)
{
	int rival = player == 1 ? 2 : 1;
	//若己方棋子已到边缘
	if (CColumn == 0 || CColumn == 10 || CLine == 0 || CLine == 10) Position[i] = true;
	//若有对方的一个棋子
	else if (cross[CLine][CColumn] == rival)
	{
		reduceRecursionTimes();
		if (Besieged(CLine, CColumn, player, rival))
		{
			Winner = rival;
			showChessBroadInfoOnDOS();
			return true;
		}
		Position[i] = true;
	}
	//若有己方的棋子
	else if (cross[CLine][CColumn] == player)
	{
		reduceRecursionTimes();
		Position[i] = Besieged(OriLine, OriColumn, rival, player);
	}
	return false;
}
//判断对方的棋子是否被围死，两个参数表示对方棋子的位置
//该函数用于一堆棋子被围死的情况，用递归的方法检查一堆棋子的情况
//若棋子被围死，则返回true，只要棋子周围有一个空位就返回false
bool ChessBoard::Besieged(int RivalLine, int RivalColumn, int player, int rival)
{
	bool tie[4] = { false, false, false, false };
	setStatus(RivalLine, RivalColumn);//设置位置访问状态
									  //检查对方棋子的上方
	if (!CheBesie(RivalLine - 1, RivalColumn, player, rival, tie[0])) return false;
	//检查对方棋子的下方
	if (!CheBesie(RivalLine + 1, RivalColumn, player, rival, tie[1])) return false;
	//检查对方棋子的右方
	if (!CheBesie(RivalLine, RivalColumn + 1, player, rival, tie[2])) return false;
	//检查对方棋子的左方
	if (!CheBesie(RivalLine, RivalColumn - 1, player, rival, tie[3])) return false;
	if (tie[0] && tie[1] && tie[2] && tie[3]) return true;
	return false;
}
bool ChessBoard::CheBesie(int RivalLine, int RivalColumn, int player, int rival, bool &tie)
{
	//若对方棋子的某方向有对方的棋子且未到边缘
	if (cross[RivalLine][RivalColumn] == rival && RivalLine > 0 && RivalColumn > 0
		&& Cross[RivalLine][RivalColumn] == false && RivalLine < 10 && RivalColumn < 10)
		tie = Besieged(RivalLine, RivalColumn, player, rival);
	//若对方棋子的某方向没有棋子，直接返回false
	else if (cross[RivalLine][RivalColumn] == 0 && RivalLine > 0 && RivalColumn > 0 && RivalLine < 10 && RivalColumn < 10)
		return false;
	//若对方棋子的某方向有己方的棋子或已到边缘
	else tie = true;
	return true;
}
