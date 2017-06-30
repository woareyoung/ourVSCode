#include "../ChessBoard_Header/AI.h"

void AI::Statistic(int line, int column)
{
	if (line > 5 && column < 5) Qua.FirstQuadrant++;
	else if (line < 5 && column < 5) Qua.SecondQuadrant++;
	else if (line < 5 && column > 5) Qua.ThirdQuadrant++;
	else if (line > 5 && column > 5) Qua.ForthQuadrant++;
}
std::shared_ptr<NEXTPACE> AI::MatchMemory(int line, int column, bool &None)
{
	SITUATION NowStatus;
	Statistic(line, column);//先统计当前局面
	int maxQuadrant = Qua.GetMaxQuadrant();//获取最多棋子的象限
	int rival = 3 - PlayerId;
	int i, j;
	int TempSum;
	//如果最多棋子的是第一象限
	if (maxQuadrant == Qua.FirstQuadrant)
	{
		for (i = 1; i < 10; ++i)
		{
			TempSum = 1;
			for (j = 9; j > 0; j--)
			{
				if (cross[i][j] == PlayerId) NowStatus.Line[i] += pow(2, 10 - j) * 10000;
				else if (cross[i][j] == rival) NowStatus.Line[i] += pow(2, 10 - j);
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
				if (cross[i][j] == PlayerId) NowStatus.Line[i] += pow(2, j) * 10000;
				else if (cross[i][j] == rival) NowStatus.Line[i] += pow(2, j);
			}
		}
	}
	//如果最多棋子的是第三象限
	else if (maxQuadrant == Qua.ThirdQuadrant)
	{
		for (i = 9; i > 0; i--)
		{
			for (j = 1; j < 10; ++j)
			{
				if (cross[i][j] == PlayerId) NowStatus.Line[10 - i] += pow(2, j) * 10000;
				else if (cross[i][j] == rival) NowStatus.Line[10 - i] += pow(2, j);
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
				if (cross[i][j] == PlayerId) NowStatus.Line[10 - i] += pow(2, 10 - j) * 10000;
				else if (cross[i][j] == rival) NowStatus.Line[10 - i] += pow(2, 10 - j);
			}
		}
	}
	std::shared_ptr<NEXTPACE> np = FS.Match(NowStatus, PlayerId, CurrentRound);
	//如果有一模一样的记录，则直接跟着下
	if (np != NULL)
	{
		None = false;
		return np;
	}
	for (i = CurrentRound + 2; ; i = i + 2)
	{
		//没有一模一样的记录，则查询有没有含有当前盘面的“终盘”
		if(CurrentNull > i) np = FS.GenerMatch(NowStatus, PlayerId, i);
		else return NULL;
		if (np != nullptr)
		{
			None = true;
			return np;
		}
	}
	None = true;
	//如果什么都没有
	return NULL;
}

void AI::ClearList(std::shared_ptr<NEXTPACE> head)
{
	std::shared_ptr<NEXTPACE> temp = head->next;
	// delete head;
	head = nullptr;
	while (temp != NULL)
	{
		head = temp;
		temp = temp->next;
		// delete head;
		head = nullptr;
	}
}
