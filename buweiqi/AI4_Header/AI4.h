#pragma once
#include <list>
#include <set>
#include <random>
#include "../ChessBoard_Header/AI.h"
#include "../ChessBoard_Header/Pattern_Moves.h"
#define GetId(x) ((x) < 0 ? (-x) : (x)) //��ȡ������
#define HaveChess -50000 //���ӵ�λ�õķ���
#define DeadChess -10 //�����ķ���

class AI4 : public AI
{
	Pattern_Moves PM;
	int cross[10][10]; //����״��
	int PlayerId; //�����ݡ������1�������2
	int Round;//�غ���
	double Score[10][10]; //��¼����ÿ��λ�õķ�ֵ

	/*
	param[line]:�����λ�á�����
	param[column]:�����λ�á�����
	param[onTurn]:�Է�����ݡ����������
	*/
	void GetPosition(int &line, int &column, int onTurn);//AI����ں���
	/*
	param[SpecialPoint]:�������㼰�����������
	*/
	void UpdateScore(std::list<std::pair<int, int>> &SpecialPoint);//��ȡ������б����Ӧ�ķ���
	/*
	param[line]:�ӷֵ����ĵ�λ�á�����
	param[column]:�ӷֵ����ĵ�λ�á�����
	param[score]:���ĵ�ķ���
	param[ScorePoor]:�����֮��ķֲ�
	*/
	template<class T1, class T2>
	void AddScore(int row1, int row2, T1 score, T2 ScorePoor);//��ɢ�ӷ֣����Ը��ֵ�λ�ò�����

	/*
	param[MaxLine]:��������λ�á�����
	param[MaxColumn]:��������λ�á�����
	*/
	void GetMaxScorePosition(int &MaxLine, int &MaxColumn)//��ȡ��ֵ����λ��
	{
		std::vector<std::pair<int, int>> MaxPos;
		auto tempMax = -100.0;//��ʱ���ֵ
		for (int i = 1; i < 10; ++i)
		{
			for (int j = 1; j < 10; ++j)
			{
				if (Score[i][j] > tempMax)
				{
					MaxPos.clear();
					tempMax = Score[i][j];
				}
				else if (Score[i][j] == tempMax)
				{
					MaxPos.emplace_back(std::make_pair(i, j));
				}
			}
		}
		std::mt19937_64 random_engine(1000);
		std::uniform_int_distribution<std::size_t> movesDist(0, MaxPos.size() - 1);
		auto move = MaxPos[movesDist(random_engine)];
		MaxLine = move.first;
		MaxColumn = move.second;
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
				if(Score[i][j] > 0) Score[i][j] = 1;//С��1��λ�ò��ǶԷ�����������Լ���������߲��ǿ�λ
	}
};