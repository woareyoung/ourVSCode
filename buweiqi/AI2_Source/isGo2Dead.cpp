#include "../AI2_Header/AI2.h"
#include <map>
#include <stack>

DIRECTION direction_8[] = { { -1, 0 },{ 0, 1 },{ 1, 0 },{ 0, -1 },{ -1, 1 },{ 1, 1 },{ 1, -1 },{ -1, -1 } };
/**
* [AI2::isGo2Dead �ж����ӵ��Ƿ�����ɱ����]
*/
bool AI2::isGo2Dead(const int& line, const int& column, const int& type)
{
	//cross[line][column] = type;
	//int player = type;//��¼�����ı�ţ���1����2��
	//int rival = getRival(player);//��¼�Է��ı�ţ���1����2��
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
	//	// ����Cross����״̬
	//	auto setStatus = [&](int RivalLine, int RivalColumn) -> void {
	//		isGo2DeadStatus[RivalLine][RivalColumn] = true;
	//		isGo2DeadStatus[RivalLine][0] = true;//�±�Ϊ0����һ�����Ϊtrue���ʼ���������ó�ʼ��
	//	};
	//	setStatus(RivalLine, RivalColumn);

	//	for (int i = 0; i < 4; ++i) {
	//		//���Է����ӵ��Ϸ��жԷ���������δ���ϱ�Ե
	//		if (cross[RivalLine + direction_8[i].x_offset][RivalColumn + direction_8[i].y_offset] == rival
	//			&& OnChessBoard(RivalLine + direction_8[i].x_offset, RivalColumn + direction_8[i].y_offset)
	//			&& !isGo2DeadStatus[RivalLine + direction_8[i].x_offset][RivalColumn + direction_8[i].y_offset])
	//			tie[i] = Besieg(RivalLine + direction_8[i].x_offset,
	//				RivalColumn + direction_8[i].y_offset, player, rival);
	//		//���Է����ӵ��Ϸ�û�����ӣ�ֱ�ӷ���false
	//		else if (cross[RivalLine + direction_8[i].x_offset][RivalColumn + direction_8[i].y_offset] == NoChess
	//			&& OnChessBoard(RivalLine + direction_8[i].x_offset, RivalColumn + direction_8[i].y_offset)) return false;
	//		//���Է����ӵ��Ϸ��м��������ӻ��ѵ���Ե
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

	bool simulate = false;//����Ƿ���ģ������
	int rival = getRival(type);//�Է�
	int tempLine = line, tempColumn = column;

	std::stack<std::pair<int, int>> position;
	//��������
	auto POPdata = [&]() {
		if (position.empty()) return false;
		std::tie(tempLine, tempColumn) = position.top();
		position.pop();
		return true;
	};
	//ѹջ�������ñ���
	auto PUSHdata = [&](int l, int c) {
		position.push(std::make_pair(tempLine, tempColumn));
		tempLine = l;
		tempColumn = c;
	};
	//��ԭ����
	auto BackData = [&]() {
		tempLine = line;
		tempColumn = column;
		while (!position.empty()) position.pop();
	};
	//�ж��Ƿ���ģ������
	if (cross[tempLine][tempColumn] == NoChess)
	{
		cross[tempLine][tempColumn] = type;
		simulate = true;
	}
	//���ٱ������ж���û�б�Χ����
	//param[Player]:��� ˭ �����ӱ�Χ��
	auto VisitAll = [&](const int& Player)
	{
		resetGo2DeadStatus();
		int Rival = getRival(Player);
		while (true)
		{
			isGo2DeadStatus[tempLine][tempColumn] = true;
			//������λ������ȥ��
			if (cross[tempLine][tempColumn] == NoChess && OnChessBoard(tempLine, tempColumn))
			{
				BackData();
				return false;
			}
			//����������Լ���������δ������
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
			//����������Լ���������δ������
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
			//���������Լ���������δ������
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
			//����ұ����Լ���������δ������
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
			return true;//�ܵ�����֤������
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

