#include "../stdafx.h"
#include "../ChessBoard_Header/ChessBoard.h"
#include "../ChessBoard_Header/Chess.h"
#include "../AI1_Header/AI1.h"
#include "../AI2_Header/AI2.h"
#include "../AI3_Header/AI3.h"
#include <memory>
#include <iostream>

ChessBoard::ChessBoard()
{
	RootWidth = 1366 * 800 / GetSystemMetrics(SM_CXSCREEN);//��ȡ��Ļ��Ȳ����ô�����
	RootHeight = 768 * 550 / GetSystemMetrics(SM_CYSCREEN);//��ȡ��Ļ�߶Ȳ����ô���߶�
	Base = RootWidth > RootHeight ? RootHeight : RootWidth;//���ô������ݹ�ģ����Ӧ��ͬƽ̨
	ChessDiameter = Base / 10;//��������ֱ����Ҳ��������ÿ�����ӵı߳�
	///Ĭ��û��ʹ��AI
	Player1isAI = false;
	Player2isAI = false;
	ControlLoop = true;
	line = 0;
	column = 0;
	Start = false;//��ʾδ��ʼ��Ϸ
	AllTime1 = 900;//��ʱ������λ��S��
	AllTime2 = 900;
	Round1 = 60;//��ʱ����λ��s��
	Round2 = 60;
	onTurn = 1;//�ֵ�˭
	Winner = 0;//ʤ����
	PrintTime = true;//�Ƿ���Ҫ���ʱ��
	Repaint = true;
	///�ṹ�壬��������
	rect.left = 0;
	rect.top = 0;
	rect.right = Base;
	rect.bottom = Base;
	///������ɫ��ˢ������
	int i;
	//����ÿ������������ֵ
	for (i = 1; i < 10; ++i)
	{
		CrossCross[i] = i * ChessDiameter;
	}
	Init_cross();
	///Ĭ��AI����ѡ
	CanSelectAI1 = false;
	CanSelectAI2 = false;
	CanSelectAI3 = false;
	///���AI�Ƿ����ʹ��
	//ʹ��0��������
	ai1.GetPosition(line, column, 0);
	if (line != 0) CanSelectAI1 = true;
	line = 0;
	ai2.GetPosition(line, column, 0);
	if (line != 0) CanSelectAI2 = true;
	line = 0;
	ai3.GetPosition(line, column, 0);
	if (line != 0) CanSelectAI3 = true;
	line = 0;
	column = 0;
	Player1AI = nullptr;
	Player2AI = nullptr;
}
///��ʼ������״̬����
void ChessBoard::Init_cross()
{
	CurrentRoundNumber = 0;
	int i, j;
	for (i = 0; i < 10; ++i) {
		for (j = 0; j < 10; ++j) {
			cross[i][j] = 0;
		}
	}
}
///��ʼ�������¼
void ChessBoard::Init_Pace()
{
	MemPace.clear();
	MemBattle.clear();
	PACE pac;
	pac.line = 0;
	pac.column = 0;
	MemPace.push_back(pac);
	/*
	ChessAmount[1] = 0;
	ChessAmount[2] = 0;
	ChessAmount[0] = 0;
	*/
}