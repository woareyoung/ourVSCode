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
	//获取特殊点链表
	Pattern_Moves PM(playerId);
	std::list<std::pair<int, int>> SP = PM.getMoves(true, BoardCross);
	CanSeeWinner = true;
	//没有匹配到模式
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
				//如果遇到不会死的位置
				if (DeadCheck(i, j, PlayerId, cross) == false)
				{
					SP.push_back(std::make_pair(i * 100 + j, 0));
					CanSeeWinner = false;//设定当前回合不是最终回合
				}
			}
		}
		//如果连不会死的位置都没有了，就随便找个空位下了
		if (SP.empty())
		{
			moves.emplace_back(EmptyPosLine * 100 + EmptyPosColumn);
			if(CanSeeWinner) return false;
		}
	}
	CanSeeWinner = false;
	/*
	//搜索眼的数量
	{
		//计算棋盘上虎口数量
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
						if (DeadCheck(GetLine(pos), GetColumn(pos), playerId, cross) == false)
						{
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
		//如果对方有一个虎口，优先阻止，不管三七二十一
		if (RivalEyeNumber == 1)
		{
			moves.clear();
			moves.emplace_back(temp1);
			return false;
		}
		//如果对方没有虎口，而自己有虎口
		else if (RivalEyeNumber == 0 && MyEyeNumber > 0)
		{
			moves.clear();
			moves.emplace_back(temp2);
			return false;
		}
	}*/
	int temp;
	//遍历链表
	for (auto t : SP)
	{
		if (DeadCheck(GetLine(t.first), GetColumn(t.first), PlayerId, cross)) continue;
		temp = CalDeadPosNumber(GetLine(t.first), GetColumn(t.first));
		if (temp == -1) continue;
		if (temp != 1)
		{
			moves.clear();
			moves.emplace_back(t.first);
			return false;
		}
		else moves.emplace_back(t.first);
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
	if (DoubleNotDeadPos == 0) return 0;//如果只剩下死棋位置，那么就返回
	//获取棋盘上的死棋数
	auto GetDeadNumber = [&](int type, int &MyDeadPosNumber, int &RivalDeadPosNumber) {
		int rival = 3 - type;
		for (int i = 1; i < 10; ++i)
		{
			for (int j = 1; j < 10; ++j)
			{
				if (cross[i][j] != 0) continue;
				MyDead = DeadCheck(i, j, PlayerId, cross);
				RivalDead = DeadCheck(i, j, 3 - PlayerId, cross);
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
	GetDeadNumber(3 - PlayerId, RivalDeadPosNumber2, MyDeadPosNumber2);
	cross[line][column] = 0;
	//如果己方下了该位置，可令对方增加2个以上死棋位置数，则不用犹豫了
	if (RivalDeadPosNumber1 - saveRivalDead > 1) return 2;
	//如果对方下了该位置，可令己方增加2个或以上死棋位置数
	if (MyDeadPosNumber2 - saveMyDead > 1) return 2;
	return 1;
}
