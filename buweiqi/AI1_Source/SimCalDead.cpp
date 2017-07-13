#include "../AI1_Header/AI1.h"

double AI1::CalDeadPosNumber(int line, int column)
{
	bool MyDeath, RivalDeath;

	double MyDeadPosNumber = 0;//自己的死棋位置数量
	double RivalDeadPosNumber = 0;//对方的死棋位置数量
	double TigerMouth = 0;//虎口数量
	auto CheckTigerMouth = [&](int TL, int TC) {
		int num = 0;
		if (cross[TL][TC + 1] != PlayerId && cross[TL][TC + 1] != 0) num++;
		if (cross[TL + 1][TC] != PlayerId && cross[TL + 1][TC] != 0) num++;
		if (cross[TL - 1][TC] != PlayerId && cross[TL - 1][TC] != 0) num++;
		if (cross[TL][TC - 1] != PlayerId && cross[TL][TC - 1] != 0) num++;
		if (num == 3) TigerMouth++;
	};
	for (int i = 1; i < 10; i++)
	{
		for (int j = 1; j < 10; j++)
		{
			if (cross[i][j] != 0) continue;
			MyDeath = DeadCheck(i, j, PlayerId);
			RivalDeath = DeadCheck(i, j, 3 - PlayerId);
			if (MyDeath && !RivalDeath) MyDeadPosNumber++;
			else if (!MyDeath && RivalDeath) RivalDeadPosNumber++;
			else CheckTigerMouth(i, j);
		}
	}
	cross[line][column] = 0;
	return RivalDeadPosNumber - MyDeadPosNumber + 0.5 * TigerMouth;
}