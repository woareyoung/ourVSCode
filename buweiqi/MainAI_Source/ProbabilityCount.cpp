#include "../ChessBoard_Header/AI.h"
#include "../ChessBoard_Header/SIP.h"
#include <future>

double AI::ProbabilityCount(int site)
{
	SITUATION SaveData = NowStatus;//保存数据（相当于压栈）
	double probability = 0;
	int line = site / 10;
	int column = site % 10;
	int winCount = 0;
	int loseCount = 0;
	bool wait = true;
	///模拟数据
	cross[line][column] = PlayerId;//模拟下棋
	Statistic(line, column);
	GetCurrentStatus(Qua.GetMaxQuadrant());//获取盘面状况数据
	//计算失败频数
	std::async(std::launch::async, [&] () 
	{
		FS.Match(NowStatus, loseCount, CurrentRound + 1, false);
		wait = false;
	});
	FS.GenerMatch(NowStatus, winCount, CurrentRound + 2);
	while (wait) {}//等待线程执行完毕
	probability = winCount / ((double)winCount + (double)loseCount);//计算胜利频率
	///恢复数据
	cross[line][column] = 0;
	BackQua(line, column);
	NowStatus = SaveData;
	return probability;
}