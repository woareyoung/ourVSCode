#include "../AI2_Header/AI2.h"

DIRECTION direction_8[] = { { -1, 0 },{ 0, 1 },{ 1, 0 },{ 0, -1 },{ -1, 1 },{ 1, 1 },{ 1, -1 },{ -1, -1 } };
/**
* [AI2::isGo2Dead 判断着子点是否有自杀倾向]
*/
bool AI2::isGo2Dead(int line, int column, int type)
{
	int player = type;//记录己方的编号（是1还是2）
	int rival = getRival(player);//记录对方的编号（是1还是2）
	auto reduceRecursionTimes = [&]() -> void {
		for (int i = 0; i < 10; ++i)
		{
			if (isGo2DeadStatus[i][0] == true)
				for (int j = 0; j < 10; ++j)
					isGo2DeadStatus[i][j] = false;
		}
	};
	for (int i = 0; i < 4; ++i) Position[i] = false;
	for (int i = 0; i < 4; ++i)
	{
		if (cross[line + direction_8[i].x_offset][column + direction_8[i].y_offset] == rival)
		{
			reduceRecursionTimes();
			if (Besieg(line + direction_8[i].x_offset, column + direction_8[i].y_offset, turn2Who, rival))
				return true;
			Position[i] = true;
		}
		else if (line + direction_8[i].x_offset < 1 || line + direction_8[i].x_offset > 9
			|| column + direction_8[i].y_offset < 1 || column + direction_8[i].y_offset > 9)
		{
			Position[i] = true;
		}
		else if (cross[line + direction_8[i].x_offset][column + direction_8[i].y_offset] == turn2Who)
		{
			reduceRecursionTimes();
			Position[i] = Besieg(line, column, rival, turn2Who);
		}
	}
	return Position[0] && Position[1] && Position[2] && Position[3];
}

bool AI2::Besieg(int RivalLine, int RivalColumn, int player, int rival)
{
	bool tie[4] = { false, false, false, false };
	// 设置Cross数组状态
	auto setStatus = [&](int RivalLine, int RivalColumn) -> void {
		isGo2DeadStatus[RivalLine][RivalColumn] = true;
		isGo2DeadStatus[RivalLine][0] = true;//下标为0的那一行如果为true则初始化，否则不用初始化
	};
	setStatus(RivalLine, RivalColumn);

	//若对方棋子的上方有对方的棋子且未到上边缘
	if (cross[RivalLine - 1][RivalColumn] == rival && RivalLine - 1 > 0
		&& isGo2DeadStatus[RivalLine - 1][RivalColumn] == false)
		tie[0] = Besieg(RivalLine - 1, RivalColumn, player, rival);
	//若对方棋子的上方没有棋子，直接返回false
	else if (cross[RivalLine - 1][RivalColumn] == NoChess && RivalLine - 1 > 0) return false;
	//若对方棋子的上方有己方的棋子或已到边缘
	else tie[0] = true;
	//若对方棋子的下方有对方的棋子且未到下边缘
	if (cross[RivalLine + 1][RivalColumn] == rival && RivalLine + 1 < 10
		&& isGo2DeadStatus[RivalLine + 1][RivalColumn] == false)
		tie[1] = Besieg(RivalLine + 1, RivalColumn, player, rival);
	//若对方棋子的下方没有棋子
	else if (cross[RivalLine + 1][RivalColumn] == NoChess && RivalLine + 1 < 10) return false;
	//若对方棋子的下方有己方的棋子或已到边缘
	else tie[1] = true;
	//若对方棋子的右方有对方的棋子且未到右边缘
	if (cross[RivalLine][RivalColumn + 1] == rival && RivalColumn + 1 < 10
		&& isGo2DeadStatus[RivalLine][RivalColumn + 1] == false)
		tie[2] = Besieg(RivalLine, RivalColumn + 1, player, rival);
	//若对方棋子的右方没有棋子
	else if (cross[RivalLine][RivalColumn + 1] == NoChess && RivalColumn + 1 < 10) return false;
	//若对方棋子的右方有己方的棋子或已到边缘
	else tie[2] = true;
	//若对方棋子的左方有对方的棋子且未到左边缘
	if (cross[RivalLine][RivalColumn - 1] == rival && RivalColumn - 1 > 0
		&& isGo2DeadStatus[RivalLine][RivalColumn - 1] == false)
		tie[3] = Besieg(RivalLine, RivalColumn - 1, player, rival);
	//若对方棋子的左方没有棋子
	else if (cross[RivalLine][RivalColumn - 1] == NoChess && RivalColumn - 1 > 0) return false;
	//若对方棋子的左方有己方的棋子或已到边缘
	else tie[3] = true;
	if (tie[0] && tie[1] && tie[2] && tie[3]) return true;
	return false;
}


