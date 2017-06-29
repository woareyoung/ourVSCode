#include "../AI2_Header/DefaultChess.h"

DefaultChess::DefaultChess()
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
			DefaultChessScore[i][j] = temp[i][j];
		}
	}
}

DefaultChess::~DefaultChess()
{
}

int DefaultChess::getDefaultChessScore(int line, int column) {
	return DefaultChessScore[line][column];
}