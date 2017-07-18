#include "../AI4_Header/AI4.h"

void AI4::GetPosition(int &line, int &column, int onTurn)
{
	if (onTurn != 1 && onTurn != -1 && onTurn != 2 && onTurn != -2)
	{
		//界面的初始化命令
		if (onTurn == 0) Initialize();
		//回退一步处理
		else cross[line][column] = 0;
	}
	InitScore();
	if (Round == 0) Round = 1;
	else Round++;
	PlayerId = GetId(onTurn);//获取玩家身份
	//获取特殊点链表
	std::list<std::pair<int, int>> SP = PM.getMoves(Round == 1 ? true : false, cross);
	//更新分值
	UpdateScore(SP);
}