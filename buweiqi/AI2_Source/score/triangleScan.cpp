#include "../../stdafx.h"
#include "../../AI2_Header/AI2.h"

/**
* [AI2::Tirangle ����Χɱ]
*/
void AI2::Tirangle()
{
	UTirangle();
	RTirangle();
	DTirangle();
	LTirangle();
}

/**
* [AI2::RTirangle �ҽ��������Χɱ]
*/
void AI2::RTirangle()
{
	for (int i = 1; i < 8; i++)
	{
		// --------------�ұ�--------------------
		if (cross[i + 1][9] == noChess)
		{
			// ���ǵ���--�м�����,û������
			// ����˫��
			if (cross[i][9] != noChess && cross[i + 1][8] != noChess && cross[i][9] == cross[i + 1][8] && cross[i + 2][9] == noChess)
			{
				chessScore[i + 2][9] += tirangleScore2;
			}
			else if (cross[i][9] != noChess && cross[i + 1][8] == noChess && cross[i][9] == cross[i + 2][9] && cross[i + 2][9] != noChess)
			{
				chessScore[i + 1][8] += tirangleScore2;
			}
			else if (cross[i][9] == noChess && cross[i + 1][8] != noChess && cross[i + 1][8] == cross[i + 2][9] && cross[i + 2][9] != noChess)
			{
				chessScore[i][9] += tirangleScore2;
			}
			// ��������,�Ѿ��γɰ�ΧȦ��
			else if (cross[i][9] == isWhite && cross[i + 1][8] == isWhite && cross[i + 2][9] == isWhite)
			{
				chessScore[i + 1][9] = minLimit;
			}
			else if (cross[i][9] == isBlack && cross[i + 1][8] == isBlack && cross[i + 2][9] == isBlack)
			{
				chessScore[i + 1][9] = minLimit;
			}
		}
		else if (cross[i + 1][9] == Rival)
		{
			// ����˫��
			if (cross[i][9] == turn2Who && cross[i][9] == cross[i + 1][8] && cross[i + 2][9] == noChess)
			{
				chessScore[i + 2][9] = minLimit;
			}
			else if (cross[i][9] == turn2Who && cross[i + 1][8] == noChess && cross[i][9] == cross[i + 2][9])
			{
				chessScore[i + 1][8] = minLimit;
			}
			else if (cross[i][9] == noChess && cross[i + 1][8] == turn2Who && cross[i + 1][8] == cross[i + 2][9])
			{
				chessScore[i][9] = minLimit;
			}
		}
	}
}

/**
* [AI2::RTirangle ����������Χɱ]
*/
void AI2::LTirangle()
{
	for (int i = 1; i < 8; i++)
	{
		// --------------���--------------------
		if (cross[i + 1][1] == noChess)
		{
			// ���ǵ���--�м�����,û������
			// ����˫��
			if (cross[i][1] != noChess && cross[i + 1][2] != noChess && cross[i][1] == cross[i + 1][2] && cross[i + 2][1] == noChess)
			{
				chessScore[i + 2][1] += tirangleScore2;
			}
			else if (cross[i][1] != noChess && cross[i + 1][2] == noChess && cross[i][1] == cross[i + 2][1] && cross[i + 2][1] != noChess)
			{
				chessScore[i + 1][2] += tirangleScore2;
			}
			else if (cross[i][1] == noChess && cross[i + 1][2] != noChess && cross[i + 1][2] == cross[i + 2][1] && cross[i + 2][1] != noChess)
			{
				chessScore[i][1] += tirangleScore2;
			}
			// ��������
			else if (cross[i][1] == isWhite && cross[i + 1][2] == isWhite && cross[i + 2][1] == isWhite)
			{
				chessScore[i + 1][1] = minLimit;
			}
			else if (cross[i][1] == isBlack && cross[i + 1][2] == isBlack && cross[i + 2][1] == isBlack)
			{
				chessScore[i + 1][1] = minLimit;
			}
		}
		else if (cross[i + 1][1] == Rival)
		{
			// ����˫��,�Ѿ��γɰ�ΧȦ��
			if (cross[i][1] == turn2Who && cross[i][1] == cross[i + 1][2] && cross[i + 2][1] == noChess)
			{
				chessScore[i + 2][1] = minLimit;
			}
			else if (cross[i][1] == turn2Who && cross[i + 1][2] == noChess && cross[i][1] == cross[i + 2][1])
			{
				chessScore[i + 1][2] = minLimit;
			}
			else if (cross[i][1] == noChess && cross[i + 1][2] == turn2Who && cross[i + 1][2] == cross[i + 2][1])
			{
				chessScore[i][1] = minLimit;
			}
		}
	}
}

/**
* [AI2::RTirangle �Ͻ��������Χɱ]
*/
void AI2::UTirangle()
{
	for (int i = 1; i < 8; i++)
	{
		// --------------�ϱ�--------------------
		if (cross[1][i + 1] == noChess)
		{
			// ���ǵ���--�м�����,û������
			// ����˫��
			if (cross[1][i] != noChess && cross[2][i + 1] != noChess && cross[1][i] == cross[2][i + 1] && cross[1][i + 2] == noChess)
			{
				chessScore[1][i + 2] += tirangleScore2;
			}
			else if (cross[1][i] != noChess && cross[2][i + 1] == noChess && cross[1][i] == cross[1][i + 2] && cross[1][i + 2] != noChess)
			{
				chessScore[2][i + 1] += tirangleScore2;
			}
			else if (cross[1][i] == noChess && cross[2][i + 1] != noChess && cross[1][i + 2] == cross[2][i + 1] && cross[1][i + 2] != noChess)
			{
				chessScore[1][i] += tirangleScore2;
			}
			// ��������
			else if (cross[1][i] == isWhite && cross[2][i + 1] == isWhite && cross[1][i + 2] == isWhite)
			{
				chessScore[1][i + 1] = minLimit;
			}
			else if (cross[1][i] == isBlack && cross[2][i + 1] == isBlack && cross[1][i + 2] == isBlack)
			{
				chessScore[1][i + 1] = minLimit;
			}
		}
		else if (cross[1][i + 1] == Rival)
		{
			// ����˫��,�Ѿ��γɰ�ΧȦ��
			if (cross[2][i + 1] == turn2Who && cross[1][i] == cross[2][i + 1] && cross[1][i + 2] == noChess)
			{
				chessScore[1][i + 2] = minLimit;
			}
			else if (cross[1][i] == turn2Who && cross[2][i + 1] == noChess && cross[1][i] == cross[1][i + 2])
			{
				chessScore[2][i + 1] = minLimit;
			}
			else if (cross[1][i] == noChess && cross[2][i + 1] == turn2Who && cross[1][i + 2] == cross[2][i + 1])
			{
				chessScore[1][i] = minLimit;
			}
		}
	}
}

/**
* [AI2::DTirangle �±߽������Χɱ]
*/
void AI2::DTirangle()
{
	for (int i = 1; i < 8; i++)
	{
		// -------------�±�--------------------
		if (cross[9][i + 1] == noChess)
		{
			// ���ǵ���--�м�����,û������
			// ����˫��
			if (cross[9][i] != noChess && cross[8][i + 1] != noChess && cross[9][i] == cross[8][i + 1] && cross[9][i + 2] == noChess)
			{
				chessScore[9][i + 2] += tirangleScore2;
			}
			else if (cross[9][i] != noChess && cross[8][i + 1] == noChess && cross[9][i] == cross[9][i + 2] && cross[9][i + 2] != noChess)
			{
				chessScore[8][i + 1] += tirangleScore2;
			}
			else if (cross[9][i] == noChess && cross[8][i + 1] != noChess && cross[8][i + 1] == cross[9][i + 2] && cross[9][i + 2] != noChess)
			{
				chessScore[9][i] += tirangleScore2;
			}
			// ��������,�Ѿ��γɰ�ΧȦ��
			else if (cross[9][i] == isWhite && cross[8][i + 1] == isWhite && cross[9][i + 2] == isWhite)
			{
				chessScore[9][i + 1] = minLimit;
			}
			else if (cross[9][i] == isBlack && cross[8][i + 1] == isBlack && cross[9][i + 2] == isBlack)
			{
				chessScore[9][i + 1] = minLimit;
			}
		}
		else if (cross[9][i + 1] == Rival)
		{
			// ����˫��
			if (cross[9][i] == turn2Who && cross[9][i] == cross[8][i + 1] && cross[9][i + 2] == noChess)
			{
				chessScore[9][i + 2] = minLimit;
			}
			else if (cross[9][i] == turn2Who && cross[8][i + 1] == noChess && cross[9][i] == cross[9][i + 2])
			{
				chessScore[8][i + 1] = minLimit;
			}
			else if (cross[9][i] == noChess && cross[8][i + 1] == turn2Who && cross[8][i + 1] == cross[9][i + 2])
			{
				chessScore[9][i] = minLimit;
			}
		}
	}
}
