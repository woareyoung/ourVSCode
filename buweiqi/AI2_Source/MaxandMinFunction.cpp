#include "../AI2_Header/AI2.h"
/**
* [AI2::maxandmin 极大极小函数]
* @param depth 		执行的深度
*/
int AI2::maxandmin(const int& depth)
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
	for (int i = 0; i < depth; ++i)
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
	/*if (!goodMoves.empty()) {
		std::vector<goodMove>().swap(goodMoves);
	}*/
	Revalute();
	return DealWithScore(turn2Who == PlayerId);
}

int AI2::DealWithScore(const bool& isEqual) {
	int tempLine = 0;
	int tempColumn = 0;
	/************************************************
	匹配成功
	*************************************************/
	this->chessCount++;
	this->getScore(tempLine, tempColumn, isEqual);// 获取最大值
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

#include "../ChessBoard_Header/WinCheck.h"

void AI2::getScore(int& tempLine, int& tempColumn, bool isGetMax)
{
//	if (goodMoves.size() >= 1) {
//		this->arraySort();
//	}
//	else {
//		goto other;
//	}
//	goodMove gm = isGetMax ? goodMoves.front() : goodMoves.back();
//	int size = goodMoves.size();
//	int maxsize = 0;
//
//	int i;
//	for (i = 0; i < size; ++i) {
//		if (goodMoves[i].Score == gm.Score) {
//			++maxsize;
//		}
//	}
//	i = isGetMax ? 0 : size - maxsize;
//	int range = isGetMax ? maxsize : size;
//	for (; i < range; ++i)
//	{
//		tempLine = goodMoves[i].line;
//		tempColumn = goodMoves[i].column;
//		return;
//	}
//other:
	WinCheck::ChessBoardOption option;
	option.black = Black;
	option.white = White;
	option.edge = Edge;
	option.emptyChess = NoChess;
	WinCheck::ChessInfo chessInfo(option);
	bool isFirst = true;
	for (int i = 1; i < 10; ++i)
	{
		for (int j = 1; j < 10; ++j)
		{
			// 这里需要修改
			if (chessScore[i][j] == minLimit || cross[i][j] != NoChess) continue;
			if (chessScore[i][j] != 0 && chessInfo.WinOrLoseCheck(i, j, turn2Who,cross)) continue;
			if (isFirst)
			{
				tempLine = i;
				tempColumn = j;
				isFirst = false;
			}
			else if (!isFirst)
			{
				if (isGetMax) {
					if (chessScore[tempLine][tempColumn] < chessScore[i][j]) {
						tempLine = i;
						tempColumn = j;
					}
				}
				else {
					if (chessScore[tempLine][tempColumn] > chessScore[i][j]) {
						tempLine = i;
						tempColumn = j;
					}
				}
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
	for (i = 1; i < 10; ++i)
	{
		for (j = 1; j < 10; ++j)
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
	if (i == 10 && j == 10)
	{
		return x * 100 + y;
	}
	WinCheck::ChessBoardOption option;
	option.black = Black;
	option.white = White;
	option.edge = Edge;
	option.emptyChess = NoChess;
	WinCheck::ChessInfo chessInfo(option);
	for (i = 1; i < 10; ++i) {
		for (j = 1; j < 10; ++j) {
			x = i;
			y = j;
			if (cross[x][y] != NoChess || chessScore[x][y] == minLimit) continue;
			// 对于当前匹配到的着子点的环境进行分析
			// 临时设置当前获得的位置为我方着子点，判断是否是我方的自杀点
			if (chessInfo.WinOrLoseCheck(x, y, turn2Who, cross)) {
				chessScore[x][y] = minLimit;
				// 如果是我方的自杀点的话，就直接跳转，不用判断是否是敌方的自杀点了。
				continue;
			}
			if (isFinal()) continue;
			// 临时设置当前获得的位置为我方着子点，判断是否是敌方的自杀点
			if (chessScore[x][y] == 0) continue;
			if (chessInfo.WinOrLoseCheck(x, y, turn2Who, cross)) {
				chessScore[x][y] = 0;
				continue;
			}
			// 这里什么都没有匹配到，所以进行重置
		}
	}
	return x * 100 + y;
}
