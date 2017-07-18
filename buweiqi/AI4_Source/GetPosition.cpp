#include "../AI4_Header/AI4.h"

void AI4::GetPosition(int &line, int &column, int onTurn)
{
	if (onTurn != 1 && onTurn != -1 && onTurn != 2 && onTurn != -2)
	{
		//����ĳ�ʼ������
		if (onTurn == 0) Initialize();
		//����һ������
		else cross[line][column] = 0;
	}
	InitScore();
	if (Round == 0) Round = 1;
	else Round++;
	PlayerId = GetId(onTurn);//��ȡ������
	//��ȡ���������
	std::list<std::pair<int, int>> SP = PM.getMoves(Round == 1 ? true : false, cross);
	//���·�ֵ
	UpdateScore(SP);
}