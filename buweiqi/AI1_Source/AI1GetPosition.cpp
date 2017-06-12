#include "../stdafx.h"
#include "../AI1_Header/AI1.h"
///获取下棋位置
void AI1::GetPosition(int &line, int &column, int onTurn)
{
	if (onTurn != 1 && onTurn != -1 && onTurn != 2 && onTurn != -2)
	{
		//用于响应主窗口对AI的检查
		if (onTurn == 0)
		{
			line++;
			column++;
			CurrentRound = 0;
			InitializeD();
		}
		//退一步操作
		else if (onTurn == 100 || onTurn == 200)
		{
			CurrentRound--;
			int play = onTurn / 100;
			if (play == 3 - PlayerId)
			{
				UpdateScore(line, column, 3 - PlayerId, false);
				///按比例缩小分值
				if (line < 3 || line > 7 || column < 3 || column > 7) RateResetScore(0.78, false);
				else RateResetScore(0.92, false);
			}
			else UpdateScore(line, column, play, false);
		}
		return;
	}
	CurrentRound++;
	bool abc = true;
	OT = (onTurn == 1 || onTurn == -1) ? 1 : 2;
	PlayerId = 3 - OT;//设置该AI的玩家编号
	//如果是正常下棋，则作出棋盘状态的改变
	if (line != 0 && column != 0)
	{
		cross[line][column] = OT;//先更新棋盘信息数组
		//按比例缩小分值
		if (line < 3 || line > 7 || column < 3 || column > 7) RateResetScore(0.78);
		else RateResetScore(0.92);
		UpdateScore(line, column, OT, true);
		//Display(OT, line, column);
	}
	//如果是第一步下棋
	else
	{
		GetMaxScorePosition();
		line = MaxScorePosition / 10;
		column = MaxScorePosition % 10;
		cross[line][column] = PlayerId;
		abc = false;
	}
	if (abc)
	{
		int NextPace;
		np = MatchMemory(line, column);//获取对局记录中符合当前盘面的对应方法
		//如果对局记录中有应对的方法
		if (np != NULL)
		{
			NextPace = np->site;//获取下一步棋的位置
			abc = false;
			line = NextPace / 10;
			column = NextPace % 10;
			cross[line][column] = PlayerId;
		}
	}
	///若是死棋位置则一直循环，直到不是死棋位置
	while (abc)
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
		cross[line][column] = PlayerId;
		///若是死棋位置，且棋盘上还有位置不是死棋，则继续循环
		if (DeadCheck(line, column, PlayerId) == true && MaxScore > PointStyle[1])
		{
			Score[line][column] = PointStyle[1];
			cross[line][column] = 0;
			continue;
		}
		else break;
	}
	cross[line][0] = 1;
	Statistic(line, column);
	UpdateScore(line, column, PlayerId);
	CurrentRound++;
	//	Display(PlayerNumber, line, column);
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
