#include "../stdafx.h"
#include "../AI1_Header/AI1.h"
///��ȡ�ض�λ�õ��ĸ���������������������߽磩
///����who������鿴��������������ұ��
int AI1::GetChessAmount(int row1, int row2, int who, int &num)
{
	int amount = 0;
	if (cross[row1][row2 + 1] == who || row2 + 1 == 10)
	{
		num += 1;//��
		amount++;
	}
	if (cross[row1][row2 - 1] == who || row2 - 1 == 0)
	{
		num += 2;//��
		amount++;
	}
	if (cross[row1 + 1][row2] == who || row1 + 1 == 10)
	{
		num += 4;//��
		amount++;
	}
	if (cross[row1 - 1][row2] == who || row1 - 1 == 0)
	{
		num += 8;//��
		amount++;
	}
	return amount;
}
///��ȡ����ֵ��λ��
void AI1::GetMaxScorePosition()
{
	srand(time(NULL));
	int ran = 0, i, j;
	while (ran == 0 || ran % 10 == 0) ran = rand() % 89 + 11;
	MaxScore = Score[ran / 10][ran % 10];
	MaxScorePosition = ran;
	for (i = 1; i < 10; ++i)
	{
		for (j = 1; j < 10; ++j)
		{
			if (MaxScore < Score[i][j])
			{
				MaxScorePosition = i * 10 + j;
				MaxScore = Score[i][j];
			}
		}
	}
}
///��¼��������
void AI1::MemoryData(int line, int column)
{
	if (line < 5 && column > 5) Qua.FirstQuadrant++;
	else if (line < 5 && column < 5) Qua.SecondQuadrant++;
	else if (line > 5 && column < 5) Qua.ThirdQuadrant++;
	else if (line > 5 && column > 5) Qua.ForthQuadrant++;
}