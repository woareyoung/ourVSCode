#include "../stdafx.h"
#include "../AI2_Header/AI2.h"

/**
* [AI2::GetPosition AI����ӿ�]
* @param line 			����λ�õ�������
* @param column 		����λ�õ�������
* @param onTurn 		��һ���з���ID
* @param isExist[10][10]		���������������
*/
void AI2::GetPosition(int& line, int& column, int onTurn)
{
	//������Ӧ�����ڶ�AI�ļ��
	if (onTurn == 0)
	{
		// ���������¿�ʼ��Ϸ���������ù���
		line++;
		column++;
		resetGo2DeadStatus();
		initAllArray();
		return;
	}
	this->chessCount++;
	// ע�⴫�ݽ�����onTurn�����ǶԷ��ģ������Ǽ����ġ�
	Rival = (onTurn == isBlack || onTurn == isAI1onTurn) ? isBlack : isWhite;
	turn2Who = (Rival == isWhite ? isBlack : isWhite);
	PlayerId = turn2Who;
	cross[line][column] = Rival;
	chessScore[line][column] = minLimit;

	// ���ñ��������
	int temp = maxandmin(1);
	line = temp / 100;
	column = temp % 100;
	// ���������λ�õķ�����Ϊ��Сֵ
	chessScore[line][column] = minLimit;
	cross[line][column] = turn2Who;
	this->chessCount++;

	_cprintf("**************This is chess score*******(%d, %d)***********\n", line, column);
	for (int i = 1; i < 10; i++)
	{
		for (int j = 1; j < 10; j++)
			_cprintf("%d\t", chessScore[i][j]);
		_cprintf("\n");
	}
}
