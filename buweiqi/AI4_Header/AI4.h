#pragma once
#include <list>
#include <set>
#include <random>
#include "../ChessBoard_Header/AI.h"
#define GetId(x) ((x) < 0 ? 3 + x : 3 - (x)) //��ȡ������
#define HaveChess -20 //���ӵ�λ�õķ���
#define MyDeadChess -10 //�ҵ������ķ���
#define RivalDeadChess -5 //�Է������ķ���
#define GetLine(x) (x / 100)
#define GetColumn(y) (y % 100)

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
	int Score[10][10] = { 0 }; //��¼����ÿ��λ�õķ�ֵ
	int level[10][10];//��¼ÿ��λ�õļ�������5���������ӵ�λ����0������ֵ�ֱ���6��4��3��2��1��0��
	int MaxScore;
	std::set<std::pair<int, int>> ForeFiveLevel;//ǰ�弶���λ��
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
	void GetMaxScorePosition(int &MaxLine, int &MaxColumn);//��ȡ��ֵ����λ��
	//��ʼ�����飨һ����Ϸһ�Σ�
	void Initialize()
	{
		for (int i = 0; i < 10; ++i)
			for (int j = 0; j < 10; ++j)
			{
				cross[i][j] = 0;
				Score[i][j] = 0;
			}
		InitScore();
		Round = 0;
	}
	//��ʼ����ֵ��һ�غ�һ�Σ�
	void InitScore()
	{
		for (int i = 0; i < 10; ++i)
			for (int j = 0; j < 10; ++j)
				if(Score[i][j] > RivalDeadChess) Score[i][j] = 0;//С��1��λ�ò��ǶԷ�����������Լ���������߲��ǿ�λ
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