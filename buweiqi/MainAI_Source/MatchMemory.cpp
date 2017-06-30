#include "../ChessBoard_Header/AI.h"

void AI::Statistic(int line, int column)
{
	if (line > 5 && column < 5) Qua.FirstQuadrant++;
	else if (line < 5 && column < 5) Qua.SecondQuadrant++;
	else if (line < 5 && column > 5) Qua.ThirdQuadrant++;
	else if (line > 5 && column > 5) Qua.ForthQuadrant++;
}
std::shared_ptr<NEXTPACE> AI::MatchMemory(int line, int column, bool &None)
{
	SITUATION NowStatus;
	Statistic(line, column);//��ͳ�Ƶ�ǰ����
	int maxQuadrant = Qua.GetMaxQuadrant();//��ȡ������ӵ�����
	int rival = 3 - PlayerId;
	int i, j;
	int TempSum;
	//���������ӵ��ǵ�һ����
	if (maxQuadrant == Qua.FirstQuadrant)
	{
		for (i = 1; i < 10; ++i)
		{
			TempSum = 1;
			for (j = 9; j > 0; j--)
			{
				if (cross[i][j] == PlayerId) NowStatus.Line[i] += pow(2, 10 - j) * 10000;
				else if (cross[i][j] == rival) NowStatus.Line[i] += pow(2, 10 - j);
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
				if (cross[i][j] == PlayerId) NowStatus.Line[i] += pow(2, j) * 10000;
				else if (cross[i][j] == rival) NowStatus.Line[i] += pow(2, j);
			}
		}
	}
	//���������ӵ��ǵ�������
	else if (maxQuadrant == Qua.ThirdQuadrant)
	{
		for (i = 9; i > 0; i--)
		{
			for (j = 1; j < 10; ++j)
			{
				if (cross[i][j] == PlayerId) NowStatus.Line[10 - i] += pow(2, j) * 10000;
				else if (cross[i][j] == rival) NowStatus.Line[10 - i] += pow(2, j);
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
				if (cross[i][j] == PlayerId) NowStatus.Line[10 - i] += pow(2, 10 - j) * 10000;
				else if (cross[i][j] == rival) NowStatus.Line[10 - i] += pow(2, 10 - j);
			}
		}
	}
	std::shared_ptr<NEXTPACE> np = FS.Match(NowStatus, PlayerId, CurrentRound);
	//�����һģһ���ļ�¼����ֱ�Ӹ�����
	if (np != NULL)
	{
		None = false;
		return np;
	}
	for (i = CurrentRound + 2; ; i = i + 2)
	{
		//û��һģһ���ļ�¼�����ѯ��û�к��е�ǰ����ġ����̡�
		if(CurrentNull > i) np = FS.GenerMatch(NowStatus, PlayerId, i);
		else return NULL;
		if (np != nullptr)
		{
			None = true;
			return np;
		}
	}
	None = true;
	//���ʲô��û��
	return NULL;
}

void AI::ClearList(std::shared_ptr<NEXTPACE> head)
{
	std::shared_ptr<NEXTPACE> temp = head->next;
	// delete head;
	head = nullptr;
	while (temp != NULL)
	{
		head = temp;
		temp = temp->next;
		// delete head;
		head = nullptr;
	}
}
