#include "../stdafx.h"
#include "../AI1_Header/AI1.h"

///参数：预备下棋位置
bool AI1::DeadCheck(int line, int column, int who)
{
	bool Position[4] = { false, false, false, false };
	int player = who;//记录己方的编号（是1还是2）
	//判断棋子四周有没有棋子
	//检查左边
	if (CheckPosition(line, column, line, column - 1, player, Position[0])) return true;
	//检查右边
	if (CheckPosition(line, column, line, column + 1, player, Position[1]))  return true;
	//检查上边
	if (CheckPosition(line, column, line - 1, column, player, Position[2])) return true;
	//检查下边
	if (CheckPosition(line, column, line + 1, column, player, Position[3])) return true;
	if (Position[0] && Position[1] && Position[2] && Position[3]) return true;
	return false;
}
bool AI1::CheckPosition(int OriLine, int OriColumn, int CLine, int CColumn, int player, bool &Position)
{
	int rival = player == 1 ? 2 : 1;
	auto reduceRecursionTimes = [&]() -> void {
		for (int i = 0; i < 10; ++i)
		{
			if (Cross[i][0] == true)
				for (int j = 0; j < 10; ++j)
					Cross[i][j] = false;
		}
	};
	//若己方棋子已到边缘
	if (CColumn == 0 || CColumn == 10 || CLine == 0 || CLine == 10) Position = true;
	//若有对方的一个棋子
	else if (cross[CLine][CColumn] == rival)
	{
		reduceRecursionTimes();
		if (Besieg(CLine, CColumn, player, rival))
			return true;
		Position = true;
	}
	//若有己方的棋子
	else if (cross[CLine][CColumn] == player)
	{
		reduceRecursionTimes();
		Position = Besieg(OriLine, OriColumn, rival, player);
	}
	return false;
}
bool AI1::Besieg(int RivalLine, int RivalColumn, int player, int rival)
{
	bool tie[4] = { false, false, false, false };
	Cross[RivalLine][RivalColumn] = true;
	Cross[RivalLine][0] = true;//下标为0的那一行如果为true则初始化，否则不用初始化
	//检查对方棋子的上方
	if (!BesiegePosition(RivalLine - 1, RivalColumn, player, rival, tie[0])) return false;
	//检查对方棋子的下方
	if (!BesiegePosition(RivalLine + 1, RivalColumn, player, rival, tie[1])) return false;
	//检查对方棋子的右方
	if (!BesiegePosition(RivalLine, RivalColumn + 1, player, rival, tie[2])) return false;
	//检查对方棋子的左方
	if (!BesiegePosition(RivalLine, RivalColumn - 1, player, rival, tie[3])) return false;
	if (tie[0] && tie[1] && tie[2] && tie[3]) return true;
	return false;
}
bool AI1::BesiegePosition(int RivalLine, int RivalColumn, int player, int rival, bool &tie)
{
	//若对方棋子的某方向有对方的棋子且未到边缘
	if (cross[RivalLine][RivalColumn] == rival && RivalLine > 0 && RivalColumn > 0
		&& Cross[RivalLine][RivalColumn] == false && RivalLine < 10 && RivalColumn < 10)
		tie = Besieg(RivalLine, RivalColumn, player, rival);
	//若对方棋子的某方向没有棋子，直接返回false
	else if (cross[RivalLine][RivalColumn] == 0 && RivalLine > 0 && RivalColumn > 0 && RivalLine < 10 && RivalColumn < 10)
		return false;
	//若对方棋子的某方向有己方的棋子或已到边缘
	else tie = true;
	return true;
}
