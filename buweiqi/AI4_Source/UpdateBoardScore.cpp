#include "../AI4_Header/AI4.h"

void AI4::UpdateBoardScore(std::list<std::pair<int, int>> &SpecialPoint)
{
	std::list<std::pair<int, int>>::iterator Traverse = SpecialPoint.begin();//ͷ���
	std::list<std::pair<int, int>>::iterator ListEnd = SpecialPoint.end();//β���
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
	int angelx1, angelx2, angelx3, angelx4;//4���ǵĺ�����
	int angely1, angely2, angely3, angely4;//4���ǵ�������
	int level = 0;
	if (Score[row1][row2] > RivalDeadChess) Score[row1][row2] += score;//�ȶ����ļӷ�
	///0������  1������  2������  3������
	bool edge[4] = { false, false, false, false };//��¼�Ƿ��ѵ���Ե
	 ///��ɢ�ӷ�
	while (score - ScorePoor > 0)
	{
		score -= ScorePoor;//��������һ��
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
			for (; angely1 < angely2; angely1++)
			{
				if (Score[angelx1][angely1] > RivalDeadChess)
				{
					Score[angelx1][angely1] += score;
				}
			}
		}
		///���ұ߼ӷ�
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
		///���±߼ӷ�
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
		///����߼ӷ�
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
		///��δ�������Եʱ������Ϊ�ϱ�Ե��һ��λ�üӷ�/����
		if (edge[0] == false && edge[1] == true && Score[angelx1][angely1] > RivalDeadChess) Score[angelx1][angely1] += score;
		///��δ�����ϱ�Եʱ������Ϊ�ұ�Ե��һ��λ�üӷ�/����
		if (edge[1] == false && edge[2] == true && Score[angelx2][angely2] > RivalDeadChess) Score[angelx2][angely2] += score;
		///��δ�����ұ�Եʱ������Ϊ�±�Ե��һ��λ�üӷ�/�ӷ�
		if (edge[2] == false && edge[3] == true && Score[angelx3][angely3] > RivalDeadChess) Score[angelx3][angely3] += score;
		///��δ�����±�Եʱ������Ϊ���Ե��һ��λ�üӷ�/����
		if (edge[3] == false && edge[0] == true && Score[angelx4][angely4] > RivalDeadChess) Score[angelx4][angely4] += score;
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