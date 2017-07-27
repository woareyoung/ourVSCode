#include "../AI4_Header/AI4.h"
#include "../ChessBoard_Header/Pattern_Moves.h"

void AI4::GetPosition(int &line, int &column, int onTurn)
{
	if (onTurn != 1 && onTurn != -1 && onTurn != 2 && onTurn != -2)
	{
		//界面的初始化命令
		if (onTurn == 0) Initialize();
		//回退一步处理
		else cross[line][column] = 0;
		return;
	}
	InitScore();
	if (Round == 0)
	{
		Round = 1;
		FirstPaceScore();
	}
	else Round++;
	PlayerId = GetId(onTurn);//获取玩家身份
	//判断是不是第一步
	if (line != 0 && column != 0)
	{
		cross[line][column] = 3 - PlayerId;
		Score[line][column] = HaveChess;
	}
	//获取特殊点链表
	Pattern_Moves PM(PlayerId);
	std::list<std::pair<int, int>> SP = PM.getMoves(true, cross);
	//更新分值
	UpdateBoardScore(SP);
	for (int i = 1; i < 10; ++i)
	{
		for (int j = 1; j < 10; ++j)
		{
			if (cross[i][j] != 0) continue;
			int save = Score[i][j];
			Score[i][j] += DeadPosNumberAddScore(i, j);
			if (Score[i][j] < 0)
			{
				if (save < 0) Score[i][j] = save;
				else Score[i][j] = 0;
			}
		}
	}
	//获取最高分位置
	GetMaxScorePosition(line, column);
	cross[line][column] = PlayerId;
	Score[line][column] = HaveChess;
}