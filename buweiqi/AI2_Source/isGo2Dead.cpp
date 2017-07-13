#include "../AI2_Header/AI2.h"
#include <map>
#include <stack>

DIRECTION direction_8[] = { { -1, 0 },{ 0, 1 },{ 1, 0 },{ 0, -1 },{ -1, 1 },{ 1, 1 },{ 1, -1 },{ -1, -1 } };
/**
* [AI2::isGo2Dead �ж����ӵ��Ƿ�����ɱ����]
*/
bool AI2::isGo2Dead(int line, int column, int type)
{
	cross[line][column] = type;
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

	std::function<bool(int, int, int, int)> Besieg;
	Besieg = [&](int RivalLine, int RivalColumn, int player, int rival) {
		bool tie[4] = { false, false, false, false };
		// ����Cross����״̬
		auto setStatus = [&](int RivalLine, int RivalColumn) -> void {
			isGo2DeadStatus[RivalLine][RivalColumn] = true;
			isGo2DeadStatus[RivalLine][0] = true;//�±�Ϊ0����һ�����Ϊtrue���ʼ���������ó�ʼ��
		};
		setStatus(RivalLine, RivalColumn);

		for (int i = 0; i < 4; ++i) {
			//���Է����ӵ��Ϸ��жԷ���������δ���ϱ�Ե
			if (cross[RivalLine + direction_8[i].x_offset][RivalColumn + direction_8[i].y_offset] == rival
				&& OnChessBoard(RivalLine + direction_8[i].x_offset, RivalColumn + direction_8[i].y_offset)
				&& !isGo2DeadStatus[RivalLine + direction_8[i].x_offset][RivalColumn + direction_8[i].y_offset])
				tie[i] = Besieg(RivalLine + direction_8[i].x_offset,
					RivalColumn + direction_8[i].y_offset, player, rival);
			//���Է����ӵ��Ϸ�û�����ӣ�ֱ�ӷ���false
			else if (cross[RivalLine + direction_8[i].x_offset][RivalColumn + direction_8[i].y_offset] == NoChess
				&& OnChessBoard(RivalLine + direction_8[i].x_offset, RivalColumn + direction_8[i].y_offset)) return false;
			//���Է����ӵ��Ϸ��м��������ӻ��ѵ���Ե
			else tie[i] = true;
		}
		return tie[0] && tie[1] && tie[2] && tie[3];
	};
	for (int i = 0; i < 4; ++i)
	{
		if (cross[line + direction_8[i].x_offset][column + direction_8[i].y_offset] == rival) {
			reduceRecursionTimes();
			if (Besieg(line + direction_8[i].x_offset, column + direction_8[i].y_offset, player, rival)) {
				cross[line][column] = NoChess;
				return true;
			}
			Position[i] = true;
		}
		else if (line + direction_8[i].x_offset < 1 || line + direction_8[i].x_offset > 9) {
			Position[i] = Besieg(line, column, rival, player);
		}
	}
	cross[line][column] = NoChess;
	return Position[0] && Position[1] && Position[2] && Position[3];
}

