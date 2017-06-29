#include "../AI2_Header/AI2.h"

DIRECTION direction_8[] = { { -1, 0 },{ 0, 1 },{ 1, 0 },{ 0, -1 },{ -1, 1 },{ 1, 1 },{ 1, -1 },{ -1, -1 } };
/**
* [AI2::isGo2Dead �ж����ӵ��Ƿ�����ɱ����]
*/
bool AI2::isGo2Dead(int line, int column, int type)
{
	int player = type;//��¼�����ı�ţ���1����2��
	int rival = getRival(player);//��¼�Է��ı�ţ���1����2��
	auto reduceRecursionTimes = [&]() -> void {
		for (int i = 0; i < 10; ++i)
		{
			if (isGo2DeadStatus[i][0] == true)
				for (int j = 0; j < 10; ++j)
					isGo2DeadStatus[i][j] = false;
		}
	};
	for (int i = 0; i < 4; ++i) Position[i] = false;
	for (int i = 0; i < 4; ++i)
	{
		if (cross[line + direction_8[i].x_offset][column + direction_8[i].y_offset] == rival)
		{
			reduceRecursionTimes();
			if (Besieg(line + direction_8[i].x_offset, column + direction_8[i].y_offset, turn2Who, rival))
				return true;
			Position[i] = true;
		}
		else if (line + direction_8[i].x_offset < 1 || line + direction_8[i].x_offset > 9
			|| column + direction_8[i].y_offset < 1 || column + direction_8[i].y_offset > 9)
		{
			Position[i] = true;
		}
		else if (cross[line + direction_8[i].x_offset][column + direction_8[i].y_offset] == turn2Who)
		{
			reduceRecursionTimes();
			Position[i] = Besieg(line, column, rival, turn2Who);
		}
	}
	return Position[0] && Position[1] && Position[2] && Position[3];
}

bool AI2::Besieg(int RivalLine, int RivalColumn, int player, int rival)
{
	bool tie[4] = { false, false, false, false };
	// ����Cross����״̬
	auto setStatus = [&](int RivalLine, int RivalColumn) -> void {
		isGo2DeadStatus[RivalLine][RivalColumn] = true;
		isGo2DeadStatus[RivalLine][0] = true;//�±�Ϊ0����һ�����Ϊtrue���ʼ���������ó�ʼ��
	};
	setStatus(RivalLine, RivalColumn);

	//���Է����ӵ��Ϸ��жԷ���������δ���ϱ�Ե
	if (cross[RivalLine - 1][RivalColumn] == rival && RivalLine - 1 > 0
		&& isGo2DeadStatus[RivalLine - 1][RivalColumn] == false)
		tie[0] = Besieg(RivalLine - 1, RivalColumn, player, rival);
	//���Է����ӵ��Ϸ�û�����ӣ�ֱ�ӷ���false
	else if (cross[RivalLine - 1][RivalColumn] == NoChess && RivalLine - 1 > 0) return false;
	//���Է����ӵ��Ϸ��м��������ӻ��ѵ���Ե
	else tie[0] = true;
	//���Է����ӵ��·��жԷ���������δ���±�Ե
	if (cross[RivalLine + 1][RivalColumn] == rival && RivalLine + 1 < 10
		&& isGo2DeadStatus[RivalLine + 1][RivalColumn] == false)
		tie[1] = Besieg(RivalLine + 1, RivalColumn, player, rival);
	//���Է����ӵ��·�û������
	else if (cross[RivalLine + 1][RivalColumn] == NoChess && RivalLine + 1 < 10) return false;
	//���Է����ӵ��·��м��������ӻ��ѵ���Ե
	else tie[1] = true;
	//���Է����ӵ��ҷ��жԷ���������δ���ұ�Ե
	if (cross[RivalLine][RivalColumn + 1] == rival && RivalColumn + 1 < 10
		&& isGo2DeadStatus[RivalLine][RivalColumn + 1] == false)
		tie[2] = Besieg(RivalLine, RivalColumn + 1, player, rival);
	//���Է����ӵ��ҷ�û������
	else if (cross[RivalLine][RivalColumn + 1] == NoChess && RivalColumn + 1 < 10) return false;
	//���Է����ӵ��ҷ��м��������ӻ��ѵ���Ե
	else tie[2] = true;
	//���Է����ӵ����жԷ���������δ�����Ե
	if (cross[RivalLine][RivalColumn - 1] == rival && RivalColumn - 1 > 0
		&& isGo2DeadStatus[RivalLine][RivalColumn - 1] == false)
		tie[3] = Besieg(RivalLine, RivalColumn - 1, player, rival);
	//���Է����ӵ���û������
	else if (cross[RivalLine][RivalColumn - 1] == NoChess && RivalColumn - 1 > 0) return false;
	//���Է����ӵ����м��������ӻ��ѵ���Ե
	else tie[3] = true;
	if (tie[0] && tie[1] && tie[2] && tie[3]) return true;
	return false;
}


