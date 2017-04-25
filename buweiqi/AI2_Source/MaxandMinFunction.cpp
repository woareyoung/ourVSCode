#include "../stdafx.h"
#include "../AI2_Header/AI2.h"
/**
* [AI2::maxandmin 极大极小函数]
* @param depth 		执行的深度
*/
int AI2::maxandmin(int depth)
{
	int tempArray[10] =
	{
		2,1,2,1,2,1,2,1,2,1
	};
	int temp;
	for (int i = 0; i < depth; i++)
	{
		turn2Who = tempArray[i];
		Rival = (tempArray[i] == isWhite ? isBlack : isWhite);
		temp = singleLayer();
	}
	return temp;
}

/**
* [AI2::singleLayer 单层执行]
*/
int AI2::singleLayer()
{
	Revalute();
	if (turn2Who == PlayerId)
	{
		return MaxScore();
	}
	else
	{
		return MinScore();
	}
}

/**
* [AI2::MaxScore 获取极大值]
*/
int AI2::MaxScore()
{
	this->chessCount++;
	bool isFirst = true;
	int tempLine = 0;
	int tempColumn = 0;
	int changePoint = 0;
	int RivalKillCount = 0;
	int endCount = 81 - chessCount;
	int minNum = isFinal();
	bool no = minNum == chessCount;
	while (true)
	{
		_cprintf("**************chessStatusShaped******************\n");
		for (int i = 1; i < 10; i++)
		{
			for (int j = 1; j < 10; j++)
			{
				if (chessScore[i][j] < 10 && chessScore[i][j] > 0)
				{
					_cprintf("0\t");
				}
				else
				{
					_cprintf("%d\t", chessScore[i][j]);
				}
			}
			_cprintf("\n");
		}
		_cprintf("**************chessStatusShaped cross******************\n");
		for (int i = 1; i < 10; i++)
		{
			for (int j = 1; j < 10; j++)
				_cprintf("%d\t", cross[i][j]);
			_cprintf("\n");
		}

		getMaxScore(tempLine, tempColumn);// 获取最大值
		if (no && changePoint == tempLine * 100 + tempColumn && endCount > 0)
		{
			--endCount;
		}
		changePoint = tempLine * 100 + tempColumn;
		// 临时设置当前获得的位置为敌方着子点，判断是否是对方的自杀点
		cross[tempLine][tempColumn] = Rival;
		if (tempLine != 0 && 0 != tempColumn && isGo2Dead(tempLine, tempColumn, Rival))
		{
			if (no)
			{
				chessScore[tempLine][tempColumn] = 23;
				RivalKillCount++;
				_cprintf("----------(%d, %d)\n", tempLine, tempColumn);
				break;
			}
			else
				chessScore[tempLine][tempColumn] = -23;
			cross[tempLine][tempColumn] = noChess;
			continue;
		}
		// 临时设置当前获得的位置为我方着子点，判断是否是我方的自杀点
		cross[tempLine][tempColumn] = turn2Who;
		if (tempLine != 0 && 0 != tempColumn && isGo2Dead(tempLine, tempColumn, turn2Who))
		{
			chessScore[tempLine][tempColumn] = minLimit;
			cross[tempLine][tempColumn] = noChess;
			continue;
		}
		else
		{
			break;
		}
		if (RivalKillCount == 0)
		{
			tempLine = 0;
			tempColumn = 0;
			break;
		}
	}
	return tempLine * 100 + tempColumn;
}

/**
* [AI2::MinScore 获取极小值]
*/
int AI2::MinScore()
{
	bool isFirst = true;
	int tempLine;
	int tempColumn;
	// 输出分数
	for (int i = 1; i < 10; i++)
	{
		for (int j = 1; j < 10; j++)
		{
			if (chessScore[i][j] == minLimit || cross[i][j] != noChess)
			{
				continue;
			}
			if (isFirst)
			{
				tempLine = i;
				tempColumn = j;
				isFirst = false;
			}
			else if (!isFirst && chessScore[tempLine][tempColumn] < chessScore[i][j])
			{
				tempLine = i;
				tempColumn = j;
			}
		}
	}
	return tempLine * 100 + tempColumn;
}

void AI2::getMaxScore(int& tempLine, int& tempColumn)
{
	bool isFirst = true;
	for (int i = 1; i < 10; i++)
	{
		for (int j = 1; j < 10; j++)
		{
			if (chessScore[i][j] == minLimit || cross[i][j] != noChess)
				continue;
			if (isFirst)
			{
				tempLine = i;
				tempColumn = j;
				isFirst = false;
			}
			else if (!isFirst && chessScore[tempLine][tempColumn] < chessScore[i][j])
			{
				tempLine = i;
				tempColumn = j;
			}
		}
	}
}

/**
* [AI2::Revalute 估值函数]
*/
void AI2::Revalute()
{
	// 初始化棋盘的分数
	initChessScore();
	// 估值并加分
	chessStatusShaped();// 十字围杀
	AcrossCorners();// 边角围杀
	Tirangle();// 三角围杀
}
