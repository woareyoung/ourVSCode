#include "../stdafx.h"
#include "../AI2_Header/AI2.h"
#include <algorithm>
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
		}
		else if (PlayerId == Black) {
			turn2Who = tempArrayB[i];
		}
		Rival = getRival(turn2Who);
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
	匹配不成功，也没有分数最高的，重新选择一个寻找最佳点
	*************************************************/
	return FindPosition();
Find:
	return tempLine * 100 + tempColumn;
}

void AI2::getMaxScore(int& tempLine, int& tempColumn)
{
	/*
	if (MovePointer == 0) {
		return;
	}
	bool flag = true;
	int sum = getMaxScoreNum(goodMoves[0].Score);
	int ran;
	int i, j;
	while (flag)
	{
		ran = random(0, sum--);
		i = goodMoves[ran].line;
		j = goodMoves[ran].column;
		flag = isGo2Dead(i, j, turn2Who);
		goodMoves[ran] = { 0,0,0 };
		std::sort(goodMoves, goodMoves + MovePointer);
		--MovePointer;
	}
	tempLine = goodMoves[ran].line;
	tempColumn = goodMoves[ran].column;
	*/
	bool isFirst = true;
	for (int i = 1; i < 10; i++)
	{
		for (int j = 1; j < 10; j++)
		{
			// 这里需要修改
			if (chessScore[i][j] == minLimit || cross[i][j] != NoChess) continue;
			if (isGo2Dead(i, j, turn2Who)) continue;
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
			if (chessScore[i][j] == minLimit || cross[i][j] != NoChess) continue;
			if (isGo2Dead(i, j, turn2Who)) continue;
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
	if (this->chessCount < 2) {
		initChessScore(true);
	}
	else {
		initChessScore(false);
	}

	// 估值并加分
	// 这里进行模板匹配
	startPattern();
}

int AI2::FindPosition() {
	register int x = 0;
	register int y = 0;
	register int i; 
	register int j;
	bool Notloop = false;//是否断开循环
	//扫描棋盘，判断是否只有死棋
	for (i = 1; i < 10; i++)
	{
		for (j = 1; j < 10; j++)
		{	
			//如果不是空位，则扫描下一个位置
			if (cross[i][j] != NoChess) continue;
			//如果不是死棋，意味着棋局还没结束，则终止循环
			if (chessScore[i][j] != minLimit)
			{
				Notloop = true;
				break;
			}
			//如果扫描到一个死棋位置
			else
			{
				x = i;
				y = j;
			}
		}
		if (Notloop) break;
	}
	//如果棋盘上只剩下死棋，则返回一个空位
	if(i == 10 && j == 10)
	{
		return x * 100 + y;
	}
	for (i = 1; i < 10; i++) {
		for (j = 1; j < 10; j++) {
			x = i;
			y = j;
			if (cross[x][y] != NoChess || chessScore[x][y] == minLimit) continue;
			// 对于当前匹配到的着子点的环境进行分析
			// 临时设置当前获得的位置为我方着子点，判断是否是我方的自杀点
			cross[x][y] = turn2Who;
			if (isGo2Dead(x, y, turn2Who)) {
				chessScore[x][y] = minLimit;
				// 如果是我方的自杀点的话，就直接跳转，不用判断是否是敌方的自杀点了。
				goto empty;
			}
			if (isFinal()) goto empty;
			// 临时设置当前获得的位置为我方着子点，判断是否是敌方的自杀点
			if (chessScore[x][y] == 0) goto empty;
			cross[x][y] = Rival;
			if (isGo2Dead(x, y, Rival)) {
				chessScore[x][y] = 0;
				goto empty;
			}
		empty:
			// 这里什么都没有匹配到，所以进行重置
			cross[x][y] = NoChess;
		}
	}
	return x * 100 + y;
}
