#pragma once
#include <list>
#include <set>
#include <random>
#include "../ChessBoard_Header/AI.h"
#include "../ChessBoard_Header/Pattern_Moves.h"
#define GetId(x) ((x) < 0 ? 3 + x : 3 - (x)) //��ȡ������
#define HaveChess -50000 //���ӵ�λ�õķ���
#define MyDeadChess -10 //�ҵ������ķ���
#define RivalDeadChess -1

class AI4 : public AI
{
public:
	/*
	param[line]:�����λ�á�����
	param[column]:�����λ�á�����
	param[onTurn]:�Է�����ݡ����������
	*/
	void GetPosition(int &line, int &column, int onTurn);//AI����ں���
private:
	int cross[10][10]; //����״��
	int PlayerId; //�����ݡ������1�������2
	int Round;//�غ���
	int Score[10][10] = { 1 }; //��¼����ÿ��λ�õķ�ֵ
	int MaxScore;
	/*
	param[SpecialPoint]:�������㼰�����������
	*/
	void UpdateBoardScore(std::list<std::pair<int, int>> &SpecialPoint);//��ȡ������б����Ӧ�ķ���
	/*
	param[line]:�ӷֵ����ĵ�λ�á�����
	param[column]:�ӷֵ����ĵ�λ�á�����
	param[score]:���ĵ�ķ���
	param[ScorePoor]:�����֮��ķֲ�
	*/
	template<class T1, class T2>
	void AddScore(int row1, int row2, T1 score, T2 ScorePoor);//��ɢ�ӷ֣����Ը��ֵ�λ�ò�����
	/*
	����line:�����λ�á�����
	����column:�����λ�á�����
	����who:˭ �µ���
	����CROSS:��ǰ����״�������ڽ�����̵߳ĳ�ͻ��
	*/
	bool DeadCheck(int line, int column, int who, int CROSS[][10]);

	/*
	param[MaxLine]:��������λ�á�����
	param[MaxColumn]:��������λ�á�����
	*/
	void GetMaxScorePosition(int &MaxLine, int &MaxColumn)//��ȡ��ֵ����λ��
	{
		std::vector<std::pair<int, int>> MaxPos;
		bool RivalDead = false;
		bool MyDead = false;
		int NoChessPosLine, NoChessPosColumn;
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
	//��ʼ�����飨һ����Ϸһ�Σ�
	void Initialize()
	{
		for (int i = 0; i < 10; ++i)
			for (int j = 0; j < 10; ++j)
				cross[i][j] = 0;
		InitScore();
		Round = 0;
	}
	//��ʼ����ֵ��һ�غ�һ�Σ�
	void InitScore()
	{
		for (int i = 0; i < 10; ++i)
			for (int j = 0; j < 10; ++j)
				if(Score[i][j] > RivalDeadChess) Score[i][j] = 1;//С��1��λ�ò��ǶԷ�����������Լ���������߲��ǿ�λ
	}
	//ִ����ʱ����һ����
	void FirstPaceScore()
	{
		Score[1][1] = 2;
		Score[1][2] = 2;
		Score[2][1] = 2;
		Score[1][8] = 2;
		Score[1][9] = 2;
		Score[2][9] = 2;
		Score[8][1] = 2;
		Score[9][1] = 2;
		Score[9][2] = 2;
		Score[8][9] = 2;
		Score[9][9] = 2;
	}
};