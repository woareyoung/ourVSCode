#include "../AI2_Header/AI2.h"
#include <map>
#include <stack>

DIRECTION direction_8[] = { { -1, 0 },{ 0, 1 },{ 1, 0 },{ 0, -1 },{ -1, 1 },{ 1, 1 },{ 1, -1 },{ -1, -1 } };
/**
* [AI2::isGo2Dead 判断着子点是否有自杀倾向]
*/
bool AI2::isGo2Dead(const int& line, const int& column, const int& type)
{
	//cross[line][column] = type;
	//int player = type;//记录己方的编号（是1还是2）
	//int rival = getRival(player);//记录对方的编号（是1还是2）
	//auto reduceRecursionTimes = [&]() -> void {
	//	for (int i = 0; i < 10; ++i)
	//	{
	//		if (isGo2DeadStatus[i][0] == true)
	//			for (int j = 0; j < 10; ++j)
	//				isGo2DeadStatus[i][j] = false;
	//	}
	//};
	//for (int i = 0; i < 4; ++i) Position[i] = false;

	//std::function<bool(int, int, int, int)> Besieg;
	//Besieg = [&](const int& RivalLine, const int& RivalColumn, const int& player, const int& rival) {
	//	bool tie[4] = { false, false, false, false };
	//	// 设置Cross数组状态
	//	auto setStatus = [&](int RivalLine, int RivalColumn) -> void {
	//		isGo2DeadStatus[RivalLine][RivalColumn] = true;
	//		isGo2DeadStatus[RivalLine][0] = true;//下标为0的那一行如果为true则初始化，否则不用初始化
	//	};
	//	setStatus(RivalLine, RivalColumn);

	//	for (int i = 0; i < 4; ++i) {
	//		//若对方棋子的上方有对方的棋子且未到上边缘
	//		if (cross[RivalLine + direction_8[i].x_offset][RivalColumn + direction_8[i].y_offset] == rival
	//			&& OnChessBoard(RivalLine + direction_8[i].x_offset, RivalColumn + direction_8[i].y_offset)
	//			&& !isGo2DeadStatus[RivalLine + direction_8[i].x_offset][RivalColumn + direction_8[i].y_offset])
	//			tie[i] = Besieg(RivalLine + direction_8[i].x_offset,
	//				RivalColumn + direction_8[i].y_offset, player, rival);
	//		//若对方棋子的上方没有棋子，直接返回false
	//		else if (cross[RivalLine + direction_8[i].x_offset][RivalColumn + direction_8[i].y_offset] == NoChess
	//			&& OnChessBoard(RivalLine + direction_8[i].x_offset, RivalColumn + direction_8[i].y_offset)) return false;
	//		//若对方棋子的上方有己方的棋子或已到边缘
	//		else tie[i] = true;
	//	}
	//	return tie[0] && tie[1] && tie[2] && tie[3];
	//};
	//for (int i = 0; i < 4; ++i)
	//{
	//	if (cross[line + direction_8[i].x_offset][column + direction_8[i].y_offset] == rival) {
	//		reduceRecursionTimes();
	//		if (Besieg(line + direction_8[i].x_offset, column + direction_8[i].y_offset, player, rival)) {
	//			cross[line][column] = NoChess;
	//			return true;
	//		}
	//		Position[i] = true;
	//	}
	//	else if (line + direction_8[i].x_offset < 1 || line + direction_8[i].x_offset > 9) {
	//		Position[i] = Besieg(line, column, rival, player);
	//	}
	//}
	//cross[line][column] = NoChess;
	//return Position[0] && Position[1] && Position[2] && Position[3];

	bool simulate = false;//标记是否是模拟下棋
	int rival = getRival(type);//对方
	int tempLine = line, tempColumn = column;

	std::stack<std::pair<int, int>> position;
	//弹出数据
	auto POPdata = [&]() {
		if (position.empty()) return false;
		std::tie(tempLine, tempColumn) = position.top();
		position.pop();
		return true;
	};
	//压栈，并设置遍历
	auto PUSHdata = [&](int l, int c) {
		position.push(std::make_pair(tempLine, tempColumn));
		tempLine = l;
		tempColumn = c;
	};
	//还原数据
	auto BackData = [&]() {
		tempLine = line;
		tempColumn = column;
		while (!position.empty()) position.pop();
	};
	//判断是否是模拟下棋
	if (cross[tempLine][tempColumn] == NoChess)
	{
		cross[tempLine][tempColumn] = type;
		simulate = true;
	}
	//跟踪遍历（判断有没有被围死）
	//param[Player]:检查 谁 的棋子被围死
	auto VisitAll = [&](const int& Player)
	{
		resetGo2DeadStatus();
		int Rival = getRival(Player);
		while (true)
		{
			isGo2DeadStatus[tempLine][tempColumn] = true;
			//遇到空位，活下去了
			if (cross[tempLine][tempColumn] == NoChess && OnChessBoard(tempLine, tempColumn))
			{
				BackData();
				return false;
			}
			//如果上面有自己的棋子且未遍历过
			if (cross[tempLine - 1][tempColumn] == Player && !isGo2DeadStatus[tempLine - 1][tempColumn] && tempLine > 1)
			{
				PUSHdata(tempLine - 1, tempColumn);
				continue;
			}
			else if (cross[tempLine - 1][tempColumn] == NoChess && tempLine > 1)
			{
				--tempLine;
				continue;
			}
			//如果下面有自己的棋子且未遍历过
			if (cross[tempLine + 1][tempColumn] == Player && !isGo2DeadStatus[tempLine + 1][tempColumn] && tempLine < 9)
			{
				PUSHdata(tempLine + 1, tempColumn);
				continue;
			}
			else if (cross[tempLine + 1][tempColumn] == NoChess && tempLine < 9)
			{
				++tempLine;
				continue;
			}
			//如果左边有自己的棋子且未遍历过
			if (cross[tempLine][tempColumn - 1] == Player && !isGo2DeadStatus[tempLine][tempColumn - 1] && tempColumn > 1)
			{
				PUSHdata(tempLine, tempColumn - 1);
				continue;
			}
			else if (cross[tempLine][tempColumn - 1] == NoChess && tempColumn > 1)
			{
				--tempColumn;
				continue;
			}
			//如果右边有自己的棋子且未遍历过
			if (cross[tempLine][tempColumn + 1] == Player && !isGo2DeadStatus[tempLine][tempColumn + 1] && tempColumn < 9)
			{
				PUSHdata(tempLine, tempColumn + 1);
				continue;
			}
			else if (cross[tempLine][tempColumn + 1] == NoChess && tempColumn < 9)
			{
				++tempColumn;
				continue;
			}
			if (POPdata()) continue;
			BackData();
			return true;//能到这里证明挂了
		}
	};
	if (cross[tempLine - 1][tempColumn] == rival && tempLine > 1)
	{
		--tempLine;
		if (VisitAll(rival))
		{
			if (simulate) cross[line][column] = NoChess;
			return true;
		}
	}
	if (cross[tempLine + 1][tempColumn] == rival && tempLine < 9)
	{
		++tempLine;
		if (VisitAll(rival))
		{
			if (simulate) cross[line][column] = NoChess;
			return true;
		}
	}
	if (cross[tempLine][tempColumn - 1] == rival && tempColumn > 1)
	{
		--tempColumn;
		if (VisitAll(rival))
		{
			if (simulate) cross[line][column] = NoChess;
			return true;
		}
	}
	if (cross[tempLine][tempColumn + 1] == rival && tempColumn < 9)
	{
		++tempColumn;
		if (VisitAll(rival))
		{
			if (simulate) cross[line][column] = NoChess;
			return true;
		}
	}
	if (VisitAll(type))
	{
		if (simulate) cross[line][column] = NoChess;
		return true;
	}
	if (simulate) cross[line][column] = NoChess;
	return false;
}

