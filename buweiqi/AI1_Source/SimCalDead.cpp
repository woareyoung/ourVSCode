#include "../AI1_Header/AI1.h"

double AI1::CalDeadPosNumber(int line, int column)
{
	cross[line][column] = PlayerId;
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
			MyDeath = DeadCheck(i, j, PlayerId, true);
			RivalDeath = DeadCheck(i, j, 3 - PlayerId, true);
			if (MyDeath && !RivalDeath) MyDeadPosNumber++;
			else if (!MyDeath && RivalDeath) RivalDeadPosNumber++;
			else CheckTigerMouth(i, j);
		}
	}
	cross[line][column] = 0;
	return RivalDeadPosNumber - MyDeadPosNumber + 0.5 * TigerMouth;
	/*
	bool MUp = false, MDown = false, MLeft = false, MRight = false;
	bool RUp = false, RDown = false, RLeft = false, RRight = false;
	if (cross[line + 1][column] == 0) MUp = DeadCheck(line - 1, column, PlayerId, true);
	if (cross[line - 1][column] == 0) MDown = DeadCheck(line + 1, column, PlayerId, true);
	if (cross[line][column + 1] == 0) MLeft = DeadCheck(line, column - 1, PlayerId, true);
	if (cross[line][column - 1] == 0) MRight = DeadCheck(line, column + 1, PlayerId, true);
	
	int Rival = 3 - PlayerId;
	if (cross[line + 1][column] == 0) RUp = DeadCheck(line - 1, column, Rival, true);
	if (cross[line - 1][column] == 0) RDown = DeadCheck(line + 1, column, Rival, true);
	if (cross[line][column + 1] == 0) RLeft = DeadCheck(line, column - 1, Rival, true);
	if (cross[line][column - 1] == 0) RRight = DeadCheck(line, column + 1, Rival, true);
	if (MUp || RUp) DoubleNoDeadNumber--;
	if (MDown || RDown) DoubleNoDeadNumber--;
	if(MLeft || RLeft) DoubleNoDeadNumber--;
	if(MRight || RRight) DoubleNoDeadNumber--;
	if (MUp && !RUp) MyDeadPosNumber++;
	else if (!MUp && RUp) RivalDeadPosNumber++;
	if (MDown && !RDown) MyDeadPosNumber++;
	else if (!MDown && RDown) RivalDeadPosNumber++;
	if (MLeft && !RLeft) MyDeadPosNumber++;
	else if (!MLeft && RLeft) RivalDeadPosNumber++;
	if (MRight && !RRight) MyDeadPosNumber++;
	else if (!MRight && RRight) RivalDeadPosNumber++;*/
}