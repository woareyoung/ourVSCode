#include "../AI3_Header/AI3.h"

int SimulatorGo::GetSurroundChessNumber(int line, int column, int playerNum, int CROSS[][10], std::vector<std::pair<int, int>> &NullPos)
{
	int number = 0;//��λ����
	bool Visit[10][10] = { false };//������ʶ
	int saveLine = line, saveColumn = column;//��������
	std::stack<std::pair<int, int>> MyChessPosition;//����playerNum������
	std::pair<int, int> p;//��ʱ����
	//��������
	auto POPdata = [&]() {
		if (MyChessPosition.empty()) return false;
		p = MyChessPosition.top();
		line = p.first;
		column = p.second;
		MyChessPosition.pop();
		return true;
	};
	//ѹջ�������ñ���
	auto PUSHdata = [&](int l, int c) {
		MyChessPosition.push(std::make_pair(line, column));
		line = l;
		column = c;
	};
	//��ԭ����
	auto BackData = [&]() {
		line = saveLine;
		column = saveColumn;
		while (!MyChessPosition.empty()) MyChessPosition.pop();
	};
	Visit[line][column] = true;//���������ĵı�����ʶ
	while (true)
	{
		Visit[line][column] = true;
		//����������Լ���������δ������
		if (CROSS[line - 1][column] == playerNum && Visit[line - 1][column] == false && line > 1)
		{
			PUSHdata(line - 1, column);
			continue;
		}
		else if (CROSS[line - 1][column] == NoChess && Visit[line - 1][column] == false && line > 1)
		{
			NullPos.emplace_back(std::make_pair(line - 1, column));
			Visit[line - 1][column] = true;
			number++;
		}
		//����������Լ���������δ������
		if (CROSS[line + 1][column] == playerNum && Visit[line + 1][column] == false && line < 9)
		{
			PUSHdata(line + 1, column);
			continue;
		}
		else if (CROSS[line + 1][column] == NoChess && Visit[line + 1][column] == false && line < 9)
		{
			NullPos.emplace_back(std::make_pair(line + 1, column));
			Visit[line + 1][column] = true;
			number++;
		}
		//���������Լ���������δ������
		if (CROSS[line][column - 1] == playerNum && Visit[line][column - 1] == false && column > 1)
		{
			PUSHdata(line, column - 1);
			continue;
		}
		else if (CROSS[line][column - 1] == NoChess && Visit[line][column - 1] == false && column > 1)
		{
			NullPos.emplace_back(std::make_pair(line, column - 1));
			Visit[line][column - 1] = true;
			number++;
		}
		//����ұ����Լ���������δ������
		if (CROSS[line][column + 1] == playerNum && Visit[line][column + 1] == false && column < 9)
		{
			PUSHdata(line, column + 1);
			continue;
		}
		else if (CROSS[line][column + 1] == NoChess && Visit[line][column + 1] == false && column < 9)
		{
			NullPos.emplace_back(std::make_pair(line, column + 1));
			Visit[line][column + 1] = true;
			number++;
		}
		if (POPdata()) continue;
		break;
	}
	return number;
}