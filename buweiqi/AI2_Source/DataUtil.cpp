#include "../AI2_Header/AI2.h"

void AI2::initAllArray() {
	this->chessCount = 0;
	for (int i = 1; i < 10; i++) {
		for (int j = 1; j < 10; j++) {
			cross[i][j] = NoChess;
		}
	}
}

void AI2::initChessScore(bool isFirst)
{
	int temp[10][10] =
	{
		{ 0,0,0,0,0,0,0,0,0,0 },
		{ 0,1,1,1,1,1,1,1,1,1 },
		{ 0,1,2,2,2,2,2,2,2,1 },
		{ 0,1,2,3,3,3,3,3,2,1 },
		{ 0,1,2,3,4,4,4,3,2,1 },
		{ 0,1,2,3,4,5,4,3,2,1 },
		{ 0,1,2,3,4,4,4,3,2,1 },
		{ 0,1,2,3,3,3,3,3,2,1 },
		{ 0,1,2,2,2,2,2,2,2,1 },
		{ 0,1,1,1,1,1,1,1,1,1 }
	};
	for (int i = 1; i < 10; i++)
	{
		for (int j = 1; j < 10; j++)
		{
			if (cross[i][j] == NoChess && chessScore[i][j] == minLimit) continue;
			if (!isFirst && cross[i][j] == NoChess && chessScore[i][j] == 0) continue;
			chessScore[i][j] = temp[i][j];
		}
	}
}

bool AI2::isFinal() {
	int emptyCount = 0;
	int minLimitCount = 0;
	int NoughtCount = 0;
	for (int i = 1; i < 10; i++)
		for (int j = 1; j < 10; j++) {
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
						
	return NoughtCount+minLimitCount == emptyCount;
}

void AI2::resetGo2DeadStatus() {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			isGo2DeadStatus[i][j] = false;
		}
	}
}

void AI2::ScanChessBroad() {
	for (int x = 0; x < 10; x++) {
		for (int y = 0; y < 10; y++) {
			if (cross[x][y] == NoChess) {
				cross[x][y] = turn2Who;
				if (isGo2Dead(x, y, turn2Who)) {
					chessScore[x][y] = minLimit;
					cross[x][y] = NoChess;
					// 如果是我方的自杀点的话，就直接跳转，不用判断是否是敌方的自杀点了。
					goto mismatch;
				}
				// 临时设置当前获得的位置为敌方着子点，判断是否是敌方的自杀点
				if (cross[x][y] == NoChess && chessScore[x][y] == 0) goto mismatch;
				cross[x][y] = Rival;
				if (isGo2Dead(x, y, Rival)) {
					cross[x][y] = NoChess;
					// 如果是敌方的自杀点的话，这里就置零   -.-！！！
					chessScore[x][y] = 0;
					goto mismatch;
				}
				// 这里既不是我方自杀点，也不是敌方自杀点
				cross[x][y] = NoChess;
			}
			else {
				if (isGo2Dead(x, y, cross[x][y])) {
					chessScore[x][y] = minLimit;
					// 如果是我方的自杀点的话，就直接跳转，不用判断是否是敌方的自杀点了。
					goto mismatch;
				}
			}	
		mismatch:;
		}
	}
}