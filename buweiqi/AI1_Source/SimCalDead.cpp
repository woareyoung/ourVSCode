#include "../AI1_Header/AI1.h"

void AI1::CalDeadPosNumber(int line, int column)
{
	DoubleNoDeadNumber--;
	bool MUp = false, MDown = false, MLeft = false, MRight = false;
	if (cross[line + 1][column] == 0) MUp = DeadCheck(line - 1, column, PlayerId, true);
	if (cross[line - 1][column] == 0) MDown = DeadCheck(line + 1, column, PlayerId, true);
	if (cross[line][column + 1] == 0) MLeft = DeadCheck(line, column - 1, PlayerId, true);
	if (cross[line][column - 1] == 0) MRight = DeadCheck(line, column + 1, PlayerId, true);
	bool RUp = false, RDown = false, RLeft = false, RRight = false;
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
	else if (!MRight && RRight) RivalDeadPosNumber++;
}