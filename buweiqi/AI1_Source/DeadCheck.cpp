#include "../stdafx.h"
#include "../AI1_Header/AI1.h"

///������Ԥ������λ��
bool AI1::DeadCheck(int line, int column, int who)
{
	bool Position[4] = { false, false, false, false };
	int player = who;//��¼�����ı�ţ���1����2��
	//�ж�����������û������
	//������
	if (CheckPosition(line, column, line, column - 1, player, Position[0])) return true;
	//����ұ�
	if (CheckPosition(line, column, line, column + 1, player, Position[1]))  return true;
	//����ϱ�
	if (CheckPosition(line, column, line - 1, column, player, Position[2])) return true;
	//����±�
	if (CheckPosition(line, column, line + 1, column, player, Position[3])) return true;
	if (Position[0] && Position[1] && Position[2] && Position[3]) return true;
	return false;
}
bool AI1::CheckPosition(int OriLine, int OriColumn, int CLine, int CColumn, int player, bool &Position)
{
	int rival = player == 1 ? 2 : 1;
	auto reduceRecursionTimes = [&]() -> void {
		for (int i = 0; i < 10; ++i)
		{
			if (Cross[i][0] == true)
				for (int j = 0; j < 10; ++j)
					Cross[i][j] = false;
		}
	};
	//�����������ѵ���Ե
	if (CColumn == 0 || CColumn == 10 || CLine == 0 || CLine == 10) Position = true;
	//���жԷ���һ������
	else if (cross[CLine][CColumn] == rival)
	{
		reduceRecursionTimes();
		if (Besieg(CLine, CColumn, player, rival))
			return true;
		Position = true;
	}
	//���м���������
	else if (cross[CLine][CColumn] == player)
	{
		reduceRecursionTimes();
		Position = Besieg(OriLine, OriColumn, rival, player);
	}
	return false;
}
bool AI1::Besieg(int RivalLine, int RivalColumn, int player, int rival)
{
	bool tie[4] = { false, false, false, false };
	Cross[RivalLine][RivalColumn] = true;
	Cross[RivalLine][0] = true;//�±�Ϊ0����һ�����Ϊtrue���ʼ���������ó�ʼ��
	//���Է����ӵ��Ϸ�
	if (!BesiegePosition(RivalLine - 1, RivalColumn, player, rival, tie[0])) return false;
	//���Է����ӵ��·�
	if (!BesiegePosition(RivalLine + 1, RivalColumn, player, rival, tie[1])) return false;
	//���Է����ӵ��ҷ�
	if (!BesiegePosition(RivalLine, RivalColumn + 1, player, rival, tie[2])) return false;
	//���Է����ӵ���
	if (!BesiegePosition(RivalLine, RivalColumn - 1, player, rival, tie[3])) return false;
	if (tie[0] && tie[1] && tie[2] && tie[3]) return true;
	return false;
}
bool AI1::BesiegePosition(int RivalLine, int RivalColumn, int player, int rival, bool &tie)
{
	//���Է����ӵ�ĳ�����жԷ���������δ����Ե
	if (cross[RivalLine][RivalColumn] == rival && RivalLine > 0 && RivalColumn > 0
		&& Cross[RivalLine][RivalColumn] == false && RivalLine < 10 && RivalColumn < 10)
		tie = Besieg(RivalLine, RivalColumn, player, rival);
	//���Է����ӵ�ĳ����û�����ӣ�ֱ�ӷ���false
	else if (cross[RivalLine][RivalColumn] == 0 && RivalLine > 0 && RivalColumn > 0 && RivalLine < 10 && RivalColumn < 10)
		return false;
	//���Է����ӵ�ĳ�����м��������ӻ��ѵ���Ե
	else tie = true;
	return true;
}
