#include "../ChessBoard_Header/AI.h"
#define MAX_ROUND_K 4

void AI::Statistic(int line, int column)
{
	if (line < 5 && column > 5) Qua.FirstQuadrant++;
	else if (line < 5 && column < 5) Qua.SecondQuadrant++;
	else if (line > 5 && column < 5) Qua.ThirdQuadrant++;
	else if (line > 5 && column > 5) Qua.ForthQuadrant++;
}
bool AI::MatchMemory(int line, int column, std::set<int> &res)
{
	Statistic(line, column);//��ͳ�Ƶ�ǰ����
	int maxQuadrant = Qua.GetMaxQuadrant();//��ȡ������ӵ�����
	int i;
	GetCurrentStatus(maxQuadrant);
	line = FS.Match(NowStatus, res, CurrentRound, true);
	//�����һģһ���ļ�¼����ֱ�Ӹ�����
	if (!res.empty()) return false;
	for (i = CurrentRound + MAX_ROUND_K; ; i = i + 2)
	{
		//û��һģһ���ļ�¼�����ѯ��û�к��е�ǰ����ġ����̡�
		if(i < 81) column = FS.GenerMatch(NowStatus, res, i, true);
		else return false;
		if (!res.empty()) return true;
	}
	return true;
}
void AI::GetCurrentStatus(int maxQuadrant)
{
	int rival = 3 - PlayerId;
	int TempSum, i, j;
	for (i = 0; i < 10; i++) NowStatus.Line[i] = 0;
	//���������ӵ��ǵ�һ����
	if (maxQuadrant == Qua.FirstQuadrant)
	{
		for (i = 1; i < 10; ++i)
		{
			TempSum = 1;
			for (j = 9; j > 0; j--)
			{
				TempSum *= 2;
				if (cross[i][j] == PlayerId) NowStatus.Line[i] += TempSum * 10000;
				else if (cross[i][j] == rival) NowStatus.Line[i] += TempSum;
			}
		}
	}
	//���������ӵ��ǵڶ�����
	else if (maxQuadrant == Qua.SecondQuadrant)
	{
		for (i = 1; i < 10; ++i)
		{
			TempSum = 1;
			for (j = 1; j < 10; ++j)
			{
				TempSum *= 2;
				if (cross[i][j] == PlayerId) NowStatus.Line[i] += TempSum * 10000;
				else if (cross[i][j] == rival) NowStatus.Line[i] += TempSum;
			}
		}
	}
	//���������ӵ��ǵ�������
	else if (maxQuadrant == Qua.ThirdQuadrant)
	{
		for (i = 9; i > 0; i--)
		{
			TempSum = 1;
			for (j = 1; j < 10; ++j)
			{
				TempSum *= 2;
				if (cross[i][j] == PlayerId) NowStatus.Line[10 - i] += TempSum * 10000;
				else if (cross[i][j] == rival) NowStatus.Line[10 - i] += TempSum;
			}
		}
	}
	//���������ӵ��ǵ�������
	else if (maxQuadrant == Qua.ForthQuadrant)
	{
		for (i = 9; i > 0; --i)
		{
			TempSum = 1;
			for (j = 9; j > 0; --j)
			{
				TempSum *= 2;
				if (cross[i][j] == PlayerId) NowStatus.Line[10 - i] += TempSum * 10000;
				else if (cross[i][j] == rival) NowStatus.Line[10 - i] += TempSum;
			}
		}
	}
}