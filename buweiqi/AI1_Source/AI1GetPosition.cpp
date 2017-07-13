#include "../stdafx.h"
#include "../AI1_Header/AI1.h"

#define MAX_SIMILAR 2 //����ͬ��������ﵽ����2�κ�ı����
#define ThreadAmount 5//�߳���
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
			GetMaxScorePosition();
			np.insert(MaxScorePosition);
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
		if (DeadCheck(line, column, OT, cross) == true && MaxScore > PointStyle[9])
		{
			Score[line][column] = PointStyle[9];
			continue;
		}
		///��������λ�ã��������ϻ���λ�ò������壬�����ѭ��
		if (DeadCheck(line, column, PlayerId, cross) == true && MaxScore > PointStyle[1])
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
			if (np.empty())
			{
				if (Similar == -1) Similar = -1;
				break;
			}
			else
			{
				if (Similar == -1) Similar = 2;
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
	//�������ֻ��һ��Ԫ��
	if (np.size() == 1)
	{
		std::set<int>::iterator i = np.begin();
		BestSite = *i;
		np.clear();
		return BestSite;
	}
	double maxScore = -100;
	bool ThreadGo[ThreadAmount] = { false };//����߳��Ƿ�����ִ��
	int i;

	while (!np.empty())
	{
		for (i = 0; i < ThreadAmount; i++)
		{
			if (np.empty()) break;
			if (!ThreadGo[i])//����߳̿���״��
			{
				ThreadGo[i] = true;//����߳�����ִ��
				std::async(std::launch::async, [&]() {
//					g_lock.lock();//�ӻ�������������ʳ�ͻ
					auto t = np.begin();//��ȡ��λ�õĵ�һ��
					int tempPos = *t;
					np.erase(tempPos);//����λ�ôӺ��б��в���
					double ttt = CalDeadPosNumber(tempPos / 10, tempPos % 10);//��ȡ��λ�õ�����
					if (ttt > maxScore)
					{
						maxScore = ttt;
						BestSite = tempPos;
					}
//					g_lock.unlock();//����
				});
				ThreadGo[i] = false;//����߳̿���
			}
		}
	}
	bool wait = true;
	while (wait)
	{
		for (int i = 0; i < ThreadAmount; i++)
		{
			if (ThreadGo[i]) break;//��������߳�����ִ�У������̵߳ȴ�
			else if (i == ThreadAmount - 1) wait = false;
		}
	}
	np.clear();
	return BestSite;
}