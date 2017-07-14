#include "../AI1_Header/AI1.h"

double AI1::CalDeadPosNumber(int line, int column)
{
	if (cross[line][column] != 0) return -1000;
	bool MyDeath, RivalDeath;
	int i, j;
	int crossing[10][10];
	for (i = 0; i < 10; i++)
		for (j = 0; j < 10; j++)
			crossing[i][j] = cross[i][j];
	crossing[line][column] = PlayerId;
	if(DeadCheck(line, column, PlayerId, crossing)) return -101;
	double MyDeadPosNumber = 0;//自己的死棋位置数量
	double RivalDeadPosNumber = 0;//对方的死棋位置数量
	double TigerMouth = 0;//虎口数量
	auto CheckTigerMouth = [&](int TL, int TC) {
		int num = 0;
		if (crossing[TL][TC + 1] != PlayerId && crossing[TL][TC + 1] != 0) num++;
		if (crossing[TL + 1][TC] != PlayerId && crossing[TL + 1][TC] != 0) num++;
		if (crossing[TL - 1][TC] != PlayerId && crossing[TL - 1][TC] != 0) num++;
		if (crossing[TL][TC - 1] != PlayerId && crossing[TL][TC - 1] != 0) num++;
		if (num == 3) TigerMouth++;
	};
	for (i = 1; i < 10; i++)
	{
		for (j = 1; j < 10; j++)
		{
			if (crossing[i][j] != 0) continue;
			MyDeath = DeadCheck(i, j, PlayerId, crossing);
			RivalDeath = DeadCheck(i, j, 3 - PlayerId, crossing);
			if (MyDeath && !RivalDeath) MyDeadPosNumber++;
			else if (!MyDeath && RivalDeath) RivalDeadPosNumber++;
			else CheckTigerMouth(i, j);
		}
	}
	return RivalDeadPosNumber - MyDeadPosNumber + 0.5 * TigerMouth;
}