#include "../AI4_Header/AI4.h"
#include "../ChessBoard_Header/Pattern_Moves.h"

bool AI4::getMoves(std::vector<int> &moves, const int BoardCross[][10], int playerId, bool &CanSeeWinner)
{
	PlayerId = playerId;
	for (int i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			if (BoardCross[i][j] == 4)
			{
				cross[i][j] = 0;
				Score[i][j] = 0;
			}
			else
			{
				cross[i][j] = BoardCross[i][j];
				Score[i][j] = HaveChess;
			}
		}
	}
	//��ȡ���������
	Pattern_Moves PM(playerId);
	std::list<std::pair<int, int>> SP = PM.getMoves(true, BoardCross);
	std::set<int> NeedPosition;
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
					NeedPosition.insert(GetPositionCode(i, j));
					CanSeeWinner = false;//�趨��ǰ�غϲ������ջغ�
				}
			}
		}
		//�������������λ�ö�û���ˣ�������Ҹ���λ����
		if (NeedPosition.empty())
		{
			moves.emplace_back(GetPositionCode(EmptyPosLine, EmptyPosColumn));
			if (CanSeeWinner) return false;
		}
		CanSeeWinner = false;
	}
	/*
	//�����۵�����
	{
		//���������ϻ�������
		auto TigerMouthNumberCal = [&](int &EyePosition) {
			int temp;
			int number = 0;
			int pos;
			bool Attack;
			if (EyePosition == 0) Attack = true;
			else Attack = false;
			for (int i = 1; i < 10; ++i)
			{
				for (int j = 1; j < 10; ++j)
				{
					if (cross[i][j] != 0) continue;
					if (Attack) pos = 0;
					else pos = 1;
					temp = GetSurroundNumber(i, j, pos);
					if (temp == 3)
					{
						if (DeadCheck(GetLine(pos), GetColumn(pos), playerId, cross) == false) {
							EyePosition = pos;
							number++;
						}
					}
				}
			}
			return number;
		};
		int MyEyeNumber = 0, RivalEyeNumber = 0;
		int temp1 = 1, temp2 = 0;
		RivalEyeNumber = TigerMouthNumberCal(temp1);
		PlayerId = 3 - PlayerId;
		MyEyeNumber = TigerMouthNumberCal(temp2);
		PlayerId = 3 - PlayerId;
		//����Է���һ�����ڣ�������ֹ���������߶�ʮһ
		if (RivalEyeNumber == 1)
		{
			moves.clear();
			moves.emplace_back(temp1);
			return false;
		}
		//����Է�û�л��ڣ����Լ��л���
		else if (RivalEyeNumber == 0 && MyEyeNumber > 0)
		{
			moves.clear();
			moves.emplace_back(temp2);
			return false;
		}
	}*/
	else
	{
		int line = 0, column = 0;
		std::list<std::pair<int, int>> sp;
		for (auto s : SP)
		{
			if (s.second > 9) sp.push_back(std::make_pair(s.first, s.second));
		}
		if (sp.empty())
		{
			for (auto s : SP)
			{
				if(s.second > 4) sp.push_back(std::make_pair(s.first, s.second));
			}
		}
		if (sp.empty())
		{
			for (auto s : SP)
			{
				if (s.second > RivalDeadChess) sp.push_back(std::make_pair(s.first, s.second));
			}
		}
		UpdateBoardScore(sp);//���·���
		GetMaxScorePosition(line, column);
		MaxScore = Score[line][column];
		GetNeedScorePosition(NeedPosition); 
		for (auto au : NeedPosition)
			moves.emplace_back(au);
		return true;
	}
	int temp;
	//��������
	for (auto t : NeedPosition)
	{
		if (DeadCheck(GetLine(t), GetColumn(t), PlayerId, cross)) continue;
		temp = CalDeadPosNumber(GetLine(t), GetColumn(t));
		if (temp == -1) continue;
		if (temp != 1)
		{
			moves.clear();
			moves.emplace_back(t);
			return false;
		}
		else moves.emplace_back(t);
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
	//����Է����˸�λ�ã����������2������������λ����
	if (MyDeadPosNumber2 - saveMyDead > 1) return 2;
	return 1;
}

