#include "../AI4_Header/AI4.h"

void AI4::UpdateBoardScore(std::list<std::pair<int, int>> &SpecialPoint)
{
	std::list<std::pair<int, int>>::iterator Traverse = SpecialPoint.begin();//头结点
	std::list<std::pair<int, int>>::iterator ListEnd = SpecialPoint.end();//尾结点
	while (Traverse != ListEnd)
	{
		if (Traverse->second < 0) Score[GetLine(Traverse->first)][GetColumn(Traverse->first)] = MyDeadChess;
		else if(Traverse->second == 0) Score[GetLine(Traverse->first)][GetColumn(Traverse->first)] = RivalDeadChess;
		else if(Traverse->second == 25) AddScore<int, int>(GetLine(Traverse->first), GetColumn(Traverse->first), Traverse->second, 12);
		else AddScore<int, int>(GetLine(Traverse->first), GetColumn(Traverse->first), Traverse->second, 2);
		auto T = Traverse;
		Traverse++;
		SpecialPoint.erase(T);
	}
}

template<class T1, class T2>
void AI4::AddScore(int row1, int row2, T1 score, T2 ScorePoor)
{
	int angelx1, angelx2, angelx3, angelx4;//4个角的横坐标
	int angely1, angely2, angely3, angely4;//4个角的纵坐标
	int level = 0;
	if (Score[row1][row2] > RivalDeadChess) Score[row1][row2] += score;//先对中心加分
	///0——左  1——上  2——右  3——下
	bool edge[4] = { false, false, false, false };//记录是否已到边缘
	 ///扩散加分
	while (score - ScorePoor > 0)
	{
		score -= ScorePoor;//分数减少一级
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
			for (; angely1 < angely2; angely1++)
			{
				if (Score[angelx1][angely1] > RivalDeadChess)
				{
					Score[angelx1][angely1] += score;
				}
			}
		}
		///对右边加分
		if (edge[2] == false)
		{
			for (; angelx2 < angelx3; angelx2++)
			{
				if (Score[angelx2][angely2] > RivalDeadChess)
				{
					Score[angelx2][angely2] += score;
				}
			}
		}
		///对下边加分
		if (edge[3] == false)
		{
			for (; angely3 > angely4; angely3--)
			{
				if (Score[angelx3][angely3] > RivalDeadChess)
				{
					Score[angelx3][angely3] += score;
				}
			}
		}
		///对左边加分
		if (edge[0] == false)
		{
			for (; angelx4 > angelx1; angelx4--)
			{
				if (Score[angelx4][angely4] > RivalDeadChess)
				{
					Score[angelx4][angely4] += score;
				}
			}
		}
		///在未到达左边缘时，继续为上边缘的一个位置加分/减分
		if (edge[0] == false && edge[1] == true && Score[angelx1][angely1] > RivalDeadChess) Score[angelx1][angely1] += score;
		///在未到达上边缘时，继续为右边缘的一个位置加分/减分
		if (edge[1] == false && edge[2] == true && Score[angelx2][angely2] > RivalDeadChess) Score[angelx2][angely2] += score;
		///在未到达右边缘时，继续为下边缘的一个位置加分/加分
		if (edge[2] == false && edge[3] == true && Score[angelx3][angely3] > RivalDeadChess) Score[angelx3][angely3] += score;
		///在未到达下边缘时，继续为左边缘的一个位置加分/减分
		if (edge[3] == false && edge[0] == true && Score[angelx4][angely4] > RivalDeadChess) Score[angelx4][angely4] += score;
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