#include "../stdafx.h"
#include "../AI1_Header/AIForFile.h"
#define MAX_ROUND_K 70

bool AIForFile::MatchMemory(int line, int column, std::set<int> &res)
{
	Statistic(line, column);//��ͳ�Ƶ�ǰ����
	int i;
	GetCurrentStatus(Qua.GetMaxQuadrant(), NowStatus);
	std::pair<int, int> p;
	if (ForeReadHaveMem)
	{
		auto ran = ForeReadContent.equal_range(NowStatus.BoardStatus);
		for (auto it = ran.first; it != ran.second; it++)
		{
			p = it->second;//��ȡ��ֵ��
			res.insert(p.first);//��ȡ��ֵ���ĵ�һ��Ԫ��
		}
	}
	else FS.Match(NowStatus, res, CurrentRound, PlayerId);
	//�����һģһ���ļ�¼����ֱ�Ӹ�����
	if (!res.empty()) return false;
	for (i = CurrentRound > MAX_ROUND_K ? CurrentRound : MAX_ROUND_K; ; i = i + 1)
	{
		//û��һģһ���ļ�¼�����ѯ��û�к��е�ǰ����ġ����̡�
		if(i < 81) FS.GenerMatch(NowStatus, res, i, PlayerId);
		else return false;
		if (!res.empty()) return true;
	}
	return true;
}
void AIForFile::GetCurrentStatus(int maxQuadrant, SITUATION &CurrentStatus)
{
	int i, j;
	for (i = 0; i < 10; i++) CurrentStatus.BoardStatus = "";
	//���������ӵ��ǵ�һ����
	if (maxQuadrant == Qua.FirstQuadrant)
	{
		for (i = 1; i < 10; ++i)
			for (j = 9; j > 0; j--)
				CurrentStatus.BoardStatus += std::to_string(cross[i][j]);
	}
	//���������ӵ��ǵڶ�����
	else if (maxQuadrant == Qua.SecondQuadrant)
	{
		for (i = 1; i < 10; ++i)
			for (j = 1; j < 10; ++j)
				CurrentStatus.BoardStatus += std::to_string(cross[i][j]);
	}
	//���������ӵ��ǵ�������
	else if (maxQuadrant == Qua.ThirdQuadrant)
	{
		for (i = 9; i > 0; i--)
			for (j = 1; j < 10; ++j)
				CurrentStatus.BoardStatus += std::to_string(cross[i][j]);
	}
	//���������ӵ��ǵ�������
	else if (maxQuadrant == Qua.ForthQuadrant)
	{
		for (i = 9; i > 0; i--)
			for (j = 9; j > 0; j--)
				CurrentStatus.BoardStatus += std::to_string(cross[i][j]);
	}
}