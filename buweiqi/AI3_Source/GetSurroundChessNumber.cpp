#include "../AI3_Header/AI3.h"

int SimulatorGo::GetSurroundChessNumber(int line, int column, int playerNum, int CROSS[][10], std::vector<std::pair<int, int>> &NullPos)
{
	int number = 0;//空位数量
	bool Visit[10][10] = { false };//遍历标识
	int saveLine = line, saveColumn = column;//保存数据
	std::stack<std::pair<int, int>> MyChessPosition;//属于playerNum的棋子
	std::pair<int, int> p;//临时变量
	//弹出数据
	auto POPdata = [&]() {
		if (MyChessPosition.empty()) return false;
		p = MyChessPosition.top();
		line = p.first;
		column = p.second;
		MyChessPosition.pop();
		return true;
	};
	//压栈，并设置遍历
	auto PUSHdata = [&](int l, int c) {
		MyChessPosition.push(std::make_pair(line, column));
		line = l;
		column = c;
	};
	//还原数据
	auto BackData = [&]() {
		line = saveLine;
		column = saveColumn;
		while (!MyChessPosition.empty()) MyChessPosition.pop();
	};
	Visit[line][column] = true;//设置眼中心的遍历标识
	while (true)
	{
		Visit[line][column] = true;
		//如果上面有自己的棋子且未遍历过
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
		//如果下面有自己的棋子且未遍历过
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
		//如果左边有自己的棋子且未遍历过
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
		//如果右边有自己的棋子且未遍历过
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