#include "../AI3_Header/AI3.h"

#define MaxNull 2 //�����������ʱ�򣬷���Χ�������

int AI3::GetSurroundChessNumber(int line, int column, int playerNum, int CROSS[][10], std::vector<std::pair<int, int>> &NullPos)
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
		//����������Լ���������δ������
		if (CROSS[line - 1][column] == playerNum && Visit[line - 1][column] == false && line > 1)
		{
			PUSHdata(line - 1, column);
			continue;
		}
		else if (CROSS[line - 1][column] == 0 && line > 1)
		{
			line = line - 1;
			Visit[line][column] = true;
			++number;
			continue;
		}
		//����������Լ���������δ������
		if (CROSS[line + 1][column] == playerNum && Visit[line + 1][column] == false && line < 9)
		{
			PUSHdata(line + 1, column);
			continue;
		}
		else if (CROSS[line + 1][column] == 0 && line < 9)
		{
			line = line + 1;
			Visit[line][column] = true;
			++number;
			continue;
		}
		//���������Լ���������δ������
		if (CROSS[line][column - 1] == playerNum && Visit[line][column - 1] == false && column > 1)
		{
			PUSHdata(line, column - 1);
			continue;
		}
		else if (CROSS[line][column - 1] == 0 && column > 1)
		{
			column = column - 1;
			Visit[line][column] = true;
			++number;
			continue;
		}
		//����ұ����Լ���������δ������
		if (CROSS[line][column + 1] == playerNum && Visit[line][column + 1] == false && column < 9)
		{
			PUSHdata(line, column + 1);
			continue;
		}
		else if (CROSS[line][column + 1] == 0 && column < 9)
		{
			column = column + 1;
			Visit[line][column] = true;
			++number;
			continue;
		}
		if (POPdata()) continue;
		break;
	}
	return number;
}