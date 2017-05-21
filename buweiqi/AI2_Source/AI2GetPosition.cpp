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
	//���յ������Զ������
	if (onTurn != 1 && onTurn != 2)
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

		}
		return;
	}
	/******************************************
	��ӵз����ӵ㵽�ҷ�������
	*******************************************/
	this->chessCount++;
	// ע�⴫�ݽ�����onTurn�����ǶԷ��ģ������Ǽ����ġ�
	Rival = (onTurn == Black || onTurn == isAI1onTurn) ? Black : White;
	this->turn2Who = getRival(Rival);
	this->PlayerId = turn2Who;
	PlayerId = turn2Who;
	cross[line][column] = Rival;

	/******************************************
	�ҷ�����
	*******************************************/
	ScanChessBroad();
	// ���ñ��������
	int temp = maxandmin(1);
	line = temp / 100;
	column = temp % 100;
	cross[line][column] = turn2Who;
	// ���������λ�õķ�����Ϊ��Сֵ
/*
	_cprintf("**************This is chess score*******(%d, %d)***********\n", line, column);
	for (int i = 1; i < 10; i++)
	{
		for (int j = 1; j < 10; j++)
			_cprintf("%d\t", chessScore[i][j]);
		_cprintf("\n");
	}
	_cprintf("**************This is chess cross*******(%d, %d)***********\n", line, column);
	for (int i = 1; i < 10; i++)
	{
		for (int j = 1; j < 10; j++)
			_cprintf("%d\t", cross[i][j]);
		_cprintf("\n");
	}
*/
}
