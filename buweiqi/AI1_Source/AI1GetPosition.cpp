#include "../stdafx.h"
#include "../AI1_Header/AI1.h"
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
			InitializeD();
		}
		else if (onTurn == 100 || onTurn == 200)
		{
			int play = onTurn / 100;
			if (play == 3 - PlayerNumber)
			{
				UpdateScore(line, column, 3 - PlayerNumber, false);
				///��������С��ֵ
				if (line < 3 || line > 7 || column < 3 || column > 7) RateResetScore(0.78, false);
				else RateResetScore(0.92, false);
			}
			else UpdateScore(line, column, play, false);
		}
		return;
	}
	bool abc = true;
	OT = (onTurn == 1 || onTurn == -1) ? 1 : 2;
	PlayerNumber = 3 - OT;//���ø�AI����ұ��
	if (line != 0 && column != 0)
	{
		cross[line][column] = OT;//�ȸ���������Ϣ����
		///��������С��ֵ
		if (line < 3 || line > 7 || column < 3 || column > 7) RateResetScore(0.78);
		else RateResetScore(0.92);
		UpdateScore(line, column, OT, true);
		//Display(OT, line, column);
	}
	else
	{
		GetMaxScorePosition();
		line = MaxScorePosition / 10;
		column = MaxScorePosition % 10;
		cross[line][column] = PlayerNumber;
		abc = false;
	}
	if (abc)
	{
		Statistic(line, column);
		int NextPace = MatchMemory();
		if (NextPace != 0)
		{
			abc = false;
			line = NextPace / 10;
			column = NextPace % 10;
			cross[line][column] = PlayerNumber;
		}
	}
	///��������λ����һֱѭ����ֱ����������λ��
	while (abc)
	{
		GetMaxScorePosition();
		line = MaxScorePosition / 10;
		column = MaxScorePosition % 10;
		if (cross[line][column] != 0) continue;//�����Ȼûʲô�ã����������
		cross[line][column] = OT;
		///����λ�ö��ڶԷ���˵�����壬�����ѭ��
		if (DeadCheck(line, column, OT) == true && MaxScore > PointStyle[9])
		{
			Score[line][column] = PointStyle[9];
			cross[line][column] = 0;
			continue;
		}
		cross[line][column] = PlayerNumber;
		///��������λ�ã��������ϻ���λ�ò������壬�����ѭ��
		if (DeadCheck(line, column, PlayerNumber) == true && MaxScore > PointStyle[1])
		{
			Score[line][column] = PointStyle[1];
			cross[line][column] = 0;
			continue;
		}
		else break;
	}
	cross[line][0] = 1;
	Statistic(line, column);
	UpdateScore(line, column, PlayerNumber);
	//	Display(PlayerNumber, line, column);
}
///����̨��ʾ��Ϣ����
void AI1::Display(int n, int line, int column)
{
	/*
	_cprintf("Player %d:  line:%d  column:%d\n\t", n, line, column);
	for (int i = 0; i < 10; )
	{
		for (int j = 1; j < 10; j++)
		{
			if (i == 0) _cprintf("%d\t", j);
			else _cprintf("%.2f\t", Score[i][j]);
			if (j == 9) _cprintf("\n");
		}
		if (++i < 10) _cprintf("%d\t", i);
	}
	_cprintf("\n");
	*/
}
