#include "../stdafx.h"
#include "../AI1_Header/AI1.h"
///获取下棋位置
void AI1::GetPosition(int &line, int &column, int onTurn)
{
	//用于响应主窗口对AI的检查
	if (onTurn == 0)
	{
		line++;
		column++;
		InitializeD();
		return;
	}
	else if (onTurn == 100)
	{
		UpdateScore(line, column, 1, false);
		return;
	}
	else if (onTurn == 200)
	{
		UpdateScore(line, column, 2, false);
		return;
	}
	OT = (onTurn == 1 || onTurn == -1) ? 1 : 2;
	PlayerNumber = OT == 1 ? 2 : 1;//设置该AI的玩家编号
	CalculatePerf(line, column);//分析对手并预测（未写完，先不要调用）
	cross[line][column] = OT;//先更新棋盘信息数组
	///按比例缩小分值
	if (line < 3 || line > 7 || column < 3 || column > 7) RateResetScore(0.3);
	else RateResetScore(0.5);
	UpdateScore(line, column, OT);//更新分值
	if (line != 0) Display(OT, line, column);//输出棋盘分值
	///若是死棋位置则一直循环，直到不是死棋位置
	while (true)
	{
		GetMaxScorePosition();
		line = MaxScorePosition / 10;
		column = MaxScorePosition % 10;
		if (cross[line][column] != 0) continue;//这句虽然没什么用，但保险起见
		cross[line][column] = OT;
		///若该位置对于对方来说是死棋，则继续循环
		if (DeadCheck(line, column, OT) == true && MaxScore > PointStyle[9])
		{
			Score[line][column] = PointStyle[9];
			cross[line][column] = 0;
			continue;
		}
		cross[line][column] = PlayerNumber;
		///若是死棋位置，且棋盘上还有位置不是死棋，则继续循环
		if (DeadCheck(line, column, PlayerNumber) == true && MaxScore > PointStyle[1])
		{
			Score[line][column] = PointStyle[1];
			cross[line][column] = 0;
			continue;
		}
		else break;
	}
	UpdateScore(line, column, PlayerNumber);
	Display(PlayerNumber, line, column);//输出棋盘分值
}
///控制台显示信息函数
void AI1::Display(int n, int line, int column)
{
	/*
	_cprintf("Player %d:  line:%d  column:%d\n\t", n, line, column);
	for (int i = 0; i < 10; )
	{
		for (int j = 1; j < 10; j++)
		{
			if (i == 0) _cprintf("%d\t", j);
			else _cprintf("%.2f\t", Score[i][j]);
			if (j == 9) _cprintf("\n");
		}
		if (++i < 10) _cprintf("%d\t", i);
	}
	_cprintf("\n");
	*/
}
