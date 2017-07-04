#include "../AI2_Header/AI2.h"
#include <cstdlib>
void AI2::initAllArray() {
	initAll();
	for (int i = ChessStart; i < ChessEnd; ++i) {
		this->goodMoves[i - 1] = { 0,0,0 };
		for (int j = ChessStart; j < ChessEnd; ++j) {
			cross[i][j] = NoChess;
		}
	}
}

void AI2::initChessScore(bool isFirst)
{
	for (int i = ChessStart; i < ChessEnd; ++i)
	{
		for (int j = ChessStart; j < ChessEnd; ++j)
		{
			if (cross[i][j] == NoChess && chessScore[i][j] == minLimit) continue;
			if (!isFirst && cross[i][j] == NoChess && chessScore[i][j] == 0) continue;
			chessScore[i][j] = getDefaultChessScore(i, j);
		}
	}
}

bool AI2::isFinal() {
	int emptyCount = 0;
	int minLimitCount = 0;
	int NoughtCount = 0;
	for (int i = ChessStart; i < ChessEnd; ++i)
		for (int j = ChessStart; j < ChessEnd; ++j) {
			if (cross[i][j] == NoChess) {
				emptyCount++;
			}
			else {
				continue;
			}
			if (chessScore[i][j] == minLimit) {
				minLimitCount++;
			}
			else if (chessScore[i][j] == 0)
			{
				NoughtCount++;
			}
		}

	return NoughtCount + minLimitCount == emptyCount;
}

void AI2::ScanChessBroad() {
	for (int x = ChessStart; x < ChessEnd; ++x) {
		for (int y = ChessStart; y < ChessEnd; ++y) {
			if (cross[x][y] == NoChess) {
				cross[x][y] = turn2Who;
				if (isGo2Dead(x, y, turn2Who)) {
					chessScore[x][y] = minLimit;
					cross[x][y] = NoChess;
					// 如果是我方的自杀点的话，就直接跳转，不用判断是否是敌方的自杀点了。
					continue;
				}
				// 临时设置当前获得的位置为敌方着子点，判断是否是敌方的自杀点
				if (cross[x][y] == NoChess && chessScore[x][y] == 0) continue;
				cross[x][y] = Rival;
				if (isGo2Dead(x, y, Rival)) {
					cross[x][y] = NoChess;
					// 如果是敌方的自杀点的话，这里就置零   -.-！！！
					chessScore[x][y] = 0;
					continue;
				}
				// 这里既不是我方自杀点，也不是敌方自杀点
				cross[x][y] = NoChess;
			}
			else {
				if (isGo2Dead(x, y, cross[x][y])) {
					chessScore[x][y] = minLimit;
					// 如果是我方的自杀点的话，就直接跳转，不用判断是否是敌方的自杀点了。
					continue;
				}
			}
		}
	}
}

int AI2::getMaxScoreNum(int judge) {
	register int i = 0;
	if (MovePointer == 0) {
		return 0;
	}
	int temp = judge;
	for (; i < MovePointer; ++i) {
		if (temp != goodMoves[i].Score) {
			break;
		}
	}
	return i;
}

