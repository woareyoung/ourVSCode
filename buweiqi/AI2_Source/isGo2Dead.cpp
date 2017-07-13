#include "../AI2_Header/AI2.h"
#include <stack>

DIRECTION direction_8[] = { { -1, 0 },{ 0, 1 },{ 1, 0 },{ 0, -1 },{ -1, 1 },{ 1, 1 },{ 1, -1 },{ -1, -1 } };
/**
* [AI2::isGo2Dead 判断着子点是否有自杀倾向]
*/
bool AI2::isGo2Dead(int line, int column, int type)
{
	bool simulate = false;//标记是否是模拟下棋
						  //为了适应多线程，另定义一个变量，解决访问冲突
	int BoardCross[10][10];
	int i, j;//遍历变量
	int rival = 3 - type;//对方
	int saveLine = line, saveColumn = column;
	for (i = 0; i < 10; i++)
		for (j = 0; j < 10; j++)
			BoardCross[i][j] = cross[i][j];
	bool VisitStatus[10][10] = { false };//标记是否已遍历过
	std::stack<std::pair<int, int>> position;
	std::pair<int, int> p;
	std::stack<bool> PosNumber;//用于计算遍历数量
	auto InitVisit = [&]() {
		for (i = 0; i < 10; i++)
			for (j = 0; j < 10; j++)
				VisitStatus[i][j] = false;
	};
	//弹出数据
	auto POPdata = [&]() {
		if (position.empty()) return false;
		p = position.top();
		line = p.first;
		column = p.second;
		position.pop();
		return true;
	};
	//压栈，并设置遍历
	auto PUSHdata = [&](int l, int c) {
		std::pair<int, int> tempP(line, column);
		position.push(tempP);
		line = l;
		column = c;
	};
	//还原数据
	auto BackData = [&]() {
		line = saveLine;
		column = saveColumn;
		while (!position.empty()) position.pop();
		if (simulate) cross[line][column] = 0;
	};
	//判断是否是模拟下棋
	if (BoardCross[line][column] == 0)
	{
		BoardCross[line][column] = type;
		simulate = true;
	}
	//跟踪遍历（判断有没有被围死）
	//param[Player]:检查 谁 的棋子被围死
	auto VisitAll = [&](int Player)
	{
		InitVisit();//初始化遍历标记
		int Rival = 3 - Player;
		while (true)
		{
			VisitStatus[line][column] = true;
			//遇到空位，活下去了
			if (BoardCross[line][column] == 0 && line > 0 && column > 0)
			{
				BackData();
				return false;
			}
			//如果上面有自己的棋子且未遍历过
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
			//如果下面有自己的棋子且未遍历过
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
			//如果左边有自己的棋子且未遍历过
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
			//如果右边有自己的棋子且未遍历过
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
			return true;//能到这里证明挂了
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
	if (VisitAll(type)) return true;
	return false;
}
}


