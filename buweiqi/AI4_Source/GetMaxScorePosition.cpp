#include "../AI4_Header/AI4.h"

void AI4::GetMaxScorePosition(int &MaxLine, int &MaxColumn)//��ȡ��ֵ����λ��
{
	std::vector<std::pair<int, int>> MaxPos;
	bool RivalDead = false;
	bool MyDead = false;
	int NoChessPosLine, NoChessPosColumn;
	//�����û����������λ��
	auto WhoseDead = [&](int line, int column) {
		RivalDead = DeadCheck(line, column, 3 - PlayerId, cross);
		MyDead = DeadCheck(line, column, PlayerId, cross);
		if (MyDead) Score[line][column] = MyDeadChess;
		else if (RivalDead) Score[line][column] = RivalDeadChess;
	};
	auto tempMax = HaveChess;//��ʱ���ֵ
	for (int i = 1; i < 10; ++i)
	{
		for (int j = 1; j < 10; ++j)
		{
			//�Կ�λ���м��
			if (cross[i][j] == 0)
			{
				WhoseDead(i, j);
				NoChessPosLine = i;
				NoChessPosColumn = j;
			}
			if (Score[i][j] > tempMax)
			{
				MaxPos.clear();
				tempMax = Score[i][j];
				MaxScore = tempMax;
				MaxPos.emplace_back(i, j);
			}
			else if (Score[i][j] == tempMax)
			{
				MaxPos.emplace_back(i, j);
			}
		}
	}
	if (MaxScore > MyDeadChess)
	{
		std::mt19937_64 random_engine(1000);
		std::uniform_int_distribution<std::size_t> movesDist(0, MaxPos.size() - 1);
		auto move = MaxPos[movesDist(random_engine)];
		MaxLine = move.first;
		MaxColumn = move.second;
	}
	else
	{
		MaxColumn = NoChessPosColumn;
		MaxLine = NoChessPosLine;
	}
}