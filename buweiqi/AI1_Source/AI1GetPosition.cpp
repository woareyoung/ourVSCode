#include "../stdafx.h"
#include "../AI1_Header/AI1.h"

#define MAX_SIMILAR 2 //设置同样的走棋达到连续2次后改变规律
#define ThreadAmount 6//线程数
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
		CalDeadPosNumber(line, column, DoubleDontDead, false);
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
	while (ForeReadFinish == false) {}//等待预读功能完成
	if (abc)
	{
		bool None;
		int NextPace;
		np.clear();
		None = MatchMemory(line, column, np);//获取对局记录中符合当前盘面的对应方法
		//如果对局记录中有应对的方法
		if (!np.empty())
		{
			GetMaxScorePosition();
			np.insert(MaxScorePosition);
			NextPace = GetNextPace(np);
			if (NextPace < 0) abc = true;
			else
			{
				line = NextPace / 10;
				column = NextPace % 10;
				abc = false;
			}
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
		if (DeadCheck(line, column, OT, cross) == true && MaxScore > PointStyle[9])
		{
			Score[line][column] = PointStyle[9];
			continue;
		}
		///若是死棋位置，且棋盘上还有位置不是死棋，则继续循环
		if (DeadCheck(line, column, PlayerId, cross) == true && MaxScore > PointStyle[1])
		{
			Score[line][column] = PointStyle[1];
			continue;
		}
		//先检查有没有“重蹈覆辙”
		else if(DoubleDontDead != 0)
		{
			Statistic(line, column);
			cross[line][column] = PlayerId;
			GetCurrentStatus(Qua.GetMaxQuadrant(), NowStatus);
			cross[line][column] = 0;
			BackQua(line, column);
			np.clear();
			FS.Match(NowStatus, np, CurrentRound + 1, 3 - PlayerId);//搜索出同样的局面输的一方的下棋位置
			if (np.empty())
			{
				if (Similar == -1) Similar = 0;
				break;
			}
			else
			{
				if (Similar == -1) Similar = MAX_SIMILAR - 1;
				else Similar++;
			}
			np.clear();
			if (Similar == MAX_SIMILAR)
			{
				Similar = -1;
				Score[line][column] *= 0.5;
				continue;
			}
			break;
		}
		else break;
	}
	cross[line][column] = PlayerId;
	Statistic(line, column);
	UpdateScore(line, column, PlayerId);
	CurrentRound++;
	CurrentNull--;
	std::async(std::launch::async, [&]() {
		ForeReadFileToMemory(CurrentRound);
	});
}
///从链表中选取最高胜率的结点
int AI1::GetNextPace(std::set<int> &np)
{
	int BestSite = -1;
	//如果容器只有一个元素
	if (np.size() == 1)
	{
		std::set<int>::iterator i = np.begin();
		BestSite = *i;
		np.clear();
		return BestSite;
	}
	double maxScore = -100;
	bool ThreadGo[ThreadAmount] = { false };//标记线程是否正在执行
	int i;
	int n;
	int Line = 0, Column = 0;
	while (!np.empty())
	{
		for (i = 0; i < ThreadAmount; i++)
		{
			if (np.empty()) break;
			if (!ThreadGo[i])//如果线程空闲状况
			{
				ThreadGo[i] = true;//标记线程已在执行
				std::async(std::launch::async, [&]() {
					auto t = np.begin();//获取候补位置的第一个
					int tempPos = *t;
					np.erase(tempPos);//将该位置从候补列表中擦除
					SymmetryExchange(Line, Column, tempPos);
					if(DeadCheck(Line, Column, PlayerId, cross) == false)
					{
						double ttt = CalDeadPosNumber(Line, Column, n);//获取该位置的评价
						if (ttt > maxScore)
						{
							maxScore = ttt;
							BestSite = Line * 10 + Column;
						}
					}
				});
				ThreadGo[i] = false;//标记线程空闲d
			}
		}
	}
	bool wait = true;
	while (wait)
	{
		for (int i = 0; i < ThreadAmount; i++)
		{
			if (ThreadGo[i]) break;//如果还有线程正在执行，则主线程等待
			else if (i == ThreadAmount - 1) wait = false;
		}
	}
	np.clear();
	return BestSite;
}

void AI1::SymmetryExchange(int &line, int &column, int site)
{
	int maxQ = Qua.GetMaxQuadrant();
	if (maxQ == Qua.FirstQuadrant)
	{
		line = site / 10;
		column = 10 - site % 10;
	}
	else if (maxQ == Qua.SecondQuadrant)
	{
		line = site / 10;
		column = site % 10;
	}
	else if (maxQ == Qua.ThirdQuadrant)
	{
		line = 10 - site / 10;
		column = site % 10;
	}
	else
	{
		line = 10 - site / 10;
		column = 10 - site % 10;
	}
}