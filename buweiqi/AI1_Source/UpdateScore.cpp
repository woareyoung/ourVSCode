#include "../stdafx.h"
#include "../AI1_Header/AI1.h"
///更新棋盘位置分值，参数：行，列，下棋的玩家，是否是加分
void AI1::UpdateScore(int row1, int row2, int who, bool isAddScore)
{
	Score[row1][row2] = PointStyle[10];//设置下棋点为-5分
	if (row1 != 0 && row2 > 1 && cross[row1][row2 - 1] == 0) RecordSpecialPoint(row1, row2 - 1, who, isAddScore);//查看下棋点的左边位置上的特殊点
	if (row1 != 0 && row2 < 9 && cross[row1][row2 + 1] == 0) RecordSpecialPoint(row1, row2 + 1, who, isAddScore);//查看下棋点的右边位置上的特殊点
	if (row2 != 0 && row1 > 1 && cross[row1 - 1][row2] == 0) RecordSpecialPoint(row1 - 1, row2, who, isAddScore);//查看下棋点的上边位置上的特殊点
	if (row2 != 0 && row1 < 9 && cross[row1 + 1][row2] == 0) RecordSpecialPoint(row1 + 1, row2, who, isAddScore);//查看下棋点的下边位置上的特殊点
}
///添加特殊点
void AI1::RecordSpecialPoint(int row1, int row2, int who, bool isAddScore)
{
	int n = row1 * 10 + row2, sc = 0;
	GetChessAmount(row1, row2, who, sc);//查看特定位置周围有多少个“自己”的棋子
	switch (sc)
	{
		//只有右边有棋子，前两个参数无意义
	case 1:
		if (who == PlayerId) SetCurrentPoint(5, MyFormatChipPoor, 15, MySinglePointPoor);
		else SetCurrentPoint(8, RivalFormatChipPoor, 8, RivalSinglePointPoor);
		AddRecord(n, n + 10, n - 10, n - 1, 0, 5, isAddScore);
		break;
		//只有左边有棋子，前两个参数无意义
	case 2:
		if (who == PlayerId) SetCurrentPoint(5, MyFormatChipPoor, 15, MySinglePointPoor);
		else SetCurrentPoint(8, RivalFormatChipPoor, 8, RivalSinglePointPoor);
		AddRecord(n, n + 10, n + 1, n - 10, 0, 5, isAddScore);
		break;
		//左、右边都有棋子
	case 3:
		if (who == PlayerId) SetCurrentPoint(3, MyFormatTigerMouthPoor, 16, MyChipPoor, 5, MyFormatChipPoor, 15, MySinglePointPoor);
		else SetCurrentPoint(6, RivalFormatTigerMouthPoor, 17, RivalChipPoor, 8, RivalFormatChipPoor, 8, RivalSinglePointPoor);
		AddRecord(n, n + 10, n - 10, 0, 0, 2, isAddScore);
		break;
		//只有下边有棋子，前两个参数无意义
	case 4:
		if (who == PlayerId) SetCurrentPoint(5, MyFormatChipPoor, 15, MySinglePointPoor);
		else SetCurrentPoint(8, RivalFormatChipPoor, 8, RivalSinglePointPoor);
		AddRecord(n, n - 10, n + 1, n - 1, 0, 5, isAddScore);
		break;
		//下、右边都有棋子
	case 5:
		if (who == PlayerId) SetCurrentPoint(3, MyFormatTigerMouthPoor, 16, MyChipPoor, 5, MyFormatChipPoor, 15, MySinglePointPoor);
		else SetCurrentPoint(6, RivalFormatTigerMouthPoor, 17, RivalChipPoor, 8, RivalFormatChipPoor, 8, RivalSinglePointPoor);
		AddRecord(n, n + 10, n + 1, 0, 0, 2, isAddScore);
		break;
		//下、左边都有棋子
	case 6:
		if (who == PlayerId) SetCurrentPoint(3, MyFormatTigerMouthPoor, 16, MyChipPoor, 5, MyFormatChipPoor, 15, MySinglePointPoor);
		else SetCurrentPoint(6, RivalFormatTigerMouthPoor, 17, RivalChipPoor, 8, RivalFormatChipPoor, 8, RivalSinglePointPoor);
		AddRecord(n, n + 10, n - 1, 0, 0, 2, isAddScore);
		break;
		//右、左、下都有棋子
	case 7:
		if (who == PlayerId)
		{
			SetCurrentPoint(4, MyFormatEyePoor, 14, MyTigerMouthPoor, 3, MyFormatTigerMouthPoor, 16, MyChipPoor);
			Score[row1][row2] = PointStyle[14];
		}
		else SetCurrentPoint(7, RivalFormatEyePoor, 2, RivalTigerMouthPoor, 6, RivalFormatTigerMouthPoor, 17, RivalChipPoor);
		AddRecord(n, n + 1, n - 1, n + 10, 0, 3, isAddScore);
		break;
		//只有上边有棋子，前两个参数无意义
	case 8:
		if (who == PlayerId) SetCurrentPoint(5, MyFormatChipPoor, 15, MySinglePointPoor);
		else SetCurrentPoint(8, RivalFormatChipPoor, 8, RivalSinglePointPoor);
		AddRecord(n, n + 10, n + 1, n - 1, 0, 5, isAddScore);
		break;
		//右、上都有棋子
	case 9:
		if (who == PlayerId) SetCurrentPoint(3, MyFormatTigerMouthPoor, 16, MyChipPoor, 5, MyFormatChipPoor, 15, MySinglePointPoor);
		else SetCurrentPoint(6, RivalFormatTigerMouthPoor, 17, RivalChipPoor, 8, RivalFormatChipPoor, 8, RivalSinglePointPoor);
		AddRecord(n, n + 1, n - 10, 0, 0, 2, isAddScore);
		break;
		//左、上都有棋子
	case 10:
		if (who == PlayerId) SetCurrentPoint(3, MyFormatTigerMouthPoor, 16, MyChipPoor, 5, MyFormatChipPoor, 15, MySinglePointPoor);
		else SetCurrentPoint(6, RivalFormatTigerMouthPoor, 17, RivalChipPoor, 8, RivalFormatChipPoor, 8, RivalSinglePointPoor);
		AddRecord(n, n - 1, n - 10, 0, 0, 2, isAddScore);
		break;
		//左、右、上都有棋子
	case 11:
		if (who == PlayerId)
		{
			SetCurrentPoint(4, MyFormatEyePoor, 14, MyTigerMouthPoor, 3, MyFormatTigerMouthPoor, 16, MyChipPoor);
			Score[row1][row2] = PointStyle[14];
		}
		else SetCurrentPoint(7, RivalFormatEyePoor, 2, RivalTigerMouthPoor, 6, RivalFormatTigerMouthPoor, 17, RivalChipPoor);
		AddRecord(n, n - 1, n + 1, n - 10, 0, 3, isAddScore);
		break;
		//上、下都有棋子
	case 12:
		if (who == PlayerId) SetCurrentPoint(3, MyFormatTigerMouthPoor, 16, MyChipPoor, 5, MyFormatChipPoor, 15, MySinglePointPoor);
		else SetCurrentPoint(6, RivalFormatTigerMouthPoor, 17, RivalChipPoor, 8, RivalFormatChipPoor, 8, RivalSinglePointPoor);
		AddRecord(n, n + 10, n - 10, 0, 0, 2, isAddScore);
		break;
		//上、下、右都有棋子
	case 13:
		if (who == PlayerId)
		{
			SetCurrentPoint(4, MyFormatEyePoor, 14, MyTigerMouthPoor, 3, MyFormatTigerMouthPoor, 16, MyChipPoor);
			Score[row1][row2] = PointStyle[14];
		}
		else SetCurrentPoint(7, RivalFormatEyePoor, 2, RivalTigerMouthPoor, 6, RivalFormatTigerMouthPoor, 17, RivalChipPoor);
		AddRecord(n, n - 10, n + 10, n + 1, 0, 3, isAddScore);
		break;
		//上、下、左都有棋子
	case 14:
		if (who == PlayerId)
		{
			SetCurrentPoint(4, MyFormatEyePoor, 14, MyTigerMouthPoor, 3, MyFormatTigerMouthPoor, 16, MyChipPoor);
			Score[row1][row2] = PointStyle[14];
		}
		else SetCurrentPoint(7, RivalFormatEyePoor, 2, RivalTigerMouthPoor, 6, RivalFormatTigerMouthPoor, 17, RivalChipPoor);
		AddRecord(n, n - 10, n + 10, n - 1, 0, 3, isAddScore);
		break;
		//上、下、左、右都有棋子
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
///记录“当前”特殊点分值
///参数1：ThisFormatStyle —— 下了当前棋子后下一个棋子可形成的特殊点类型
///参数2：ThisFormatScorePoor —— 下了当前棋子后下一个棋子可形成的特殊点类型的分值层差分
///参数3：ThisSpecialStyle —— 下了当前棋子后形成的特殊点类型
///参数4：ThisSpecialScorePoor —— 下了当前棋子后形成的特殊点分值的层差分
///参数5：LastFormatStyle —— 前一步可形成的特殊点类型
///参数6：LastFormatScorePoore —— 前一步可形成的特殊点类型分值的层差分
///参数7：LastPointStyle —— 前一步形成的特殊点类型
///参数8：LastScorePoor —— 前一步形成的特殊点类型分值的层差分
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
///添加特殊点通用函数
///参数：n：特殊点位置  n1-n4：特殊点邻近的位置   amo：标记用
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
///设置特殊点影响的分值
///参数row1：特殊点所在行
///参数row2：特殊点所在列
///参数score：特殊点所在位置的分值
///参数NumberPoor：层差分
///参数isAddScore：是否加分（若true则加分，若false则减分）
void AI1::ResetScore(int row1, int row2, int score, int NumberPoor, bool isAddScore)
{
	int angelx1, angelx2, angelx3, angelx4;//4个角的横坐标
	int angely1, angely2, angely3, angely4;//4个角的纵坐标
	int level = 0;
	if (Score[row1][row2] > 0)
	{
		if (isAddScore) Score[row1][row2] += score;//先对中心加分
		else Score[row1][row2] -= score;
	}
	///0——左  1——上  2——右  3——下
	bool edge[4] = { false, false, false, false };//记录是否已到边缘
												  ///扩散加分
	while (score - NumberPoor > 0)
	{
		score -= NumberPoor;//分数减少一级
		if (score < 1) break;
		level++;//对外扩展一层
				///下一层左上角坐标
		angelx1 = (row1 - level) < 1 ? 1 : (row1 - level);
		angely1 = (row2 - level) < 1 ? 1 : (row2 - level);
		///下一层右上角坐标
		angelx2 = (row1 - level) < 1 ? 1 : (row1 - level);
		angely2 = (row2 + level) > 9 ? 9 : (row2 + level);
		///下一层右下角坐标
		angelx3 = (row1 + level) > 9 ? 9 : (row1 + level);
		angely3 = (row2 + level) > 9 ? 9 : (row2 + level);
		///下一层左下角坐标
		angelx4 = (row1 + level) > 9 ? 9 : (row1 + level);
		angely4 = (row2 - level) < 1 ? 1 : (row2 - level);
		///加分
		///对上边加分
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
		///对右边加分
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
		///对下边加分
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
		///对左边加分
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
		///在未到达左边缘时，继续为上边缘的一个位置加分/减分
		if (edge[0] == false && edge[1] == true && Score[angelx1][angely1] > 0)
		{
			if (isAddScore == true) Score[angelx1][angely1] += score;
			else
			{
				Score[angelx1][angely1] -= score;
				if (Score[angelx1][angely1] < 0.01) Score[angelx1][angely1] = 0.01;
			}
		}
		///在未到达上边缘时，继续为右边缘的一个位置加分/减分
		if (edge[1] == false && edge[2] == true && Score[angelx2][angely2] > 0)
		{
			if (isAddScore == true) Score[angelx2][angely2] += score;
			else
			{
				Score[angelx2][angely2] -= score;
				if (Score[angelx2][angely2] < 0.01) Score[angelx2][angely2] = 0.01;
			}
		}
		///在未到达右边缘时，继续为下边缘的一个位置加分/加分
		if (edge[2] == false && edge[3] == true && Score[angelx3][angely3] > 0)
		{
			if (isAddScore == true) Score[angelx3][angely3] += score;
			else
			{
				Score[angelx3][angely3] -= score;
				if (Score[angelx3][angely3] < 0.01) Score[angelx3][angely3] = 0.01;
			}
		}
		///在未到达下边缘时，继续为左边缘的一个位置加分/减分
		if (edge[3] == false && edge[0] == true && Score[angelx4][angely4] > 0)
		{
			if (isAddScore == true) Score[angelx4][angely4] += score;
			else
			{
				Score[angelx4][angely4] -= score;
				if (Score[angelx4][angely4] < 0.01) Score[angelx4][angely4] = 0.01;
			}
		}
		//当加分区域扩散到左边缘时
		if (row2 - level < 2) edge[0] = true;
		//当加分区域扩散到上边缘时
		if (row1 - level < 2) edge[1] = true;
		//当加分区域扩散到右边缘时
		if (row2 + level > 8) edge[2] = true;
		//当加分区域扩散到下边缘时
		if (row1 + level > 8) edge[3] = true;
		if (edge[0] && edge[1] && edge[2] && edge[3]) break;
	}
}
