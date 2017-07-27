#include "../AI4_Header/AI4.h"
#include "../ChessBoard_Header/Pattern_Moves.h"

void AI4::GetPosition(int &line, int &column, int onTurn)
{
	if (onTurn != 1 && onTurn != -1 && onTurn != 2 && onTurn != -2)
	{
		//����ĳ�ʼ������
		if (onTurn == 0) Initialize();
		//����һ������
		else cross[line][column] = 0;
		return;
	}
	InitScore();
	if (Round == 0)
	{
		Round = 1;
		FirstPaceScore();
	}
	else Round++;
	PlayerId = GetId(onTurn);//��ȡ������
	//�ж��ǲ��ǵ�һ��
	if (line != 0 && column != 0)
	{
		cross[line][column] = 3 - PlayerId;
		Score[line][column] = HaveChess;
	}
	//��ȡ���������
	Pattern_Moves PM(PlayerId);
	std::list<std::pair<int, int>> SP = PM.getMoves(true, cross);
	//���·�ֵ
	UpdateBoardScore(SP);
	for (int i = 1; i < 10; ++i)
	{
		for (int j = 1; j < 10; ++j)
		{
			if (cross[i][j] != 0) continue;
			int save = Score[i][j];
			Score[i][j] += DeadPosNumberAddScore(i, j);
			if (Score[i][j] < 0)
			{
				if (save < 0) Score[i][j] = save;
				else Score[i][j] = 0;
			}
		}
	}
	//��ȡ��߷�λ��
	GetMaxScorePosition(line, column);
	cross[line][column] = PlayerId;
	Score[line][column] = HaveChess;
}