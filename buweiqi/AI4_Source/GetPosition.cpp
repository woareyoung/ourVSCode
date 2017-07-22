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
	//��ȡ��߷�λ��
	GetMaxScorePosition(line, column);
	cross[line][column] = PlayerId;
	Score[line][column] = HaveChess;
}