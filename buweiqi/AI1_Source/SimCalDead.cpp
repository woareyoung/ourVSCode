#include "../stdafx.h"
#include "../AI1_Header/AI1.h"

double AI1::CalDeadPosNumber(int line, int column, int &deadPos, bool Cal)
{
	if (cross[line][column] != 0 && Cal == true) return -1000;
	deadPos = 81;
	bool MyDeath, RivalDeath;
	int i, j;
	int crossing[10][10];
	for (i = 0; i < 10; i++)
		for (j = 0; j < 10; j++)
			crossing[i][j] = cross[i][j];
	crossing[line][column] = PlayerId;
	double MyDeadPosNumber = 0;//自己的死棋位置数量
	double RivalDeadPosNumber = 0;//对方的死棋位置数量
	double TigerMouth[3] = { 0 };//虎口数量
	auto CheckTigerMouth = [&](int TL, int TC, int player) {
		int num = 0;
		if (crossing[TL][TC + 1] == player || TC == 9) num++;
		if (crossing[TL + 1][TC] == player && TL == 9) num++;
		if (crossing[TL - 1][TC] == player && TL == 1) num++;
		if (crossing[TL][TC - 1] == player && TC == 1) num++;
		if (num == 3) ++TigerMouth[player];
	};
	for (i = 1; i < 10; ++i)
	{
		for (j = 1; j < 10; ++j)
		{
			if (crossing[i][j] != 0)
			{
				--deadPos;
				continue;
			}
			MyDeath = DeadCheck(i, j, PlayerId, crossing);
			RivalDeath = DeadCheck(i, j, 3 - PlayerId, crossing);
			if (MyDeath && !RivalDeath) MyDeadPosNumber++, deadPos--;
			else if (!MyDeath && RivalDeath) RivalDeadPosNumber++, deadPos--;
			else
			{
				CheckTigerMouth(i, j, PlayerId);
				CheckTigerMouth(i, j, 3 - PlayerId);
			}
		}
	}
	return RivalDeadPosNumber - MyDeadPosNumber + 0.5 * (TigerMouth[PlayerId] - TigerMouth[3 - PlayerId]);
}