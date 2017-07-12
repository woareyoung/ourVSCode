#include "../ChessBoard_Header/Chess.h"
#include "../ChessBoard_Header/ChessBoard.h"
#include <memory>
#include <iostream>

void ChessBoard::AddSituationList()
{
	///�½����
	if (SituaHead == nullptr)
	{
		TempTail = std::shared_ptr<SITUATION>(new SITUATION);
		TempTail->next = nullptr;
		TempTail->prior = nullptr;
		SituaHead = TempTail;
	}
	else
	{
		std::shared_ptr<SITUATION> s = std::shared_ptr<SITUATION>(new SITUATION);
		TempTail->next = s;
		s->prior = TempTail;
		TempTail = s;
		TempTail->next = nullptr;
	}
	CurrentRoundNumber++;
	int maxQuadrant = Qua.GetMaxQuadrant();//��ȡ������ӵ�����
	int i, j;
	//���������ӵ��ǵ�һ����
	if (maxQuadrant == Qua.FirstQuadrant)
	{
		TempTail->ChessPosition = line * 10 + (10 - column);//��¼��������λ��
		for (i = 1; i < 10; ++i)
			for (j = 9; j > 0; j--)
				TempTail->BoardStatus += std::to_string(cross[i][j]);
	}
	//���������ӵ��ǵڶ�����
	else if (maxQuadrant == Qua.SecondQuadrant)
	{
		TempTail->ChessPosition = line * 10 + column;//��¼��������λ��
		for (i = 1; i < 10; ++i)
			for (j = 1; j < 10; ++j)
				TempTail->BoardStatus += std::to_string(cross[i][j]);
	}
	//���������ӵ��ǵ�������
	else if (maxQuadrant == Qua.ThirdQuadrant)
	{
		TempTail->ChessPosition = (10 - line) * 10 + column;//��¼��������λ��
		for (i = 9; i > 0; i--)
			for (j = 1; j < 10; ++j)
				TempTail->BoardStatus += std::to_string(cross[i][j]);
	}
	//���������ӵ��ǵ�������
	else if (maxQuadrant == Qua.ForthQuadrant)
	{
		TempTail->ChessPosition = (10 - line) * 10 + (10 - column);//��¼��������λ��
		for (i = 9; i > 0; i--)
			for (j = 9; j > 0; j--)
				TempTail->BoardStatus += std::to_string(cross[i][j]);
	}
	if (line < 5 && column > 5) Qua.FirstQuadrant++;
	else if (line < 5 && column < 5) Qua.SecondQuadrant++;
	else if (line > 5 && column < 5) Qua.ThirdQuadrant++;
	else if (line > 5 && column > 5) Qua.ForthQuadrant++;
}

void ChessBoard::AddList(int type)
{
	//��ӵ���¼
	if (Tail == nullptr)
	{
		Tail = std::shared_ptr<PACE>(new PACE);
		Tail->line = line;
		Tail->column = column;
		Tail->player = type;
		Tail->next = nullptr;
		Tail->perior = nullptr;
	}
	else
	{
		std::shared_ptr<PACE> p = std::shared_ptr<PACE>(new PACE);
		p->line = line;
		p->column = column;
		p->player = type;
		p->next = nullptr;
		p->perior = Tail;
		Tail->next = p;
		Tail = p;
	}
	AddSituationList();
}