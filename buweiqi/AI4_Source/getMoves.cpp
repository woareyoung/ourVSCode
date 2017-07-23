#include "../AI4_Header/AI4.h"
#include "../ChessBoard_Header/Pattern_Moves.h"

bool AI4::getMoves(std::vector<int> &moves, const int BoardCross[][10], int playerId, bool &CanSeeWinner)
{
	PlayerId = playerId;
	for (int i = 0; i < 10; ++i)
		for (int j = 0; j < 10; ++j)
			cross[i][j] = BoardCross[i][j];
	//��ȡ���������
	Pattern_Moves PM(playerId);
	std::list<std::pair<int, int>> SP = PM.getMoves(true, BoardCross);
	CanSeeWinner = true;
	//û��ƥ�䵽ģʽ
	if (SP.empty())
	{
		int EmptyPosLine, EmptyPosColumn;
		for (int i = 1; i < 10; ++i)
		{
			for (int j = 1; j < 10; ++j)
			{
				if (cross[i][j] != 0) continue;
				EmptyPosLine = i;
				EmptyPosColumn = j;
				//���������������λ��
				if (DeadCheck(i, j, PlayerId, cross) == false)
				{
					SP.push_back(std::make_pair(i * 100 + j, 0));
					CanSeeWinner = false;//�趨��ǰ�غϲ������ջغ�
				}
			}
		}
		//�������������λ�ö�û���ˣ�������Ҹ���λ����
		if (SP.empty())
		{
			moves.push_back(EmptyPosLine * 100 + EmptyPosColumn);
			if(CanSeeWinner) return false;
		}
	}
	CanSeeWinner = false;
	//�����۵�����
	{
		int EyeNumber = 0;
		int temp;
		int EyeLine, EyeColumn;
		for (int i = 1; i < 10; ++i)
		{
			for (int j = 1; j < 10; ++j)
			{
				temp = GetSurroundNumber(i, j);
				if (temp == 3)
				{
					EyeNumber++;
					EyeLine = i;
					EyeColumn = j;
				}
			}
		}
		if (EyeNumber > 0)
		{
			moves.push_back(EyeLine * 100 + EyeColumn);
			return false;
		}
	}
	//��������
	for (auto t : SP)
	{
		if (CalDeadPosNumber(GetLine(t.first), GetColumn(t.first)) != 1)
		{
			moves.clear();
			moves.push_back(t.first);
			return false;
		}
		moves.push_back(t.first);
	}
	return true;
}

int AI4::CalDeadPosNumber(int line, int column)
{
	int DoubleNotDeadPos = 81;
	int saveMyDead = 0;
	int saveRivalDead = 0;
	bool MyDead = false, RivalDead = false;
	//��ȡ����ԭ������λ����
	for (int i = 1; i < 10; ++i)
	{
		for (int j = 1; j < 10; ++j)
		{
			if (cross[i][j] != 0) continue;
			MyDead = DeadCheck(i, j, PlayerId, cross);
			RivalDead = DeadCheck(i, j, 3 - PlayerId, cross);
			if (MyDead)
			{
				++saveMyDead;
				--DoubleNotDeadPos;
			}
			if (RivalDead)
			{
				++saveRivalDead;
				--DoubleNotDeadPos;
			}
		}
	}
	if (DoubleNotDeadPos == 0) return 0;//���ֻʣ������λ�ã���ô�ͷ���
	//��ȡ�����ϵ�������
	auto GetDeadNumber = [&](int type, int &MyDeadPosNumber, int &RivalDeadPosNumber) {
		int rival = 3 - type;
		for (int i = 1; i < 10; ++i)
		{
			for (int j = 1; j < 10; ++j)
			{
				if (cross[i][j] != 0) continue;
				MyDead = DeadCheck(i, j, type, cross);
				RivalDead = DeadCheck(i, j, rival, cross);
				if (MyDead) ++MyDeadPosNumber;
				if (RivalDead) ++RivalDeadPosNumber;
			}
		}
	};
	//�����λ���ǡ��ҡ��µģ�����˫����������
	cross[line][column] = PlayerId;
	int MyDeadPosNumber1 = 0;//�Լ�������λ������
	int RivalDeadPosNumber1 = 0;//�Է�������λ������
	GetDeadNumber(PlayerId, MyDeadPosNumber1, RivalDeadPosNumber1);
	//�����λ���ǶԷ��µģ�����˫����������
	cross[line][column] = 3 - PlayerId;
	int MyDeadPosNumber2 = 0;//�Լ�������λ������
	int RivalDeadPosNumber2 = 0;//�Է�������λ������
	GetDeadNumber(3 - PlayerId, MyDeadPosNumber2, RivalDeadPosNumber2);
	//����������˸�λ�ã�����Է�����2����������λ������������ԥ��
	if (RivalDeadPosNumber1 - saveRivalDead > 1) return 2;
	//����������˸�λ�ã�����Է�����1������λ�ã�������Է����˸�λ�ã����������1������λ��
	if (MyDeadPosNumber2 - MyDeadPosNumber1 > 0 && RivalDeadPosNumber1 - RivalDeadPosNumber2 > 0) return 2;
	return 1;
}