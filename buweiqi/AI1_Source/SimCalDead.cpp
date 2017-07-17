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
	QUADRANT qqq = Qua;
	SITUATION sit;
	Statistic(line, column, qqq);
	GetCurrentStatus(qqq.GetMaxQuadrant(), sit, qqq, crossing);
	sit.ChessPosition = line * 10 + column;
	std::set<int> res;
	int myLive = 0, rivalLive = 0;
	std::pair<int, int> p;
	//如果有预读内容，则用预读内容
	if (ForeReadHaveMem2)
	{
		auto ran = ForeReadContent2.equal_range(sit.BoardStatus);
		for (auto it = ran.first; it != ran.second; it++)
		{
			p = it->second;//获取“值”
			if (p.second == PlayerId) myLive++;
		}
	}
	else
	{
		FS.Match(sit, res, CurrentRound + 1, PlayerId);
		myLive = res.size();
	}
	res.clear();
	//如果有预读内容，则用预读内容
	if (ForeReadHaveMem2)
	{
		auto ran = ForeReadContent2.equal_range(sit.BoardStatus);
		for (auto it = ran.first; it != ran.second; it++)
		{
			p = it->second;//获取“值”
			if (p.second == 3 - PlayerId) rivalLive++;
		}
	}
	else
	{
		FS.Match(sit, res, CurrentRound + 1, 3 - PlayerId);
		rivalLive = res.size();
	}
	return RivalDeadPosNumber - MyDeadPosNumber + 0.45 * (TigerMouth[PlayerId] - TigerMouth[3 - PlayerId]) + 0.8 * myLive - 0.2 * rivalLive;
}