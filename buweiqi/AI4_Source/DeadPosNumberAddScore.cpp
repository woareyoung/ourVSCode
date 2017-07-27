#include "../AI4_Header/AI4.h"

#define FIRSTVALUE 100 //额外价值的倍率
#define SECVALUE 100 

int AI4::DeadPosNumberAddScore(int line, int column)
{
	int temp;
	if(GetSurroundNumber(line, column, temp) == 4) return 0;
	int Crossing[10][10];
	int i, j;
	for (i = 0; i < 10; ++i)
	{
		for (j = 0; j < 10; ++j)
		{
			Crossing[i][j] = cross[i][j];
		}
	}
	if (DeadCheck(line, column, PlayerId, Crossing)) return 0;
	//获取棋盘原有死棋位置数
	int DoubleNotDeadPos = 81;
	int saveMyDead = 0;
	int saveRivalDead = 0;
	bool MyDead = false, RivalDead = false;
	for (int i = 1; i < 10; ++i)
	{
		for (int j = 1; j < 10; ++j)
		{
			if (Crossing[i][j] != 0)
			{
				--DoubleNotDeadPos;
				continue;
			}
			MyDead = DeadCheck(i, j, PlayerId, Crossing);
			RivalDead = DeadCheck(i, j, 3 - PlayerId, Crossing);
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
	if (DoubleNotDeadPos == 0) return 0;
	//获取棋盘上的死棋数
	auto GetDeadNumber = [&](int type, int &MyDeadPosNumber, int &RivalDeadPosNumber) {
		int rival = 3 - type;
		for (int i = 1; i < 10; ++i)
		{
			for (int j = 1; j < 10; ++j)
			{
				if (Crossing[i][j] != 0) continue;
				MyDead = DeadCheck(i, j, type, Crossing);
				RivalDead = DeadCheck(i, j, rival, Crossing);
				if (MyDead) ++MyDeadPosNumber;
				if (RivalDead) ++RivalDeadPosNumber;
			}
		}
	};
	//假设该位置是“我”下的，计算双方的死棋数
	Crossing[line][column] = PlayerId;
	int MyDeadPosNumber1 = 0;//自己的死棋位置数量
	int RivalDeadPosNumber1 = 0;//对方的死棋位置数量
	GetDeadNumber(PlayerId, MyDeadPosNumber1, RivalDeadPosNumber1);
	//假设该位置是对方下的，计算双方的死棋数
	Crossing[line][column] = 3 - PlayerId;
	int MyDeadPosNumber2 = 0;//自己的死棋位置数量
	int RivalDeadPosNumber2 = 0;//对方的死棋位置数量
	GetDeadNumber(3 - PlayerId, RivalDeadPosNumber2, MyDeadPosNumber2);
	Crossing[line][column] = 0;
	int myScoreValue = (MyDeadPosNumber1 - MyDeadPosNumber2) * SECVALUE;
	int rivalScoreValue = (RivalDeadPosNumber1 - RivalDeadPosNumber2) * FIRSTVALUE;
	return myScoreValue + rivalScoreValue;
}