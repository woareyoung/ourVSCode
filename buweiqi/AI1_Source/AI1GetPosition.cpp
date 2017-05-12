#include "../stdafx.h"
#include "../AI1_Header/AI1.h"
///��ȡ����λ��
void AI1::GetPosition(int &line, int &column, int onTurn)
{
	//������Ӧ�����ڶ�AI�ļ��
	if (onTurn == 0)
	{
		line++;
		column++;
		InitializeD();
		return;
	}
	else if (onTurn == 100 || onTurn == 200)
	{
		if (PlayerNumber == 3 - onTurn / 100)
		{
			STEP *s = SSS;
			SSS = SSS->pre;
			SSS->next = NULL;
			delete s;
			s = NULL;
		}
		UpdateScore(line, column, onTurn / 100, false);
		return;
	}
	bool abc = true;
	OT = (onTurn == 1 || onTurn == -1) ? 1 : 2;
	PlayerNumber = OT == 1 ? 2 : 1;//���ø�AI����ұ��
	if (line != 0 && column != 0)
	{
		CalculatePerf(line, column);//��������
		cross[line][column] = OT;//�ȸ���������Ϣ����
		///��������С��ֵ
		if (line < 3 || line > 7 || column < 3 || column > 7) RateResetScore(0.78);
		else RateResetScore(0.92);
		UpdateScore(line, column, OT, true);
		Display(OT, line, column);
	}
	if (line == 0 || column == 0)
	{
		GetMaxScorePosition();
		line = MaxScorePosition / 10;
		column = MaxScorePosition % 10;
		cross[line][column] = PlayerNumber;
		abc = false;
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
	UpdateScore(line, column, PlayerNumber);
	Display(PlayerNumber, line, column);//������̷�ֵ
}
///����̨��ʾ��Ϣ����
void AI1::Display(int n, int line, int column)
{

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

}
