#include "../ChessBoard_Header/AI.h"

int AI::MatchMemory()
{
	double NextPace;
	GetChessBoardStatus();
	FS.TempFile.seekg(0, std::ios::end);
	NextPace = FS.TempFile.tellg();
	if (NextPace == 0) return 0;
	FS.TempFile.seekg(0);
	for (int i = 0; i < 10; i++)
	{
		FS.TempFile >> NextPace;
	}
	return NextPace;
}
void AI::Statistic(int line, int column)
{
	if (line > 5 && column < 5) Qua.FirstQuadrant++;
	else if (line < 5 && column < 5) Qua.SecondQuadrant++;
	else if (line < 5 && column > 5) Qua.ThirdQuadrant++;
	else if (line > 5 && column > 5) Qua.ForthQuadrant++;
}
void AI::GetChessBoardStatus()
{
	SITUATION NowStatus;
	int maxQuadrant = Qua.GetMaxQuadrant();//获取最多棋子的象限
	int rival = 3 - player;
	int i, j;
	//如果最多棋子的是第一象限
	if (maxQuadrant == Qua.FirstQuadrant)
	{
		for (i = 1; i < 10; i++)
		{
			if (cross[i][0] != 1)
			{
				NowStatus.Line[i] = 0;
				continue;
			}
			for (j = 9; j > 0; j--)
			{
				if (cross[i][j] == player) NowStatus.Line[i] += pow(2, 10 - j);
				else if (cross[i][j] == rival) NowStatus.Line[i] += pow(2, 10 - j) / 10000;
			}
		}
	}
	//如果最多棋子的是第二象限
	else if (maxQuadrant == Qua.SecondQuadrant)
	{
		for (i = 1; i < 10; i++)
		{
			for (j = 1; j < 10; j++)
			{
				if (cross[i][j] == player) NowStatus.Line[i] += pow(2, j);
				else if (cross[i][j] == rival) NowStatus.Line[i] += pow(2, j) / 10000;
			}
		}
	}
	//如果最多棋子的是第三象限
	else if (maxQuadrant == Qua.ThirdQuadrant)
	{
		for (i = 9; i > 0; i--)
		{
			for (j = 1; j < 10; j++)
			{
				if (cross[i][j] == player) NowStatus.Line[10 - i] += pow(2, j);
				else if (cross[i][j] == rival) NowStatus.Line[10 - i] += pow(2, j) / 10000;
			}
		}
	}
	//如果最多棋子的是第四象限
	else if (maxQuadrant == Qua.ForthQuadrant)
	{
		for (i = 9; i > 0; i--)
		{
			for (j = 9; j > 0; j--)
			{
				if (cross[i][j] == player) NowStatus.Line[10 - i] += pow(2, 10 - j);
				else if (cross[i][j] == rival) NowStatus.Line[10 - i] += pow(2, 10 - j) / 10000;
			}
		}
	}
	FS.Match(NowStatus, player);
}