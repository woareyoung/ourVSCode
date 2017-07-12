#include "../stdafx.h"
#include "../AI1_Header/AI1.h"
#define MAX_SIMILAR 2 //����ͬ��������ﵽ����3�κ�ı����
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
		np.clear();
		None = MatchMemory(line, column, np);//��ȡ�Ծּ�¼�з��ϵ�ǰ����Ķ�Ӧ����
		//����Ծּ�¼����Ӧ�Եķ���
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
			cross[line][column] = PlayerId;
			GetCurrentStatus(Qua.GetMaxQuadrant(), NowStatus);
			cross[line][column] = 0;
			BackQua(line, column);
			np.clear();
			FS.Match(NowStatus, np, CurrentRound + 1, 3 - PlayerId);//������ͬ���ľ������һ��������λ��
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
///��������ѡȡ���ʤ�ʵĽ��
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