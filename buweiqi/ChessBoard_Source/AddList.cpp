#include "../ChessBoard_Header/ChessBoard.h"
void ChessBoard::AddSituationList()
{
	SITUATION sit;
	CurrentRoundNumber++;
	int maxQuadrant = Qua.GetMaxQuadrant();//��ȡ������ӵ�����
	int i, j;
	//���������ӵ��ǵ�һ����
	if (maxQuadrant == Qua.FirstQuadrant)
	{
		sit.ChessPosition = line * 10 + (10 - column);//��¼��������λ��
		for (i = 1; i < 10; ++i)
			for (j = 9; j > 0; j--)
				sit.BoardStatus += std::to_string(cross[i][j]);
	}
	//���������ӵ��ǵڶ�����
	else if (maxQuadrant == Qua.SecondQuadrant)
	{
		sit.ChessPosition = line * 10 + column;//��¼��������λ��
		for (i = 1; i < 10; ++i)
			for (j = 1; j < 10; ++j)
				sit.BoardStatus += std::to_string(cross[i][j]);
	}
	//���������ӵ��ǵ�������
	else if (maxQuadrant == Qua.ThirdQuadrant)
	{
		sit.ChessPosition = (10 - line) * 10 + column;//��¼��������λ��
		for (i = 9; i > 0; i--)
			for (j = 1; j < 10; ++j)
				sit.BoardStatus += std::to_string(cross[i][j]);
	}
	//���������ӵ��ǵ�������
	else if (maxQuadrant == Qua.ForthQuadrant)
	{
		sit.ChessPosition = (10 - line) * 10 + (10 - column);//��¼��������λ��
		for (i = 9; i > 0; i--)
			for (j = 9; j > 0; j--)
				sit.BoardStatus += std::to_string(cross[i][j]);
	}
	MemBattle.push_back(sit);
	if (line < 5 && column > 5) Qua.FirstQuadrant++;
	else if (line < 5 && column < 5) Qua.SecondQuadrant++;
	else if (line > 5 && column < 5) Qua.ThirdQuadrant++;
	else if (line > 5 && column > 5) Qua.ForthQuadrant++;
}

void ChessBoard::AddList(int type)
{
	PACE pac;
	pac.line = line;
	pac.column = column;
	pac.player = type;
	MemPace.push_back(pac);
	AddSituationList();
}