#pragma once
#include <list>
#include <set>
#include <random>
#include "../ChessBoard_Header/AI.h"
#include "../ChessBoard_Header/Pattern_Moves.h"
#define GetId(x) ((x) < 0 ? 3 + (x) : 3 - (x)) //��ȡ������
#define HaveChess -20 //���ӵ�λ�õķ���
#define MyDeadChess -10 //�ҵ������ķ���
#define RivalDeadChess -5 //�Է������ķ���
#define GetLine(x) ((x) / 100)
#define GetColumn(y) ((y) % 100) 
#define GetPositionCode(x, y) ((x) * 100 + (y))
#define GetBoundScore(z) ((z) * 0.39) //������ʱ��ֵ

class AI4 : public AI
{
public:
	/*
	param[line]:�����λ�á�����
	param[column]:�����λ�á�����
	param[onTurn]:�Է�����ݡ����������
	*/
	void GetPosition(int &line, int &column, int onTurn);//AI����ں���
	/*
	param[moves]:�����λ�õļ���
	param[BoardCross]:����״��
	param[playerId]:AI���
	param[CanSeeWinner]:�Ƿ�ֳ�ʤ��
	return:�Ƿ���Ҫģ������
	*/
	bool getMoves(std::vector<int> &moves, const int BoardCross[][10], int playerId, bool &CanSeeWinner);//��ȡ����λ�õļ���
private:
	int cross[10][10]; //����״��
	int PlayerId; //�����ݡ������1�������2
	int Score[10][10] = { 0 }; //��¼����ÿ��λ�õķ�ֵ
	int Round;//��ǰ�غ���
	int MaxScore;//��߷�
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
	/*
	param[line]:�����λ�á�����
	param[column]:�����λ�á�����
	param[deadPos]:��������
	*/
	int CalDeadPosNumber(int line, int column);//������������λ������
	/*
	param[line]:�����λ��
	param[column]:�����λ��
	return:�����ֵ
	*/
	int DeadPosNumberAddScore(int line, int column);//������������λ�������Է�ֵ��Ӱ��
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
	int GetSurroundNumber(int line, int column, int &NullPosition)
	{
		bool Attack = false;
		if (NullPosition == 0) Attack = true;
		int num = 0;
		//�����Ƿ�����
		if (cross[line - 1][column] == 3 - PlayerId || line == 1) num++;
		else if (cross[line - 1][column] == PlayerId) return -1;
		else if(Attack) NullPosition = GetPositionCode(line - 1, column);
		else NullPosition = GetPositionCode(line, column);
		//�����Ƿ�����
		if (cross[line + 1][column] == 3 - PlayerId || line == 9) num++;
		else if (cross[line + 1][column] == PlayerId) return -1;
		else if (Attack) NullPosition = GetPositionCode(line + 1, column);
		else NullPosition = GetPositionCode(line, column);
		//����Ƿ�����
		if (cross[line][column - 1] == 3 - PlayerId || column == 1) num++;
		else if (cross[line][column - 1] == PlayerId) return -1;
		else if(Attack) NullPosition = GetPositionCode(line, column - 1);
		else NullPosition = GetPositionCode(line, column);
		//�ұ��Ƿ�����
		if (cross[line][column + 1] == 3 - PlayerId || column == 9) num++;
		else if (cross[line][column + 1] == PlayerId) return -1;
		else if (Attack) NullPosition = GetPositionCode(line, column + 1);
		else NullPosition = GetPositionCode(line, column);
		return num;
	}

	void GetNeedScorePosition(std::set<int> &pos)
	{
		double BoundScore = MaxScore > 9 ? 9 : (MaxScore > RivalDeadChess ? 5 : HaveChess);//�ٽ��ֵ
		for (int i = 1; i < 10; ++i)
		{
			for (int j = 1; j < 10; ++j)
			{
				if (Score[i][j] > BoundScore) pos.insert(GetPositionCode(i, j));//¼ȡ�ﵽ������λ��
			}
		}
	}
};