#include "../stdafx.h"
#include "../AI1_Header/AI1.h"
#define MAX_SIMILAR 3 //����ͬ��������ﵽ����3�κ�ı����
///��ȡ����λ��
void AI1::GetPosition(int &line, int &column, int onTurn)
{
	if (onTurn != 1 && onTurn != -1 && onTurn != 2 && onTurn != -2)
	{
		//������Ӧ�����ڶ�AI�ļ��
		if (onTurn == 0)
		{
			line++;
			column++;
			CurrentRound = 0;
			CurrentNull = 81;
			InitializeD();
		}
		//��һ������
		else if (onTurn == 100 || onTurn == 200)
		{
			CurrentRound--;
			int play = onTurn / 100;
			if (play == 3 - PlayerId)
			{
				UpdateScore(line, column, 3 - PlayerId, false);
				///��������С��ֵ
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
	PlayerId = 3 - OT;//���ø�AI����ұ��
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
	//������������壬����������״̬�ĸı�
	if (line != 0 && column != 0)
	{
		cross[line][column] = OT;//�ȸ���������Ϣ����
		//��������С��ֵ
		if (line < 3 || line > 7 || column < 3 || column > 7) RateResetScore(0.78);
		else RateResetScore(0.92);
		UpdateScore(line, column, OT, true);
	}
	//����ǵ�һ������
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
		np = MatchMemory(line, column, None);//��ȡ�Ծּ�¼�з��ϵ�ǰ����Ķ�Ӧ����
		//����Ծּ�¼����Ӧ�Եķ���
		if (np != nullptr)
		{
			NextPace = GetNextPace(np);
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
	///��������λ����һֱѭ����ֱ����������λ��
	while (abc)
	{
		GetMaxScorePosition();
		line = MaxScorePosition / 10;
		column = MaxScorePosition % 10;
		if (cross[line][column] != 0) continue;//�����Ȼûʲô�ã����������
		///����λ�ö��ڶԷ���˵�����壬�����ѭ��
		if (DeadCheck(line, column, OT, true) == true && MaxScore > PointStyle[9])
		{
			Score[line][column] = PointStyle[9];
			continue;
		}
		///��������λ�ã��������ϻ���λ�ò������壬�����ѭ��
		if (DeadCheck(line, column, PlayerId, true) == true && MaxScore > PointStyle[1])
		{
			Score[line][column] = PointStyle[1];
			continue;
		}
		//�ȼ����û�С��ص����ޡ�
		else
		{
			Statistic(line, column);
			GetCurrentStatus(Qua.GetMaxQuadrant());
			np = FS.Match(NowStatus, OT, CurrentRound + 1, false);//������ͬ���ľ������һ��������λ��
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
			else BackQua(line, column);
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
///��������ѡȡ���ʤ�ʵĽ��
int AI1::GetNextPace(std::shared_ptr<NEXTPACE> np)
{
	int BestSite;
	if (np->next == nullptr)
	{
		BestSite = np->site;
		np = nullptr;
		return BestSite;
	}
	double MaxPro = -2;
	double tmp;
	std::shared_ptr<NEXTPACE> temp = np;
	while(temp != nullptr)
	{
		if (cross[temp->site / 10][temp->site % 10] == 0)
		{
			tmp = ProbabilityCount(temp->site);
			if (tmp > MaxPro)
			{
				MaxPro = tmp;
				BestSite = temp->site;
			}
		}
		np = temp;
		temp = temp->next;
		np = nullptr;
	}
	return BestSite;
}