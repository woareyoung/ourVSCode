#include "../../stdafx.h"
#include "../../AI2_Header/AI2.h"

/**
* [AI2::AcrossCorners �߽�ɨ��]
* ʵ���߼���ͨ��ɨ��߽ǵ�ʵ�ַ����ӷ�
* @return [��]
*/
void AI2::AcrossCorners()
{
	// �ֵ�������ʱ��
	for (int i = 0; i < 12; i += 3) {
		ACScan(cornerArray[i] / 100, cornerArray[i] % 100,
			cornerArray[i + 1] / 100, cornerArray[i + 1] % 100,
			cornerArray[i + 2] / 100, cornerArray[i + 2] % 100);
	}
}

/**
* [AI2::ACScan ɨ�躯���������жϱ߽ǣ���߱ߣ��Ƿ�������]
* @param line    [�߽����ӵ���]
* @param column  [�߽����ӵ���]
* @param line1   [�߽�������Χ�����ӵ���]
* @param column1 [�߽�������Χ�����ӵ���]
* @param line2   [�߽�������Χ�����ӵ���]
* @param column2 [�߽�������Χ�����ӵ���]
*/
void AI2::ACScan(int line, int column, int line1, int column1, int line2, int column2)
{
	if (cross[line][column] == noChess) {
		// �߽��Ѿ��γɰ�ΧȦ�����
		if (cross[line1][column1] == Rival && cross[line2][column2] == Rival) {
			chessScore[line][column] = minLimit;
		}
		// �߽�����λ�õ��ӵ����
		else if (cross[line1][column1] == noChess && cross[line2][column2] == isWhite) {
			chessScore[line1][column1] += cornerScoreFull;
		}
		else if (cross[line1][column1] == isWhite && cross[line2][column2] == noChess) {
			chessScore[line2][column2] += cornerScoreFull;
		}
		else if (cross[line1][column1] == isBlack && cross[line2][column2] == noChess) {
			chessScore[line2][column2] += cornerScoreFull;
		}
		else if (cross[line1][column1] == noChess && cross[line2][column2] == isBlack) {
			chessScore[line1][column1] += cornerScoreFull;
		}
		// �߽ǹ��ɵ�����������һ�����Ӷ�û�У���������Ļ�����Ҫ�ӷ�
		else if (cross[line1][column1] == noChess && cross[line2][column2] == noChess) {
			chessScore[line1][column1] += cornerScore;
			chessScore[line2][column2] += cornerScore;
		}

	}// ע�⣺�߽�����������
}

