#include "../stdafx.h"
#include "../ChessBoard_Header/ChessBoard.h"

/*ʤ������
* 1����ʱ���������ж��ɴ��ڹ��̺���ʵ��
* 2�����֡��Ⱥ���
* 3��Χ��������˼�룺���������һ�����ӣ����жϸú�����Χ���ĸ�λ������û�а��ӱ�Χ�������иú����Ƿ�Χ��
*/
//ע�����øú��������������ң����Ǹ��˻�AI�ǡ�������
bool ChessBoard::WinOrLose()
{
	int i;
	for (i = 0; i < 4; i++) Position[i] = false;
	int player;//��¼�����ı�ţ���1����2��
	int rival;//��¼�Է��ı�ţ���1����2��
	//onTurn��ֵ���ȱ仯���ٵ��øú�����WinOrLose����
	if (onTurn == isPlay1onTurn || onTurn == isAI1onTurn)
	{
		player = isWhite;
		rival = isBlack;
	}
	else if (onTurn == isPlay2onTurn || onTurn == isAI2onTurn)
	{
		player = isBlack;
		rival = isWhite;
	}

	//�ж�����������û������
	//������
	if (Check(line, column, line, column - 1, player, 0))
	{
		Winner = rival;
		return true;
	}
	//����ұ�
	if (Check(line, column, line, column + 1, player, 1))
	{
		Winner = rival;
		return true;
	}
	//����ϱ�
	if (Check(line, column, line - 1, column, player, 2))
	{
		Winner = rival;
		return true;
	}
	//����±�
	if (Check(line, column, line + 1, column, player, 3))
	{
		Winner = rival;
		return true;
	}
	if (Position[0] && Position[1] && Position[2] && Position[3])
	{
		Winner = rival;
		return true;
	}
	return false;
}
///������OriLine  ����λ�õ���
///������OriColumn  ����λ�õ���
///������CLine  ��Ҫ����λ�õ���
///������CColumn   ��Ҫ����λ�õ���
///������Player  �ĸ�����µ�����
///������i  �±꣨Position��
bool ChessBoard::Check(int OriLine, int OriColumn, int CLine, int CColumn, int player, int i)
{
	int rival = player == 1 ? 2 : 1;
	//�����������ѵ���Ե
	if (CColumn == 0 || CColumn == 10 || CLine == 0 || CLine == 10) Position[i] = true;
	//���жԷ���һ������
	else if (cross[CLine][CColumn] == rival)
	{
		reduceRecursionTimes();
		if (Besieged(CLine, CColumn, player, rival))
		{
			Winner = rival;
			showChessBroadInfoOnDOS();
			return true;
		}
		Position[i] = true;
	}
	//���м���������
	else if (cross[CLine][CColumn] == player)
	{
		reduceRecursionTimes();
		Position[i] = Besieged(OriLine, OriColumn, rival, player);
	}
	return false;
}
//�ж϶Է��������Ƿ�Χ��������������ʾ�Է����ӵ�λ��
//�ú�������һ�����ӱ�Χ����������õݹ�ķ������һ�����ӵ����
//�����ӱ�Χ�����򷵻�true��ֻҪ������Χ��һ����λ�ͷ���false
bool ChessBoard::Besieged(int RivalLine, int RivalColumn, int player, int rival)
{
	bool tie[4] = { false, false, false, false };
	setStatus(RivalLine, RivalColumn);//����λ�÷���״̬
									  //���Է����ӵ��Ϸ�
	if (!CheBesie(RivalLine - 1, RivalColumn, player, rival, tie[0])) return false;
	//���Է����ӵ��·�
	if (!CheBesie(RivalLine + 1, RivalColumn, player, rival, tie[1])) return false;
	//���Է����ӵ��ҷ�
	if (!CheBesie(RivalLine, RivalColumn + 1, player, rival, tie[2])) return false;
	//���Է����ӵ���
	if (!CheBesie(RivalLine, RivalColumn - 1, player, rival, tie[3])) return false;
	if (tie[0] && tie[1] && tie[2] && tie[3]) return true;
	return false;
}
bool ChessBoard::CheBesie(int RivalLine, int RivalColumn, int player, int rival, bool &tie)
{
	//���Է����ӵ�ĳ�����жԷ���������δ����Ե
	if (cross[RivalLine][RivalColumn] == rival && RivalLine > 0 && RivalColumn > 0
		&& Cross[RivalLine][RivalColumn] == false && RivalLine < 10 && RivalColumn < 10)
		tie = Besieged(RivalLine, RivalColumn, player, rival);
	//���Է����ӵ�ĳ����û�����ӣ�ֱ�ӷ���false
	else if (cross[RivalLine][RivalColumn] == 0 && RivalLine > 0 && RivalColumn > 0 && RivalLine < 10 && RivalColumn < 10)
		return false;
	//���Է����ӵ�ĳ�����м��������ӻ��ѵ���Ե
	else tie = true;
	return true;
}
