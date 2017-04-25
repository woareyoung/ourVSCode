#include "../../stdafx.h"
#include "../../AI2_Header/AI2.h"

/**
* [AI2::chessStatusShaped ʮ��Χɱ]
* @return [��]
*/
void AI2::chessStatusShaped()
{
	for (int i = 2; i < 9; i++)
	{
		for (int j = 2; j < 9; j++)
		{
			JudgeCShape(i, j);
		}
	}
}

/**
* [AI2::JudgeCShape �ж��Ƿ��γ�ʮ��Χɱ]
* @param line   [��]
* @param column [��]
*/
void AI2::JudgeCShape(int line, int column)
{
	if (line < 1 || column < 1 || line > 9 || column > 9) {
		return;
	}
	// �Ѿ��γ���ʮ��Χɱ��
	if (cross[line - 1][column] == isWhite && cross[line + 1][column] == isWhite
		&& cross[line][column - 1] == isWhite && cross[line][column + 1] == isWhite) {
		chessScore[line][column] = minLimit;
	}
	if (cross[line - 1][column] == isBlack && cross[line + 1][column] == isBlack
		&& cross[line][column - 1] == isBlack && cross[line][column + 1] == isBlack) {
		chessScore[line][column] = minLimit;
	}
	// ȱһ�γ�ʮ��Χɱ-- ����-- �м�û�����ӵ����
	// ----------------------������Ҫ������ȼ���Ӧ�����Բ�����Ϊ�ײ�δ��׼
	if (cross[line][column] == noChess &&
		cross[line - 1][column] == noChess && cross[line + 1][column] == isWhite
		&& cross[line][column - 1] == isWhite && cross[line][column + 1] == isWhite) {
		chessScore[line - 1][column] += priority_score(crossShaped3, crossShaped4, isWhite);
	}
	else if (cross[line][column] == noChess &&
		cross[line - 1][column] == isWhite && cross[line + 1][column] == noChess
		&& cross[line][column - 1] == isWhite && cross[line][column + 1] == isWhite) {
		chessScore[line + 1][column] += priority_score(crossShaped3, crossShaped4, isWhite);
	}
	else if (cross[line][column] == noChess &&
		cross[line - 1][column] == isWhite && cross[line + 1][column] == isWhite
		&& cross[line][column - 1] == noChess && cross[line][column + 1] == isWhite) {
		chessScore[line][column - 1] += priority_score(crossShaped3, crossShaped4, isWhite);
	}
	else if (cross[line][column] == noChess &&
		cross[line - 1][column] == isWhite && cross[line + 1][column] == isWhite
		&& cross[line][column - 1] == isWhite && cross[line][column + 1] == noChess) {
		chessScore[line][column + 1] += priority_score(crossShaped3, crossShaped4, isWhite);
	}
	// ȱһ�γ�ʮ��Χɱ-- ����-- �м�û�����ӵ����
	// ----------------------������Ҫ������ȼ���Ӧ�����Բ�����Ϊ�ײ�δ��׼
	if (cross[line][column] == noChess &&
		cross[line - 1][column] == noChess && cross[line + 1][column] == isBlack
		&& cross[line][column - 1] == isBlack && cross[line][column + 1] == isBlack) {
		chessScore[line - 1][column] += priority_score(crossShaped3, crossShaped4, isBlack);
	}
	else if (cross[line][column] == noChess &&
		cross[line - 1][column] == isBlack && cross[line + 1][column] == noChess
		&& cross[line][column - 1] == isBlack && cross[line][column + 1] == isBlack) {
		chessScore[line + 1][column] += priority_score(crossShaped3, crossShaped4, isBlack);
	}
	else if (cross[line][column] == noChess &&
		cross[line - 1][column] == isBlack && cross[line + 1][column] == isBlack
		&& cross[line][column - 1] == noChess && cross[line][column + 1] == isBlack) {
		chessScore[line][column - 1] += priority_score(crossShaped3, crossShaped4, isBlack);
	}
	else if (cross[line][column] == noChess &&
		cross[line - 1][column] == isBlack && cross[line + 1][column] == isBlack
		&& cross[line][column - 1] == isBlack && cross[line][column + 1] == noChess) {
		chessScore[line][column + 1] += priority_score(crossShaped3, crossShaped4, isBlack);
	}

	// ȱһ�γ�ʮ��Χɱ-- ����-- �м������ӵ����
	// ----------------------������Ҫ������ȼ���Ӧ�����Բ�����Ϊ�ײ�δ��׼
	if (cross[line][column] != noChess &&
		cross[line - 1][column] == noChess && cross[line + 1][column] == isWhite
		&& cross[line][column - 1] == isWhite && cross[line][column + 1] == isWhite) {
		chessScore[line - 1][column] = minLimit;
	}
	else if (cross[line][column] != noChess &&
		cross[line - 1][column] == isWhite && cross[line + 1][column] == noChess
		&& cross[line][column - 1] == isWhite && cross[line][column + 1] == isWhite) {
		chessScore[line + 1][column] = minLimit;
	}
	else if (cross[line][column] != noChess &&
		cross[line - 1][column] == isWhite && cross[line + 1][column] == isWhite
		&& cross[line][column - 1] == noChess && cross[line][column + 1] == isWhite) {
		chessScore[line][column - 1] = minLimit;
	}
	else if (cross[line][column] != noChess &&
		cross[line - 1][column] == isWhite && cross[line + 1][column] == isWhite
		&& cross[line][column - 1] == isWhite && cross[line][column + 1] == noChess) {
		chessScore[line][column + 1] = minLimit;
	}
	// ȱһ�γ�ʮ��Χɱ-- ����-- �м������ӵ����
	// ----------------------������Ҫ������ȼ���Ӧ�����Բ�����Ϊ�ײ�δ��׼
	if (cross[line][column] != noChess &&
		cross[line - 1][column] == noChess && cross[line + 1][column] == isBlack
		&& cross[line][column - 1] == isBlack && cross[line][column + 1] == isBlack) {
		chessScore[line - 1][column] = minLimit;
	}
	else if (cross[line][column] != noChess &&
		cross[line - 1][column] == isBlack && cross[line + 1][column] == noChess
		&& cross[line][column - 1] == isBlack && cross[line][column + 1] == isBlack) {
		chessScore[line + 1][column] = minLimit;
	}
	else if (cross[line][column] != noChess &&
		cross[line - 1][column] == isBlack && cross[line + 1][column] == isBlack
		&& cross[line][column - 1] == noChess && cross[line][column + 1] == isBlack) {
		chessScore[line][column - 1] = minLimit;
	}
	else if (cross[line][column] != noChess &&
		cross[line - 1][column] == isBlack && cross[line + 1][column] == isBlack
		&& cross[line][column - 1] == isBlack && cross[line][column + 1] == noChess) {
		chessScore[line][column + 1] = minLimit;
	}
	/*
	// ȱ���γ�ʮ��Χɱ-- ����-- �м�û�����ӵ����
	// ----------------------������Ҫ������ȼ���Ӧ�����Բ�����Ϊ�ײ�δ��׼
	if (cross[line][column] == noChess &&
	cross[line - 1][column] == noChess && cross[line + 1][column] == noChess
	&& cross[line][column - 1] == isWhite && cross[line][column + 1] == isWhite) {
	chessScore[line - 1][column] += priority_score(crossShaped2, crossShaped3, isWhite);
	chessScore[line + 1][column] += priority_score(crossShaped2, crossShaped3, isWhite);
	} else if (cross[line][column] == noChess &&
	cross[line - 1][column] == noChess && cross[line + 1][column] == isWhite
	&& cross[line][column - 1] == noChess && cross[line][column + 1] == isWhite) {
	chessScore[line - 1][column] += priority_score(crossShaped2, crossShaped3, isWhite);
	chessScore[line][column - 1] += priority_score(crossShaped2, crossShaped3, isWhite);
	} else if (cross[line][column] == noChess &&
	cross[line - 1][column] == noChess && cross[line + 1][column] == isWhite
	&& cross[line][column - 1] == isWhite && cross[line][column + 1] == noChess) {
	chessScore[line - 1][column] += priority_score(crossShaped2, crossShaped3, isWhite);
	chessScore[line][column + 1] += priority_score(crossShaped2, crossShaped3, isWhite);
	} else if (cross[line][column] == noChess &&
	cross[line - 1][column] == isWhite && cross[line + 1][column] == noChess
	&& cross[line][column - 1] == noChess && cross[line][column + 1] == isWhite) {
	chessScore[line + 1][column] += priority_score(crossShaped2, crossShaped3, isWhite);
	chessScore[line][column - 1] += priority_score(crossShaped2, crossShaped3, isWhite);
	} else if (cross[line][column] == noChess &&
	cross[line - 1][column] == isWhite && cross[line + 1][column] == noChess
	&& cross[line][column - 1] == isWhite && cross[line][column + 1] == noChess) {
	chessScore[line + 1][column] += priority_score(crossShaped2, crossShaped3, isWhite);
	chessScore[line][column + 1] += priority_score(crossShaped2, crossShaped3, isWhite);
	} else if (cross[line][column] == noChess &&
	cross[line - 1][column] == isWhite && cross[line + 1][column] == isWhite
	&& cross[line][column - 1] == noChess && cross[line][column + 1] == noChess) {
	chessScore[line][column - 1] += priority_score(crossShaped2, crossShaped3, isWhite);
	chessScore[line][column + 1] += priority_score(crossShaped2, crossShaped3, isWhite);
	}
	// ȱ���γ�ʮ��Χɱ-- ����-- �м�û�����ӵ����
	// ----------------------������Ҫ������ȼ���Ӧ�����Բ�����Ϊ�ײ�δ��׼
	if (cross[line][column] == noChess &&
	cross[line - 1][column] == noChess && cross[line + 1][column] == noChess
	&& cross[line][column - 1] == isBlack && cross[line][column + 1] == isBlack) {
	chessScore[line - 1][column] += priority_score(crossShaped2, crossShaped3, isBlack);
	chessScore[line + 1][column] += priority_score(crossShaped2, crossShaped3, isBlack);
	} else if (cross[line][column] == noChess &&
	cross[line - 1][column] == noChess && cross[line + 1][column] == isBlack
	&& cross[line][column - 1] == noChess && cross[line][column + 1] == isBlack) {
	chessScore[line - 1][column] += priority_score(crossShaped2, crossShaped3, isBlack);
	chessScore[line][column - 1] += priority_score(crossShaped2, crossShaped3, isBlack);
	} else if (cross[line][column] == noChess &&
	cross[line - 1][column] == noChess && cross[line + 1][column] == isBlack
	&& cross[line][column - 1] == isBlack && cross[line][column + 1] == noChess) {
	chessScore[line - 1][column] += priority_score(crossShaped2, crossShaped3, isBlack);
	chessScore[line][column + 1] += priority_score(crossShaped2, crossShaped3, isBlack);
	} else if (cross[line][column] == noChess &&
	cross[line - 1][column] == isBlack && cross[line + 1][column] == noChess
	&& cross[line][column - 1] == noChess && cross[line][column + 1] == isBlack) {
	chessScore[line + 1][column] += priority_score(crossShaped2, crossShaped3, isBlack);
	chessScore[line][column - 1] += priority_score(crossShaped2, crossShaped3, isBlack);
	} else if (cross[line][column] == noChess &&
	cross[line - 1][column] == isBlack && cross[line + 1][column] == noChess
	&& cross[line][column - 1] == isBlack && cross[line][column + 1] == noChess) {
	chessScore[line + 1][column] += priority_score(crossShaped2, crossShaped3, isBlack);
	chessScore[line][column + 1] += priority_score(crossShaped2, crossShaped3, isBlack);
	} else if (cross[line][column] == noChess &&
	cross[line - 1][column] == isBlack && cross[line + 1][column] == isBlack
	&& cross[line][column - 1] == noChess && cross[line][column + 1] == noChess) {
	chessScore[line][column - 1] += priority_score(crossShaped2, crossShaped3, isBlack);
	chessScore[line][column + 1] += priority_score(crossShaped2, crossShaped3, isBlack);
	}
	*/
	// ȱ���γ�ʮ��Χɱ
	// ----------------------������Ҫ������ȼ���Ӧ�����Բ�����Ϊ�ײ�δ��׼
	// ������û�������˰ɣ��о�
}



