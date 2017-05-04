#include "../stdafx.h"
#include "../AI2_Header/AI2.h"
/**
* [AI2::maxandmin 极大极小函数]
* @param depth 		执行的深度
*/
int AI2::maxandmin(int depth)
{
	int tempArrayW[10] =
	{
		2,1,2,1,2,1,2,1,2,1
	};
	int tempArrayB[10] =
	{
		1,2,1,2,1,2,1,2,1,2
	};
	int temp;
	for (int i = 0; i < depth; i++)
	{
		if (PlayerId == White) {
			turn2Who = tempArrayW[i];
			Rival = (tempArrayW[i] == isWhite ? isBlack : isWhite);
		}
		else if (PlayerId == Black) {
			turn2Who = tempArrayB[i];
			Rival = (tempArrayB[i] == isWhite ? isBlack : isWhite);
		}
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
	return DealWithScore(turn2Who == PlayerId);
}

int AI2::DealWithScore(bool isEqual) {
	int tempLine = 0;
	int tempColumn = 0;
	/************************************************
	匹配成功
	*************************************************/
	this->chessCount++;
	if (isEqual) {
		getMaxScore(tempLine, tempColumn);// 获取最大值
	}
	else {
		getMinScore(tempLine, tempColumn);// 获取最小值
	}
	if (tempLine != 0 && tempColumn != 0) {
		goto Find;
	}
	/************************************************
	匹配不成功，重新选择一个寻找最佳点
	*************************************************/
	return FindPosition();
Find:
	return tempLine * 100 + tempColumn;
}

void AI2::getMaxScore(int& tempLine, int& tempColumn)
{
	bool isFirst = true;
	for (int i = 1; i < 10; i++)
	{
		for (int j = 1; j < 10; j++)
		{
			// 这里需要修改
			if (chessScore[i][j] == minLimit || cross[i][j] != NoChess)
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

void AI2::getMinScore(int& tempLine, int& tempColumn)
{
	bool isFirst = true;
	for (int i = 1; i < 10; i++)
	{
		for (int j = 1; j < 10; j++)
		{
			// 这里需要修改
			if (chessScore[i][j] == minLimit || cross[i][j] != NoChess)
				continue;
			if (isFirst)
			{
				tempLine = i;
				tempColumn = j;
				isFirst = false;
			}
			else if (!isFirst && chessScore[tempLine][tempColumn] > chessScore[i][j])
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
//	chessStatusShaped();// 十字围杀
//	AcrossCorners();// 边角围杀
//	Tirangle();// 三角围杀

	startPattern();
	// 这里进行模板匹配
}

int AI2::FindPosition() {
	int x = 0;
	int y = 0;

	for (int i = 1; i < 10; i++) {
		for (int j = 1; j < 1; j++) {
			if (chessScore[i][j] == minLimit) 
				continue;
			// 对于当前匹配到的着子点的环境进行分析
			// 临时设置当前获得的位置为我方着子点，判断是否是我方的自杀点
			cross[x][y] = turn2Who;
			if (isGo2Dead(x, y, turn2Who)) {
				chessScore[x][y] = minLimit;
				cross[x][y] = NoChess;
				// 如果是我方的自杀点的话，就直接跳转，不用判断是否是敌方的自杀点了。
				goto Pos;
			}
			// 临时设置当前获得的位置为我方着子点，判断是否是敌方的自杀点
			cross[x][y] = Rival;
			if (isGo2Dead(x, y, Rival)) {
				cross[x][y] = NoChess;
				// 如果是敌方的自杀点的话，这里就不进行加分处理了   -.-！！！
				goto Pos;
			}
			// 这里什么都没有匹配到，所以进行重置
			cross[x][y] = NoChess;
		}
	}
Pos:
	return x * 100 + y;
}
