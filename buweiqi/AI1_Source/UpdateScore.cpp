#include "../stdafx.h"
#include "../AI1_Header/AI1.h"
///��������λ�÷�ֵ���������У��У��������ң��Ƿ��Ǽӷ�
void AI1::UpdateScore(int row1, int row2, int who, bool isAddScore)
{
	Score[row1][row2] = PointStyle[10];//���������Ϊ-5��
	if (row1 != 0 && row2 > 1 && cross[row1][row2 - 1] == 0) RecordSpecialPoint(row1, row2 - 1, who, isAddScore);//�鿴���������λ���ϵ������
	if (row1 != 0 && row2 < 9 && cross[row1][row2 + 1] == 0) RecordSpecialPoint(row1, row2 + 1, who, isAddScore);//�鿴�������ұ�λ���ϵ������
	if (row2 != 0 && row1 > 1 && cross[row1 - 1][row2] == 0) RecordSpecialPoint(row1 - 1, row2, who, isAddScore);//�鿴�������ϱ�λ���ϵ������
	if (row2 != 0 && row1 < 9 && cross[row1 + 1][row2] == 0) RecordSpecialPoint(row1 + 1, row2, who, isAddScore);//�鿴�������±�λ���ϵ������
}
///��������
void AI1::RecordSpecialPoint(int row1, int row2, int who, bool isAddScore)
{
	int n = row1 * 10 + row2, sc = 0;
	GetChessAmount(row1, row2, who, sc);//�鿴�ض�λ����Χ�ж��ٸ����Լ���������
	switch (sc)
	{
		//ֻ���ұ������ӣ�ǰ��������������
	case 1:
		if (who == PlayerId) SetCurrentPoint(5, MyFormatChipPoor, 15, MySinglePointPoor);
		else SetCurrentPoint(8, RivalFormatChipPoor, 8, RivalSinglePointPoor);
		AddRecord(n, n + 10, n - 10, n - 1, 0, 5, isAddScore);
		break;
		//ֻ����������ӣ�ǰ��������������
	case 2:
		if (who == PlayerId) SetCurrentPoint(5, MyFormatChipPoor, 15, MySinglePointPoor);
		else SetCurrentPoint(8, RivalFormatChipPoor, 8, RivalSinglePointPoor);
		AddRecord(n, n + 10, n + 1, n - 10, 0, 5, isAddScore);
		break;
		//���ұ߶�������
	case 3:
		if (who == PlayerId) SetCurrentPoint(3, MyFormatTigerMouthPoor, 16, MyChipPoor, 5, MyFormatChipPoor, 15, MySinglePointPoor);
		else SetCurrentPoint(6, RivalFormatTigerMouthPoor, 17, RivalChipPoor, 8, RivalFormatChipPoor, 8, RivalSinglePointPoor);
		AddRecord(n, n + 10, n - 10, 0, 0, 2, isAddScore);
		break;
		//ֻ���±������ӣ�ǰ��������������
	case 4:
		if (who == PlayerId) SetCurrentPoint(5, MyFormatChipPoor, 15, MySinglePointPoor);
		else SetCurrentPoint(8, RivalFormatChipPoor, 8, RivalSinglePointPoor);
		AddRecord(n, n - 10, n + 1, n - 1, 0, 5, isAddScore);
		break;
		//�¡��ұ߶�������
	case 5:
		if (who == PlayerId) SetCurrentPoint(3, MyFormatTigerMouthPoor, 16, MyChipPoor, 5, MyFormatChipPoor, 15, MySinglePointPoor);
		else SetCurrentPoint(6, RivalFormatTigerMouthPoor, 17, RivalChipPoor, 8, RivalFormatChipPoor, 8, RivalSinglePointPoor);
		AddRecord(n, n + 10, n + 1, 0, 0, 2, isAddScore);
		break;
		//�¡���߶�������
	case 6:
		if (who == PlayerId) SetCurrentPoint(3, MyFormatTigerMouthPoor, 16, MyChipPoor, 5, MyFormatChipPoor, 15, MySinglePointPoor);
		else SetCurrentPoint(6, RivalFormatTigerMouthPoor, 17, RivalChipPoor, 8, RivalFormatChipPoor, 8, RivalSinglePointPoor);
		AddRecord(n, n + 10, n - 1, 0, 0, 2, isAddScore);
		break;
		//�ҡ����¶�������
	case 7:
		if (who == PlayerId)
		{
			SetCurrentPoint(4, MyFormatEyePoor, 14, MyTigerMouthPoor, 3, MyFormatTigerMouthPoor, 16, MyChipPoor);
			Score[row1][row2] = PointStyle[14];
		}
		else SetCurrentPoint(7, RivalFormatEyePoor, 2, RivalTigerMouthPoor, 6, RivalFormatTigerMouthPoor, 17, RivalChipPoor);
		AddRecord(n, n + 1, n - 1, n + 10, 0, 3, isAddScore);
		break;
		//ֻ���ϱ������ӣ�ǰ��������������
	case 8:
		if (who == PlayerId) SetCurrentPoint(5, MyFormatChipPoor, 15, MySinglePointPoor);
		else SetCurrentPoint(8, RivalFormatChipPoor, 8, RivalSinglePointPoor);
		AddRecord(n, n + 10, n + 1, n - 1, 0, 5, isAddScore);
		break;
		//�ҡ��϶�������
	case 9:
		if (who == PlayerId) SetCurrentPoint(3, MyFormatTigerMouthPoor, 16, MyChipPoor, 5, MyFormatChipPoor, 15, MySinglePointPoor);
		else SetCurrentPoint(6, RivalFormatTigerMouthPoor, 17, RivalChipPoor, 8, RivalFormatChipPoor, 8, RivalSinglePointPoor);
		AddRecord(n, n + 1, n - 10, 0, 0, 2, isAddScore);
		break;
		//���϶�������
	case 10:
		if (who == PlayerId) SetCurrentPoint(3, MyFormatTigerMouthPoor, 16, MyChipPoor, 5, MyFormatChipPoor, 15, MySinglePointPoor);
		else SetCurrentPoint(6, RivalFormatTigerMouthPoor, 17, RivalChipPoor, 8, RivalFormatChipPoor, 8, RivalSinglePointPoor);
		AddRecord(n, n - 1, n - 10, 0, 0, 2, isAddScore);
		break;
		//���ҡ��϶�������
	case 11:
		if (who == PlayerId)
		{
			SetCurrentPoint(4, MyFormatEyePoor, 14, MyTigerMouthPoor, 3, MyFormatTigerMouthPoor, 16, MyChipPoor);
			Score[row1][row2] = PointStyle[14];
		}
		else SetCurrentPoint(7, RivalFormatEyePoor, 2, RivalTigerMouthPoor, 6, RivalFormatTigerMouthPoor, 17, RivalChipPoor);
		AddRecord(n, n - 1, n + 1, n - 10, 0, 3, isAddScore);
		break;
		//�ϡ��¶�������
	case 12:
		if (who == PlayerId) SetCurrentPoint(3, MyFormatTigerMouthPoor, 16, MyChipPoor, 5, MyFormatChipPoor, 15, MySinglePointPoor);
		else SetCurrentPoint(6, RivalFormatTigerMouthPoor, 17, RivalChipPoor, 8, RivalFormatChipPoor, 8, RivalSinglePointPoor);
		AddRecord(n, n + 10, n - 10, 0, 0, 2, isAddScore);
		break;
		//�ϡ��¡��Ҷ�������
	case 13:
		if (who == PlayerId)
		{
			SetCurrentPoint(4, MyFormatEyePoor, 14, MyTigerMouthPoor, 3, MyFormatTigerMouthPoor, 16, MyChipPoor);
			Score[row1][row2] = PointStyle[14];
		}
		else SetCurrentPoint(7, RivalFormatEyePoor, 2, RivalTigerMouthPoor, 6, RivalFormatTigerMouthPoor, 17, RivalChipPoor);
		AddRecord(n, n - 10, n + 10, n + 1, 0, 3, isAddScore);
		break;
		//�ϡ��¡���������
	case 14:
		if (who == PlayerId)
		{
			SetCurrentPoint(4, MyFormatEyePoor, 14, MyTigerMouthPoor, 3, MyFormatTigerMouthPoor, 16, MyChipPoor);
			Score[row1][row2] = PointStyle[14];
		}
		else SetCurrentPoint(7, RivalFormatEyePoor, 2, RivalTigerMouthPoor, 6, RivalFormatTigerMouthPoor, 17, RivalChipPoor);
		AddRecord(n, n - 10, n + 10, n - 1, 0, 3, isAddScore);
		break;
		//�ϡ��¡����Ҷ�������
	case 15:
		if (who == PlayerId)
		{
			SetCurrentPoint(13, 100, 14, 100, 4, MyFormatEyePoor, 14, MyTigerMouthPoor);
			Score[row1][row2] = PointStyle[13];
		}
		else
		{
			SetCurrentPoint(0, 0, 1, 0, 7, RivalFormatEyePoor, 2, RivalTigerMouthPoor);
			Score[row1][row2] = PointStyle[1];
		}
		AddRecord(n, n - 1, n + 1, n - 10, n + 10, 4, isAddScore);
		break;
	}
}
///��¼����ǰ��������ֵ
///����1��ThisFormatStyle ���� ���˵�ǰ���Ӻ���һ�����ӿ��γɵ����������
///����2��ThisFormatScorePoor ���� ���˵�ǰ���Ӻ���һ�����ӿ��γɵ���������͵ķ�ֵ����
///����3��ThisSpecialStyle ���� ���˵�ǰ���Ӻ��γɵ����������
///����4��ThisSpecialScorePoor ���� ���˵�ǰ���Ӻ��γɵ�������ֵ�Ĳ���
///����5��LastFormatStyle ���� ǰһ�����γɵ����������
///����6��LastFormatScorePoore ���� ǰһ�����γɵ���������ͷ�ֵ�Ĳ���
///����7��LastPointStyle ���� ǰһ���γɵ����������
///����8��LastScorePoor ���� ǰһ���γɵ���������ͷ�ֵ�Ĳ���
void AI1::SetCurrentPoint(int ThisFormatStyle, int ThisFormatScorePoor, int ThisSpecialStyle, int ThisSpecialScorePoor, int LastFormatStyle, int LastFormatScorePoore, int LastPointStyle, int LastScorePoor)
{
	CurrentFormatPointStyle = ThisFormatStyle;
	CurrentFormatScorePoor = ThisFormatScorePoor;
	CurrentPointStyle = ThisSpecialStyle;
	CurrentScorePoor = ThisSpecialScorePoor;

	LastFormatScorePoor = LastFormatScorePoore;
	LastFormatPointStyle = LastFormatStyle;
	LastSpecialPointStyle = LastPointStyle;
	LastSpecialScorePoor = LastScorePoor;
}
///��������ͨ�ú���
///������n�������λ��  n1-n4��������ڽ���λ��   amo�������
void AI1::AddRecord(int n, int n1, int n2, int n3, int n4, int amo, bool isAddScore)
{
	if (n / 10 > 0 && n / 10 < 10 && n % 10 > 0) ResetScore(n / 10, n % 10, PointStyle[CurrentPointStyle], CurrentScorePoor, isAddScore);
	switch (amo)
	{
	case 4:
		if (n / 10 > 0 && n / 10 < 10 && n % 10 > 0) ResetScore(n / 10, n % 10, PointStyle[CurrentFormatPointStyle], CurrentFormatScorePoor, isAddScore);
		break;
	case 5:;
	case 3:
		if (n3 / 10 > 0 && n3 / 10 < 10 && n3 % 10 > 0) ResetScore(n3 / 10, n3 % 10, PointStyle[CurrentFormatPointStyle], CurrentFormatScorePoor, isAddScore);
	case 2:
		if (n2 / 10 > 0 && n2 / 10 < 10 && n2 % 10 > 0) ResetScore(n2 / 10, n2 % 10, PointStyle[CurrentFormatPointStyle], CurrentFormatScorePoor, isAddScore);
	case 1:
		if (n1 / 10 > 0 && n1 / 10 < 10 && n1 % 10 > 0) ResetScore(n1 / 10, n1 % 10, PointStyle[CurrentFormatPointStyle], CurrentFormatScorePoor, isAddScore); break;
	}
	if (amo < 5)
	{
		if (n / 10 > 0 && n / 10 < 10 && n % 10 > 0) ResetScore(n / 10, n % 10, PointStyle[LastSpecialPointStyle], LastSpecialScorePoor, !isAddScore);
		switch (amo)
		{
		case 4:
			if (n4 / 10 > 0 && n4 / 10 < 10 && n4 % 10 > 0 && cross[n4 / 10][n4 % 10] == 0) ResetScore(n4 / 10, n4 % 10, PointStyle[LastFormatPointStyle], LastFormatScorePoor, !isAddScore);
		case 3:
			if (n3 / 10 > 0 && n3 / 10 < 10 && n3 % 10 > 0 && cross[n3 / 10][n3 % 10] == 0) ResetScore(n3 / 10, n3 % 10, PointStyle[LastFormatPointStyle], LastFormatScorePoor, !isAddScore);
		case 2:
			if (n2 / 10 > 0 && n2 / 10 < 10 && n2 % 10 > 0 && cross[n2 / 10][n2 % 10] == 0) ResetScore(n2 / 10, n2 % 10, PointStyle[LastFormatPointStyle], LastFormatScorePoor, !isAddScore);
		case 1:
			if (n1 / 10 > 0 && n1 / 10 < 10 && n1 % 10 > 0 && cross[n1 / 10][n1 % 10] == 0) ResetScore(n1 / 10, n1 % 10, PointStyle[LastFormatPointStyle], LastFormatScorePoor, !isAddScore);
		}
	}
}
///���������Ӱ��ķ�ֵ
///����row1�������������
///����row2�������������
///����score�����������λ�õķ�ֵ
///����NumberPoor������
///����isAddScore���Ƿ�ӷ֣���true��ӷ֣���false����֣�
void AI1::ResetScore(int row1, int row2, int score, int NumberPoor, bool isAddScore)
{
	int angelx1, angelx2, angelx3, angelx4;//4���ǵĺ�����
	int angely1, angely2, angely3, angely4;//4���ǵ�������
	int level = 0;
	if (Score[row1][row2] > 0)
	{
		if (isAddScore) Score[row1][row2] += score;//�ȶ����ļӷ�
		else Score[row1][row2] -= score;
	}
	///0������  1������  2������  3������
	bool edge[4] = { false, false, false, false };//��¼�Ƿ��ѵ���Ե
												  ///��ɢ�ӷ�
	while (score - NumberPoor > 0)
	{
		score -= NumberPoor;//��������һ��
		if (score < 1) break;
		level++;//������չһ��
				///��һ�����Ͻ�����
		angelx1 = (row1 - level) < 1 ? 1 : (row1 - level);
		angely1 = (row2 - level) < 1 ? 1 : (row2 - level);
		///��һ�����Ͻ�����
		angelx2 = (row1 - level) < 1 ? 1 : (row1 - level);
		angely2 = (row2 + level) > 9 ? 9 : (row2 + level);
		///��һ�����½�����
		angelx3 = (row1 + level) > 9 ? 9 : (row1 + level);
		angely3 = (row2 + level) > 9 ? 9 : (row2 + level);
		///��һ�����½�����
		angelx4 = (row1 + level) > 9 ? 9 : (row1 + level);
		angely4 = (row2 - level) < 1 ? 1 : (row2 - level);
		///�ӷ�
		///���ϱ߼ӷ�
		if (edge[1] == false)
		{
			if (isAddScore)
			{
				for (; angely1 < angely2; angely1++)
				{
					if (Score[angelx1][angely1] > 0)
					{
						Score[angelx1][angely1] += score;
					}
				}
			}
			else
			{
				for (; angely1 < angely2; angely1++)
				{
					if (Score[angelx1][angely1] > 0)
					{
						Score[angelx1][angely1] -= score;
						if (Score[angelx1][angely1] < 0.01) Score[angelx1][angely1] = 0.01;
					}
				}
			}
		}
		///���ұ߼ӷ�
		if (edge[2] == false)
		{
			if (isAddScore)
			{
				for (; angelx2 < angelx3; angelx2++)
				{
					if (Score[angelx2][angely2] > 0)
					{
						Score[angelx2][angely2] += score;
					}
				}
			}
			else
			{
				for (; angelx2 < angelx3; angelx2++)
				{
					if (Score[angelx2][angely2] > 0)
					{
						Score[angelx2][angely2] -= score;
						if (Score[angelx2][angely2] < 0.01) Score[angelx2][angely2] = 0.01;
					}
				}
			}
		}
		///���±߼ӷ�
		if (edge[3] == false)
		{
			if (isAddScore)
			{
				for (; angely3 > angely4; angely3--)
				{
					if (Score[angelx3][angely3] > 0)
					{
						Score[angelx3][angely3] += score;
					}
				}
			}
			else
			{
				for (; angely3 > angely4; angely3--)
				{
					if (Score[angelx3][angely3] > 0)
					{
						Score[angelx3][angely3] -= score;
						if (Score[angelx3][angely3] < 0.01) Score[angelx3][angely3] = 0.01;
					}
				}
			}
		}
		///����߼ӷ�
		if (edge[0] == false)
		{
			if (isAddScore)
			{
				for (; angelx4 > angelx1; angelx4--)
				{
					if (Score[angelx4][angely4] > 0)
					{
						Score[angelx4][angely4] += score;
					}
				}
			}
			else
			{
				for (; angelx4 > angelx1; angelx4--)
				{
					if (Score[angelx4][angely4] > 0)
					{
						Score[angelx4][angely4] -= score;
						if (Score[angelx4][angely4] < 0.01) Score[angelx4][angely4] = 0.01;
					}
				}
			}
		}
		///��δ�������Եʱ������Ϊ�ϱ�Ե��һ��λ�üӷ�/����
		if (edge[0] == false && edge[1] == true && Score[angelx1][angely1] > 0)
		{
			if (isAddScore == true) Score[angelx1][angely1] += score;
			else
			{
				Score[angelx1][angely1] -= score;
				if (Score[angelx1][angely1] < 0.01) Score[angelx1][angely1] = 0.01;
			}
		}
		///��δ�����ϱ�Եʱ������Ϊ�ұ�Ե��һ��λ�üӷ�/����
		if (edge[1] == false && edge[2] == true && Score[angelx2][angely2] > 0)
		{
			if (isAddScore == true) Score[angelx2][angely2] += score;
			else
			{
				Score[angelx2][angely2] -= score;
				if (Score[angelx2][angely2] < 0.01) Score[angelx2][angely2] = 0.01;
			}
		}
		///��δ�����ұ�Եʱ������Ϊ�±�Ե��һ��λ�üӷ�/�ӷ�
		if (edge[2] == false && edge[3] == true && Score[angelx3][angely3] > 0)
		{
			if (isAddScore == true) Score[angelx3][angely3] += score;
			else
			{
				Score[angelx3][angely3] -= score;
				if (Score[angelx3][angely3] < 0.01) Score[angelx3][angely3] = 0.01;
			}
		}
		///��δ�����±�Եʱ������Ϊ���Ե��һ��λ�üӷ�/����
		if (edge[3] == false && edge[0] == true && Score[angelx4][angely4] > 0)
		{
			if (isAddScore == true) Score[angelx4][angely4] += score;
			else
			{
				Score[angelx4][angely4] -= score;
				if (Score[angelx4][angely4] < 0.01) Score[angelx4][angely4] = 0.01;
			}
		}
		//���ӷ�������ɢ�����Եʱ
		if (row2 - level < 2) edge[0] = true;
		//���ӷ�������ɢ���ϱ�Եʱ
		if (row1 - level < 2) edge[1] = true;
		//���ӷ�������ɢ���ұ�Եʱ
		if (row2 + level > 8) edge[2] = true;
		//���ӷ�������ɢ���±�Եʱ
		if (row1 + level > 8) edge[3] = true;
		if (edge[0] && edge[1] && edge[2] && edge[3]) break;
	}
}
