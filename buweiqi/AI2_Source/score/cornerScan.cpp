#include "../../stdafx.h"
#include "../../AI2_Header/AI2.h"

/**
* [AI2::AcrossCorners 边角扫描]
* 实现逻辑，通过扫描边角的实现分数加法
* @return [无]
*/
void AI2::AcrossCorners()
{
	// 轮到本方的时候
	for (int i = 0; i < 12; i += 3) {
		ACScan(cornerArray[i] / 100, cornerArray[i] % 100,
			cornerArray[i + 1] / 100, cornerArray[i + 1] % 100,
			cornerArray[i + 2] / 100, cornerArray[i + 2] % 100);
	}
}

/**
* [AI2::ACScan 扫描函数，用于判断边角（最边边）是否有棋子]
* @param line    [边角棋子的行]
* @param column  [边角棋子的列]
* @param line1   [边角棋子周围的棋子的行]
* @param column1 [边角棋子周围的棋子的列]
* @param line2   [边角棋子周围的棋子的行]
* @param column2 [边角棋子周围的棋子的列]
*/
void AI2::ACScan(int line, int column, int line1, int column1, int line2, int column2)
{
	if (cross[line][column] == noChess) {
		// 边角已经形成包围圈的情况
		if (cross[line1][column1] == Rival && cross[line2][column2] == Rival) {
			chessScore[line][column] = minLimit;
		}
		// 边角两个位置单子的情况
		else if (cross[line1][column1] == noChess && cross[line2][column2] == isWhite) {
			chessScore[line1][column1] += cornerScoreFull;
		}
		else if (cross[line1][column1] == isWhite && cross[line2][column2] == noChess) {
			chessScore[line2][column2] += cornerScoreFull;
		}
		else if (cross[line1][column1] == isBlack && cross[line2][column2] == noChess) {
			chessScore[line2][column2] += cornerScoreFull;
		}
		else if (cross[line1][column1] == noChess && cross[line2][column2] == isBlack) {
			chessScore[line1][column1] += cornerScoreFull;
		}
		// 边角构成的三角行上面一个棋子都没有，这种情况的话，需要加分
		else if (cross[line1][column1] == noChess && cross[line2][column2] == noChess) {
			chessScore[line1][column1] += cornerScore;
			chessScore[line2][column2] += cornerScore;
		}

	}// 注意：边角有子无意义
}

