#include "../AI2_Header/AI2.h"

void AI2::initAllArray() {
	this->chessCount = 0;
	this->PlayerId = White;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			cross[i][j] = NoChess;
		}
	}
	resetGo2DeadStatus();
	initChessScore();
	// 初始化对角数组
	this->cornerArray[0] = 101;
	this->cornerArray[1] = 201;
	this->cornerArray[2] = 102;
	this->cornerArray[3] = 109;
	this->cornerArray[4] = 209;
	this->cornerArray[5] = 108;
	this->cornerArray[6] = 901;
	this->cornerArray[7] = 801;
	this->cornerArray[8] = 902;
	this->cornerArray[9] = 909;
	this->cornerArray[10] = 809;
	this->cornerArray[11] = 908;
}

void AI2::initChessScore()
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
			chessScore[i][j] = temp[i][j];
		}
	}
}

int AI2::isFinal() {
	int minCount = 0;
	for (int i = 1; i < 10; i++)
		for (int j = 1; j < 10; j++)
			if (chessScore[i][j] == minLimit)
				++minCount;
	return minCount;
}

void AI2::resetGo2DeadStatus() {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			isGo2DeadStatus[i][j] = false;
		}
	}
}
