#include "../AI4_Header/AI4.h"
#include "../ChessBoard_Header/Pattern_Moves.h"

bool AI4::getMoves(std::vector<int> &moves, const int BoardCross[][10], int playerId)
{
	PlayerId = playerId;
	for (int i = 0; i < 10; ++i)
		for (int j = 0; j < 10; ++j)
			cross[i][j] = BoardCross[i][j];
	//获取特殊点链表
	Pattern_Moves PM(playerId);
	std::list<std::pair<int, int>> SP = PM.getMoves(true, BoardCross);
	int temp, DoubleNotDeadPos;//DoubleNotDeadPos记录双方都不会死的位置数量
	for (auto t : SP)
	{
		temp = CalDeadPosNumber(GetLine(t.first), GetColumn(t.first));
		//如果只剩下死棋的时候
		if (temp != 1)
		{
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
	//获取棋盘原有死棋位置数
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
	if (DoubleNotDeadPos == 0) return 0;//如果只剩下死棋位置，那么就返回
	//获取棋盘上的死棋数
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
	//假设该位置是“我”下的，计算双方的死棋数
	cross[line][column] = PlayerId;
	int MyDeadPosNumber1 = 0;//自己的死棋位置数量
	int RivalDeadPosNumber1 = 0;//对方的死棋位置数量
	GetDeadNumber(PlayerId, MyDeadPosNumber1, RivalDeadPosNumber1);
	//假设该位置是对方下的，计算双方的死棋数
	cross[line][column] = 3 - PlayerId;
	int MyDeadPosNumber2 = 0;//自己的死棋位置数量
	int RivalDeadPosNumber2 = 0;//对方的死棋位置数量
	GetDeadNumber(3 - PlayerId, MyDeadPosNumber2, RivalDeadPosNumber2);
	//如果己方下了该位置，可令对方增加2个以上死棋位置数，则不用犹豫了
	if (RivalDeadPosNumber1 - saveRivalDead > 1) return 2;
	//如果己方下了该位置，会令对方增加1个死棋位置；且如果对方下了该位置，会令己方增加1个死棋位置
	if (MyDeadPosNumber2 - MyDeadPosNumber1 > 0 && RivalDeadPosNumber1 - RivalDeadPosNumber2 > 0) return 2;
	return 1;
}