#include "../stdafx.h"
#include "../AI1_Header/AIForFile.h"
#define MAX_ROUND_K 67

bool AIForFile::MatchMemory(int line, int column, std::set<int> &res)
{
	QUADRANT qqq = Qua;
	if(cross[line][column] == 0) Statistic(line, column, qqq);//先统计当前局面
	int i;
	GetCurrentStatus(qqq.GetMaxQuadrant(), NowStatus, qqq, cross);
	std::pair<int, int> p;
	if (ForeReadHaveMem)
	{
		auto ran = ForeReadContent.equal_range(NowStatus.BoardStatus);
		for (auto it = ran.first; it != ran.second; it++)
		{
			p = it->second;//获取“值”
			res.insert(p.first);//获取“值”的第一个元素
		}
	}
	else FS.Match(NowStatus, res, CurrentRound, PlayerId);
	//如果有一模一样的记录，则直接跟着下
	if (!res.empty()) return false;
	for (i = CurrentRound > MAX_ROUND_K ? CurrentRound : MAX_ROUND_K; ; i = i + 1)
	{
		//没有一模一样的记录，则查询有没有含有当前盘面的“终盘”
		if(i < 81) FS.GenerMatch(NowStatus, res, i, PlayerId);
		else return false;
		if (!res.empty()) return true;
	}
	return true;
}
void AIForFile::GetCurrentStatus(int maxQuadrant, SITUATION &CurrentStatus, QUADRANT &qua, int CROSS[][10])
{
	int i, j;
	for (i = 0; i < 10; i++) CurrentStatus.BoardStatus = "";
	//如果最多棋子的是第一象限
	if (maxQuadrant == qua.FirstQuadrant)
	{
		for (i = 1; i < 10; ++i)
			for (j = 9; j > 0; j--)
				CurrentStatus.BoardStatus += std::to_string(CROSS[i][j]);
	}
	//如果最多棋子的是第二象限
	else if (maxQuadrant == qua.SecondQuadrant)
	{
		for (i = 1; i < 10; ++i)
			for (j = 1; j < 10; ++j)
				CurrentStatus.BoardStatus += std::to_string(CROSS[i][j]);
	}
	//如果最多棋子的是第三象限
	else if (maxQuadrant == qua.ThirdQuadrant)
	{
		for (i = 9; i > 0; i--)
			for (j = 1; j < 10; ++j)
				CurrentStatus.BoardStatus += std::to_string(CROSS[i][j]);
	}
	//如果最多棋子的是第四象限
	else if (maxQuadrant == qua.ForthQuadrant)
	{
		for (i = 9; i > 0; i--)
			for (j = 9; j > 0; j--)
				CurrentStatus.BoardStatus += std::to_string(CROSS[i][j]);
	}
}