#include "../AI4_Header/AI4.h"
#include "../ChessBoard_Header/Pattern_Moves.h"

bool AI4::getMoves(std::vector<int> &moves, const int BoardCross[][10], int playerId, bool &CanSeeWinner)
{
	PlayerId = playerId;
	for (int i = 0; i < 10; ++i)
		for (int j = 0; j < 10; ++j)
		{
			if (BoardCross[i][j] == 4) cross[i][j] = 0;
			else cross[i][j] = BoardCross[i][j];
		}
	//��ȡ���������
	Pattern_Moves PM(playerId);
	std::list<std::pair<int, int>> SP = PM.getMoves(true, BoardCross);
	CanSeeWinner = true;
	//û��ƥ�䵽ģʽ
	if (SP.empty())
	{
		CanSeeWinner = true;
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
			moves.emplace_back(EmptyPosLine * 100 + EmptyPosColumn);
			if(CanSeeWinner) return false;
		}
	}
	CanSeeWinner = false;
	//�����۵�����
	{
		//���������ϻ�������
		auto TigerMouthNumberCal = [&](int &EyeLine, int &EyeColumn) {
			int temp;
			int number = 0;
			for (int i = 1; i < 10; ++i)
			{
				for (int j = 1; j < 10; ++j)
				{
					if (cross[i][j] != 0) continue;
					temp = GetSurroundNumber(i, j);
					if (temp == 3)
					{
						number++;
						EyeLine = i;
						EyeColumn = j;
					}
				}
			}
			return number;
		};
		int MyEyeNumber = 0, RivalEyeNumber = 0;
		int tempL1, tempC1, tempL2, tempC2;
		RivalEyeNumber = TigerMouthNumberCal(tempL1, tempC1);
		PlayerId = 3 - PlayerId;
		MyEyeNumber = TigerMouthNumberCal(tempL2, tempC2);
		PlayerId = 3 - PlayerId;
		//����Է���һ�����ڣ�������ֹ���������߶�ʮһ
		if (RivalEyeNumber == 1)
		{
			moves.clear();
			moves.emplace_back(tempL1 * 100 + tempC1);
			return false;
		}
		//����Է�û�л��ڣ����Լ��л���
		else if (RivalEyeNumber == 0 && MyEyeNumber > 0)
		{
			moves.clear();
			moves.emplace_back(tempL2 * 100 + tempC2);
			return false;
		}
	}
	//��������
	for (auto t : SP)
	{
		if (DeadCheck(GetLine(t.first), GetColumn(t.first), PlayerId, cross)) continue;
		if (CalDeadPosNumber(GetLine(t.first), GetColumn(t.first)) != 1)
		{
			moves.clear();
			moves.emplace_back(t.first);
			return false;
		}
		moves.emplace_back(t.first);
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
			if (cross[i][j] != 0)
			{
				--DoubleNotDeadPos;
				continue;
			}
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
	GetDeadNumber(3 - PlayerId, RivalDeadPosNumber2, MyDeadPosNumber2);
	cross[line][column] = 0;
	//����������˸�λ�ã�����Է�����2����������λ������������ԥ��
	if (RivalDeadPosNumber1 - saveRivalDead > 1) return 2;
	//����������˸�λ�ã�����Է�����1������λ�ã�������Է����˸�λ�ã����������1������λ��
	if (MyDeadPosNumber2 - MyDeadPosNumber1 > 0 && RivalDeadPosNumber1 - RivalDeadPosNumber2 > 0) return 2;
	return 1;
}