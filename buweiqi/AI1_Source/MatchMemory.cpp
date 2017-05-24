#include "../AI1_Header/AI1.h"

int AI1::MatchMemory()
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

void AI1::GetChessBoardStatus()
{
	SITUATION NowStatus;
	int maxQuadrant = Qua.GetMaxQuadrant();//获取最多棋子的象限
	int rival = 3 - PlayerNumber;
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
				if (cross[i][j] == PlayerNumber) NowStatus.Line[i] += pow(2, 10 - j);
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
				if (cross[i][j] == PlayerNumber) NowStatus.Line[i] += pow(2, j);
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
				if (cross[i][j] == PlayerNumber) NowStatus.Line[10 - i] += pow(2, j);
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
				if (cross[i][j] == PlayerNumber) NowStatus.Line[10 - i] += pow(2, 10 - j);
				else if (cross[i][j] == rival) NowStatus.Line[10 - i] += pow(2, 10 - j) / 10000;
			}
		}
	}
	FS.Match(NowStatus, PlayerNumber);
}