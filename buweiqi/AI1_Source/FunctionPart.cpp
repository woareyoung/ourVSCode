#include "../stdafx.h"
#include "../AI1_Header/AI1.h"
///获取特定位置的四个方向的棋子数量（包括边界）
///参数who：所需查看的棋子所属的玩家编号
int AI1::GetChessAmount(int row1, int row2, int who, int &num)
{
	int amount = 0;
	if (cross[row1][row2 + 1] == who || row2 + 1 == 10)
	{
		num += 1;//右
		amount++;
	}
	if (cross[row1][row2 - 1] == who || row2 - 1 == 0)
	{
		num += 2;//左
		amount++;
	}
	if (cross[row1 + 1][row2] == who || row1 + 1 == 10)
	{
		num += 4;//下
		amount++;
	}
	if (cross[row1 - 1][row2] == who || row1 - 1 == 0)
	{
		num += 8;//上
		amount++;
	}
	return amount;
}
///获取最大分值的位置
void AI1::GetMaxScorePosition()
{
	srand(time(NULL));
	int ran = 0, i, j;
	while (ran == 0 || ran % 10 == 0) ran = rand() % 89 + 11;
	MaxScore = Score[ran / 10][ran % 10];
	MaxScorePosition = ran;
	for (i = 1; i < 10; ++i)
	{
		for (j = 1; j < 10; ++j)
		{
			if (MaxScore < Score[i][j])
			{
				MaxScorePosition = i * 10 + j;
				MaxScore = Score[i][j];
			}
		}
	}
}
///记录棋盘数据
void AI1::MemoryData(int line, int column)
{
	if (line < 5 && column > 5) Qua.FirstQuadrant++;
	else if (line < 5 && column < 5) Qua.SecondQuadrant++;
	else if (line > 5 && column < 5) Qua.ThirdQuadrant++;
	else if (line > 5 && column > 5) Qua.ForthQuadrant++;
}