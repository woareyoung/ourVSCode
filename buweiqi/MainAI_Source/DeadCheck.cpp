#include "../stdafx.h"
#include "../ChessBoard_Header/AI.h"
#include <stack>

///������Ԥ������λ��
bool AI::DeadCheck(int line, int column, int who, int CROSS[][10])
{
	bool simulate = false;//����Ƿ���ģ������
	//Ϊ����Ӧ���̣߳�������һ��������������ʳ�ͻ
	int BoardCross[10][10];
	int i, j;//��������
	int rival = 3 - who;//�Է�
	int saveLine = line, saveColumn = column;
	for (i = 0; i < 10; i++)
		for (j = 0; j < 10; j++)
			BoardCross[i][j] = CROSS[i][j];
	bool VisitStatus[10][10] = { false };//����Ƿ��ѱ�����
	std::stack<std::pair<int, int>> position;
	std::pair<int, int> p;
	std::stack<bool> PosNumber;//���ڼ����������
	//��ʼ�������������
	auto InitVisit = [&]() {
		for (i = 0; i < 10; i++)
			for (j = 0; j < 10; j++)
				VisitStatus[i][j] = false;
	};
	//��������
	auto POPdata = [&]() {
		if (position.empty()) return false;
		p = position.top();
		line = p.first;
		column = p.second;
		position.pop();
		return true;
	};
	//ѹջ�������ñ���
	auto PUSHdata = [&](int l, int c) {
		std::pair<int, int> tempP(line, column);
		position.push(tempP);
		line = l;
		column = c;
	};
	//��ԭ����
	auto BackData = [&]() {
		line = saveLine;
		column = saveColumn;
		while (!position.empty()) position.pop();
		if (simulate) CROSS[line][column] = 0;
	};
	//�ж��Ƿ���ģ������
	if (BoardCross[line][column] == 0)
	{
		BoardCross[line][column] = who;
		simulate = true;
	}
	//���ٱ������ж���û�б�Χ����
	//param[Player]:��� ˭ �����ӱ�Χ��
	auto VisitAll = [&](int Player)
	{
		InitVisit();//��ʼ���������
		int Rival = 3 - Player;
		while (true)
		{
			VisitStatus[line][column] = true;
			//������λ������ȥ��
			if (BoardCross[line][column] == 0 && line > 0 && column > 0)
			{
				BackData();
				return false;
			}
			//����������Լ���������δ������
			if (BoardCross[line - 1][column] == Player && VisitStatus[line - 1][column] == false && line > 1)
			{
				PUSHdata(line - 1, column);
				continue;
			}
			else if (BoardCross[line - 1][column] == 0 && line > 1)
			{
				line = line - 1;
				continue;
			}
			//����������Լ���������δ������
			if (BoardCross[line + 1][column] == Player && VisitStatus[line + 1][column] == false && line < 9)
			{
				PUSHdata(line + 1, column);
				continue;
			}
			else if (BoardCross[line + 1][column] == 0 && line < 9)
			{
				line = line + 1;
				continue;
			}
			//���������Լ���������δ������
			if (BoardCross[line][column - 1] == Player && VisitStatus[line][column - 1] == false && column > 1)
			{
				PUSHdata(line, column - 1);
				continue;
			}
			else if (BoardCross[line][column - 1] == 0 && column > 1)
			{
				column = column - 1;
				continue;
			}
			//����ұ����Լ���������δ������
			if (BoardCross[line][column + 1] == Player && VisitStatus[line][column + 1] == false && column < 9)
			{
				PUSHdata(line, column + 1);
				continue;
			}
			else if (BoardCross[line][column + 1] == 0 && column < 9)
			{
				column = column + 1;
				continue;
			}
			if (POPdata()) continue;
			BackData();
			return true;//�ܵ�����֤������
		}
	};
	if (BoardCross[line - 1][column] == rival && line > 1)
	{
		line = line - 1;
		if (VisitAll(rival)) return true;
	}
	if (BoardCross[line + 1][column] == rival && line < 9)
	{
		line = line + 1;
		if (VisitAll(rival)) return true;
	}
	if (BoardCross[line][column - 1] == rival && column > 1)
	{
		column = column - 1;
		if (VisitAll(rival)) return true;
	}
	if (BoardCross[line][column + 1] == rival && column < 9)
	{
		column = column + 1;
		if (VisitAll(rival)) return true;
	}
	if (VisitAll(who)) return true;
	return false;
}