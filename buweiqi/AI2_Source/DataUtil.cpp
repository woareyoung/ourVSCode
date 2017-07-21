#include "../AI2_Header/AI2.h"
#include <cstdlib>
void AI2::initAllArray() {
	initAll();
	for (int i = ChessStart; i < ChessEnd; ++i) {
		for (int j = ChessStart; j < ChessEnd; ++j) {
			cross[i][j] = NoChess;
		}
	}
}

void AI2::initChessScore(const bool& isFirst)
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



