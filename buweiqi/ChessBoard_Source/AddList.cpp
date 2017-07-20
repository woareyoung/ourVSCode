#include "../ChessBoard_Header/ChessBoard.h"
void ChessBoard::AddSituationList()
{
	SITUATION sit;
	CurrentRoundNumber++;
	int maxQuadrant = Qua.GetMaxQuadrant();//获取最多棋子的象限
	int i, j;
	//如果最多棋子的是第一象限
	if (maxQuadrant == Qua.FirstQuadrant)
	{
		sit.ChessPosition = line * 10 + (10 - column);//记录本次走棋位置
		for (i = 1; i < 10; ++i)
			for (j = 9; j > 0; j--)
				sit.BoardStatus += std::to_string(cross[i][j]);
	}
	//如果最多棋子的是第二象限
	else if (maxQuadrant == Qua.SecondQuadrant)
	{
		sit.ChessPosition = line * 10 + column;//记录本次走棋位置
		for (i = 1; i < 10; ++i)
			for (j = 1; j < 10; ++j)
				sit.BoardStatus += std::to_string(cross[i][j]);
	}
	//如果最多棋子的是第三象限
	else if (maxQuadrant == Qua.ThirdQuadrant)
	{
		sit.ChessPosition = (10 - line) * 10 + column;//记录本次走棋位置
		for (i = 9; i > 0; i--)
			for (j = 1; j < 10; ++j)
				sit.BoardStatus += std::to_string(cross[i][j]);
	}
	//如果最多棋子的是第四象限
	else if (maxQuadrant == Qua.ForthQuadrant)
	{
		sit.ChessPosition = (10 - line) * 10 + (10 - column);//记录本次走棋位置
		for (i = 9; i > 0; i--)
			for (j = 9; j > 0; j--)
				sit.BoardStatus += std::to_string(cross[i][j]);
	}
	MemBattle.push_back(sit);
	if (line < 5 && column > 5) Qua.FirstQuadrant++;
	else if (line < 5 && column < 5) Qua.SecondQuadrant++;
	else if (line > 5 && column < 5) Qua.ThirdQuadrant++;
	else if (line > 5 && column > 5) Qua.ForthQuadrant++;
}

void ChessBoard::AddList(int type)
{
	PACE pac;
	pac.line = line;
	pac.column = column;
	pac.player = type;
	MemPace.push_back(pac);
	AddSituationList();
}