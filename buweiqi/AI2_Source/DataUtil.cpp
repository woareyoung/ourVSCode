#include "../AI2_Header/AI2.h"
#include <cstdlib>
void AI2::initAllArray() {
	this->chessCount = 0;
	this->MovePointer = 0;
	for (int i = 1; i < 10; i++) {
		this->goodMoves[i - 1] = { 0,0,0 };
		for (int j = 1; j < 10; j++) {
			cross[i][j] = NoChess;
		}
	}
	// 初始化函数指针数组
	void(AI2::*temp[])(DIRECTION*) = {
		nullptr,//(X,Y)     纯粹只是添加进来凑数而已
		&AI2::reverse,//(X,Y)
		&AI2::reverse_Y,//(X,-Y)
		&AI2::reverse_X,//(-X,-Y)
		&AI2::reverse_Y,//(-X,Y)
		&AI2::reverseXY,//(Y,-X)
		&AI2::reverse_Y,//(Y,X)
		&AI2::reverse_X,//(-Y,X)
		&AI2::reverse_Y,//(-Y,-X)
		&AI2::reverse_X_Y//(X,Y)
	};
	for (int i = 0; i < 10; i++) {
		Reverse[i] = temp[i];
	}
}

void AI2::initChessScore(bool isFirst)
{
	for (int i = ChessStart; i < ChessEnd; i++)
	{
		for (int j = ChessStart; j < ChessEnd; j++)
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
	for (int i = ChessStart; i < ChessEnd; i++)
		for (int j = ChessStart; j < ChessEnd; j++) {
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

void AI2::resetGo2DeadStatus() {
	for (int i = ChessInit; i < ChessEnd; i++) {
		for (int j = ChessInit; j < ChessEnd; j++) {
			isGo2DeadStatus[i][j] = false;
		}
	}
}

void AI2::ScanChessBroad() {
	for (int x = ChessInit; x < ChessEnd; x++) {
		for (int y = ChessInit; y < ChessEnd; y++) {
			if (cross[x][y] == NoChess) {
				cross[x][y] = turn2Who;
				if (isGo2Dead(x, y, turn2Who)) {
					chessScore[x][y] = minLimit;
					cross[x][y] = NoChess;
					// 如果是我方的自杀点的话，就直接跳转，不用判断是否是敌方的自杀点了。
					goto unsuitable;
				}
				// 临时设置当前获得的位置为敌方着子点，判断是否是敌方的自杀点
				if (cross[x][y] == NoChess && chessScore[x][y] == 0) goto unsuitable;
				cross[x][y] = Rival;
				if (isGo2Dead(x, y, Rival)) {
					cross[x][y] = NoChess;
					// 如果是敌方的自杀点的话，这里就置零   -.-！！！
					chessScore[x][y] = 0;
					goto unsuitable;
				}
				// 这里既不是我方自杀点，也不是敌方自杀点
				cross[x][y] = NoChess;
			}
			else {
				if (isGo2Dead(x, y, cross[x][y])) {
					chessScore[x][y] = minLimit;
					// 如果是我方的自杀点的话，就直接跳转，不用判断是否是敌方的自杀点了。
					goto unsuitable;
				}
			}
		unsuitable:;
		}
	}
}

bool AI2::isContaint(goodMove move) {
	for (int i = ChessInit; i < MovePointer; ++i) {
		if (move.line == goodMoves[i].line && move.column == goodMoves[i].column
			&& move.Score == goodMoves[i].Score) {
			return true;
		}
	}
	return false;
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

int AI2::random(double start, double end)
{
	return start + (end - start)*rand() / (RAND_MAX + 1.0);
}

void AI2::rollback(int line, int column,int onTurn) {
	if (onTurn == PlayerId) {
		--chessCount;
	}
	if (cross[line][column] != NoChess) {
		cross[line][column] == NoChess;
		chessScore[line][column] = getDefaultChessScore(line, column);
	}
}