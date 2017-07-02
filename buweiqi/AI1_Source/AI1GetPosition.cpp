#include "../stdafx.h"
#include "../AI1_Header/AI1.h"
#define MAX_SIMILAR 3 //设置同样的走棋达到连续3次后改变规律
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
			CurrentNull = 81;
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
	bool abc = true;
	OT = (onTurn == 1 || onTurn == -1) ? 1 : 2;
	PlayerId = 3 - OT;//设置该AI的玩家编号
	if (CurrentRound == 0)
	{
		if (PlayerId == 2) CurrentRound = 2;
		else CurrentRound = 1;
	}
	else
	{
		CurrentRound++;
		CurrentNull--;
	}
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
		bool None;
		int NextPace;
		np = MatchMemory(line, column, None);//获取对局记录中符合当前盘面的对应方法
		//如果对局记录中有应对的方法
		if (np != nullptr)
		{
			if (None == false) NextPace = np->site;//获取下一步棋的位置
			else NextPace = GetNextPace(np);
			int maxQ = Qua.GetMaxQuadrant();
			if (maxQ == Qua.FirstQuadrant)
			{
				line = NextPace / 10;
				column = 10 - NextPace % 10;
			}
			else if (maxQ == Qua.SecondQuadrant)
			{
				line = NextPace / 10;
				column = NextPace % 10;
			}
			else if (maxQ == Qua.ThirdQuadrant)
			{
				line = 10 - NextPace / 10;
				column = NextPace % 10;
			}
			else
			{
				line = 10 - NextPace / 10;
				column = 10 - NextPace % 10;
			}
			cross[line][column] = PlayerId;
			abc = false;
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
		//先检查有没有“重蹈覆辙”
		else
		{
			Statistic(line, column);
			GetCurrentStatus(Qua.GetMaxQuadrant());
			np = FS.Match(NowStatus, PlayerId, CurrentRound + 1);//搜索出同样的局面输的一方的下棋位置
			if (np == nullptr)
			{
				BackQua(line, column);
				break;
			}
			else Similar++;
			ClearList(np);
			if (Similar == MAX_SIMILAR)
			{
				Similar = 0;
				BackQua(line, column);
				Score[line][column] *= 0.75;
				cross[line][column] = 0;
				continue;
			}
			break;
		}
	}
	cross[line][0] = 1;
	Statistic(line, column);
	UpdateScore(line, column, PlayerId);
	CurrentRound++;
	CurrentNull--;
}
///从链表中选取最高分的结点
int AI1::GetNextPace(std::shared_ptr<NEXTPACE> np)
{
	int Max = cross[np->site / 10][np->site % 10];
	int nextpace = np->site;
	std::shared_ptr<NEXTPACE> temp = np->next;
	np = nullptr;
	while(temp != nullptr)
	{
		if (cross[temp->site / 10][temp->site % 10] > Max)
		{
			Max = cross[temp->site / 10][temp->site % 10];
			nextpace = temp->site;
		}
		np = temp;
		temp = temp->next;
		np = nullptr;
	}
	return nextpace;
}
void AI1::BackQua(int line, int column)
{
	if (line < 5 && column > 5) Qua.FirstQuadrant--;
	else if (line < 5 && column < 5) Qua.SecondQuadrant--;
	else if (line > 5 && column < 5) Qua.ThirdQuadrant--;
	else if (line > 5 && column > 5) Qua.ForthQuadrant--;
}