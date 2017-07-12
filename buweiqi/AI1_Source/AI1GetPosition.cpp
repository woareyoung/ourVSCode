#include "../stdafx.h"
#include "../AI1_Header/AI1.h"
#define MAX_SIMILAR 2 //设置同样的走棋达到连续3次后改变规律
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
			if (Similar != 0) Similar--;
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
		np.clear();
		None = MatchMemory(line, column, np);//获取对局记录中符合当前盘面的对应方法
		//如果对局记录中有应对的方法
		if (!np.empty())
		{
			NextPace = GetNextPace(np);
			if (NextPace > 0) abc = false;
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
		}
	}
	///若是死棋位置则一直循环，直到不是死棋位置
	while (abc)
	{
		GetMaxScorePosition();
		line = MaxScorePosition / 10;
		column = MaxScorePosition % 10;
		if (cross[line][column] != 0) continue;//这句虽然没什么用，但保险起见
		///若该位置对于对方来说是死棋，则继续循环
		if (DeadCheck(line, column, OT, true) == true && MaxScore > PointStyle[9])
		{
			Score[line][column] = PointStyle[9];
			continue;
		}
		///若是死棋位置，且棋盘上还有位置不是死棋，则继续循环
		if (DeadCheck(line, column, PlayerId, true) == true && MaxScore > PointStyle[1])
		{
			Score[line][column] = PointStyle[1];
			continue;
		}
		//先检查有没有“重蹈覆辙”
		else
		{
			Statistic(line, column);
			cross[line][column] = PlayerId;
			GetCurrentStatus(Qua.GetMaxQuadrant(), NowStatus);
			cross[line][column] = 0;
			BackQua(line, column);
			np.clear();
			FS.Match(NowStatus, np, CurrentRound + 1, 3 - PlayerId);//搜索出同样的局面输的一方的下棋位置
			if (np.empty()) break;
			else Similar++;
			np.clear();
			if (Similar == MAX_SIMILAR)
			{
				Similar = 0;
				Score[line][column] *= 0.5;
				continue;
			}
			break;
		}
	}
	cross[line][column] = PlayerId;
	cross[line][0] = 1;
	Statistic(line, column);
	UpdateScore(line, column, PlayerId);
	CurrentRound++;
	CurrentNull--;
}
///从链表中选取最高胜率的结点
int AI1::GetNextPace(std::set<int> &np)
{
	int BestSite = -1;
	if (np.size() == 1)
	{
		std::set<int>::iterator i = np.begin();
		BestSite = *i;
		np.clear();
		return BestSite;
	}
//	FS.ReadFile(CurrentRound + 2);
	BestSite = *np.begin();
	np.clear();
	return BestSite;
}