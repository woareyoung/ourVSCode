#include "../stdafx.h"
#include "../AI2_Header/AI2.h"

/**
* [AI2::GetPosition AI对外接口]
* @param line 			着子位置的行坐标
* @param column 		着子位置的行坐标
* @param onTurn 		上一步敌方的ID
* @param isExist[10][10]		棋盘着子情况数组
*/
void AI2::GetPosition(int& line, int& column, int onTurn)
{
	//接收到界面的远程命令
	if (onTurn != 1 && onTurn != 2)
	{
		//用于响应主窗口对AI的检查
		if (onTurn == 0)
		{
			// 这里是重新开始游戏的数据重置过程
			line++;
			column++;
			resetGo2DeadStatus();
			initAllArray();
		}
		//回退一步的命令，对数据进行回退。（100为玩家1，200为玩家2，line与column为回退的位置）
		else if (onTurn == 100 || onTurn == 200)
		{

		}
		return;
	}
	/******************************************
	添加敌方着子点到我方棋盘上
	*******************************************/
	this->chessCount++;
	// 注意传递进来的onTurn参数是对方的，而不是己方的。
	Rival = (onTurn == Black || onTurn == isAI1onTurn) ? Black : White;
	this->turn2Who = getRival(Rival);
	this->PlayerId = turn2Who;
	PlayerId = turn2Who;
	cross[line][column] = Rival;

	/******************************************
	我方着子
	*******************************************/
	ScanChessBroad();
	// 设置遍历的深度
	int temp = maxandmin(1);
	line = temp / 100;
	column = temp % 100;
	cross[line][column] = turn2Who;
	// 将计算出的位置的分数设为最小值
/*
	_cprintf("**************This is chess score*******(%d, %d)***********\n", line, column);
	for (int i = 1; i < 10; i++)
	{
		for (int j = 1; j < 10; j++)
			_cprintf("%d\t", chessScore[i][j]);
		_cprintf("\n");
	}
	_cprintf("**************This is chess cross*******(%d, %d)***********\n", line, column);
	for (int i = 1; i < 10; i++)
	{
		for (int j = 1; j < 10; j++)
			_cprintf("%d\t", cross[i][j]);
		_cprintf("\n");
	}
*/
}
