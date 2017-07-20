#include "../stdafx.h"
#include "../AI2_Header/AI2.h"
#include "../ChessBoard_Header/showUnicodeInfo.h"

/**
* [AI2::GetPosition AI����ӿ�]
* @param line 			����λ�õ�������
* @param column 		����λ�õ�������
* @param onTurn 		��һ���з���ID
* @param isExist[10][10]		���������������
*/
void AI2::GetPosition(int &line, int &column, int onTurn)
{
	//���յ������Զ������
	if (onTurn != 1 && onTurn != -1 && onTurn != 2 && onTurn != -2)
	{
		//������Ӧ�����ڶ�AI�ļ��
		if (onTurn == 0)
		{
			// ���������¿�ʼ��Ϸ���������ù���
			line++;
			column++;
			resetGo2DeadStatus();
			initAllArray();
		}
		//����һ������������ݽ��л��ˡ���100Ϊ���1��200Ϊ���2��line��columnΪ���˵�λ�ã�
		else if (onTurn == 100 || onTurn == 200)
		{
			rollback(line, column, onTurn / 100);
		}
		return;
	}
	/******************************************
	��ӵз����ӵ㵽�ҷ�������
	*******************************************/
	this->chessCount++;
	// ע�⴫�ݽ�����onTurn�����ǶԷ��ģ������Ǽ����ġ�
	initPlayerRole(onTurn);
	cross[line][column] = Rival;

	/******************************************
	�ҷ�����
	*******************************************/
	ScanChessBroad();
	// ���ñ��������
	int temp = maxandmin(1);
	line = getLine(temp);
	column = getColumn(temp);
	cross[line][column] = turn2Who;
	// ���������λ�õķ�����Ϊ��Сֵ
	
	/*_cprintf("\n**************This is chess score*******(%d, %d)***********\n", line, column);
	for (int i = ChessStart; i < ChessEnd; ++i)
	{
		for (int j = 1; j < 10; ++j)
			if (CS[i][j] >= 20) {
				_cprintf("%d\t", CS[i][j]);
			}
			else if (CS[i][j] == minLimit) {
				_cprintf("M\t");
			}
			else if (CS[i][j] == 0) {
				_cprintf("0\t");
			}
			else {
				_cprintf(".\t");
			}
			_cprintf("\n");
	}
	_cprintf("**************This is chess cross*******(%d, %d)***********\n", line, column);
	showChessBoard(cross);*/
}
