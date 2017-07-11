#include "../ChessBoard_Header/AI.h"
#define MAX_ROUND_K 4

void AI::Statistic(int line, int column)
{
	if (line < 5 && column > 5) Qua.FirstQuadrant++;
	else if (line < 5 && column < 5) Qua.SecondQuadrant++;
	else if (line > 5 && column < 5) Qua.ThirdQuadrant++;
	else if (line > 5 && column > 5) Qua.ForthQuadrant++;
}
std::shared_ptr<NEXTPACE> AI::MatchMemory(int line, int column, bool &None)
{
	Statistic(line, column);//先统计当前局面
	int maxQuadrant = Qua.GetMaxQuadrant();//获取最多棋子的象限
	int i;
	GetCurrentStatus(maxQuadrant);
	std::shared_ptr<NEXTPACE> np = FS.Match(NowStatus, line, CurrentRound);
	//如果有一模一样的记录，则直接跟着下
	if (np != nullptr)
	{
		None = false;
		return np;
	}
	for (i = CurrentRound + MAX_ROUND_K; ; i = i + 2)
	{
		//没有一模一样的记录，则查询有没有含有当前盘面的“终盘”
		if(i < 81) np = FS.GenerMatch(NowStatus, column, i);
		else return nullptr;
		if (np != nullptr)
		{
			None = true;
			return np;
		}
	}
	None = true;
	//如果什么都没有
	return nullptr;
}

void AI::ClearList(std::shared_ptr<NEXTPACE> head)
{
	std::shared_ptr<NEXTPACE> temp = head->next;
	head = nullptr;
	while (temp != nullptr)
	{
		head = temp;
		temp = temp->next;
		head = nullptr;
	}
}
void AI::GetCurrentStatus(int maxQuadrant)
{
	int rival = 3 - PlayerId;
	int TempSum, i, j;
	for (i = 0; i < 10; i++) NowStatus.Line[i] = 0;
	//如果最多棋子的是第一象限
	if (maxQuadrant == Qua.FirstQuadrant)
	{
		for (i = 1; i < 10; ++i)
		{
			TempSum = 1;
			for (j = 9; j > 0; j--)
			{
				TempSum *= 2;
				if (cross[i][j] == PlayerId) NowStatus.Line[i] += TempSum * 10000;
				else if (cross[i][j] == rival) NowStatus.Line[i] += TempSum;
			}
		}
	}
	//如果最多棋子的是第二象限
	else if (maxQuadrant == Qua.SecondQuadrant)
	{
		for (i = 1; i < 10; ++i)
		{
			TempSum = 1;
			for (j = 1; j < 10; ++j)
			{
				TempSum *= 2;
				if (cross[i][j] == PlayerId) NowStatus.Line[i] += TempSum * 10000;
				else if (cross[i][j] == rival) NowStatus.Line[i] += TempSum;
			}
		}
	}
	//如果最多棋子的是第三象限
	else if (maxQuadrant == Qua.ThirdQuadrant)
	{
		for (i = 9; i > 0; i--)
		{
			TempSum = 1;
			for (j = 1; j < 10; ++j)
			{
				TempSum *= 2;
				if (cross[i][j] == PlayerId) NowStatus.Line[10 - i] += TempSum * 10000;
				else if (cross[i][j] == rival) NowStatus.Line[10 - i] += TempSum;
			}
		}
	}
	//如果最多棋子的是第四象限
	else if (maxQuadrant == Qua.ForthQuadrant)
	{
		for (i = 9; i > 0; --i)
		{
			TempSum = 1;
			for (j = 9; j > 0; --j)
			{
				TempSum *= 2;
				if (cross[i][j] == PlayerId) NowStatus.Line[10 - i] += TempSum * 10000;
				else if (cross[i][j] == rival) NowStatus.Line[10 - i] += TempSum;
			}
		}
	}
}