#include "../ChessBoard_Header/Chess.h"
#include "../ChessBoard_Header/ChessBoard.h"
#include <math.h>

void ChessBoard::AddSituationList()
{
	if (line < 5 && column > 5) FirstQuadrant++;
	else if (line < 5 && column < 5) SecondQuadrant++;
	else if (line > 5 && column < 5) ThirdQuadrant++;
	else if (line > 5 && column > 5) ForthQuadrant++;
	int maxQuadrant = GetMaxQuadrant();//��ȡ������ӵ�����
	int i, j;
	double value;
	///�½����
	if (TempTail == NULL)
	{
		TempTail = new SITUATION;
		TempTail->next = NULL;
		TempTail->prior = NULL;
		SituaHead = TempTail;
	}
	else
	{
		SITUATION *s = new SITUATION;
		TempTail->next = s;
		s->prior = TempTail;
		TempTail = s;
		TempTail->next = NULL;
	}
	TempTail->Line[0] = line * 10 + column;//��¼��������λ��
										   //���������ӵ��ǵ�һ����
	if (maxQuadrant == FirstQuadrant)
	{
		for (i = 1; i < 10; i++)
		{
			value = 0;
			for (j = 9; j > 0; j--)
			{
				if (cross[i][j] == 1) value += pow(2, 10 - j);
				else if (cross[i][j] == 2) value += pow(2, 10 - j) / 10000;
			}
			TempTail->Line[i] = value;
		}
	}
	//���������ӵ��ǵڶ�����
	else if (maxQuadrant == SecondQuadrant)
	{
		for (i = 1; i < 10; i++)
		{
			value = 0;
			for (j = 1; j < 10; j++)
			{
				if (cross[i][j] == 1) value += pow(2, j);
				else if (cross[i][j] == 2) value += pow(2, j) / 10000;
			}
			TempTail->Line[i] = value;
		}
	}
	//���������ӵ��ǵ�������
	else if (maxQuadrant == ThirdQuadrant)
	{
		for (i = 9; i > 0; i--)
		{
			value = 0;
			for (j = 1; j < 10; j++)
			{
				if (cross[i][j] == 1) value += pow(2, j);
				else if (cross[i][j] == 2) value += pow(2, j) / 10000;
			}
			TempTail->Line[i] = value;
		}
	}
	//���������ӵ��ǵ�������
	else if (maxQuadrant == ForthQuadrant)
	{
		for (i = 9; i > 0; i--)
		{
			value = 0;
			for (j = 9; j > 0; j--)
			{
				if (cross[i][j] == 1) value += pow(2, 10 - j);
				else if (cross[i][j] == 2) value += pow(2, 10 - j) / 10000;
			}
			TempTail->Line[i] = value;
		}
	}
}

void ChessBoard::AddList(int type)
{
	//��ӵ���¼
	if (Tail == NULL)
	{
		Tail = new PACE;
		Tail->line = line;
		Tail->column = column;
		Tail->player = type;
		Tail->next = NULL;
		Tail->perior = NULL;
	}
	else
	{
		PACE *p = new PACE;
		p->line = line;
		p->column = column;
		p->player = type;
		p->next = NULL;
		p->perior = Tail;
		Tail->next = p;
		Tail = p;
	}
	AddSituationList();
}
///��ȡ��������������
int Chess::GetMaxQuadrant()
{
	int max = FirstQuadrant;
	if (SecondQuadrant > max) max = SecondQuadrant;
	if (ThirdQuadrant > max) max = ThirdQuadrant;
	if (ForthQuadrant > max) max = ForthQuadrant;
	return max;
}