#include "../stdafx.h"
#include "../ChessBoard_Header/ChessBoard.h"

ChessBoard::ChessBoard()
{
	RootWidth = 1366 * 800 / GetSystemMetrics(SM_CXSCREEN);//��ȡ��Ļ��Ȳ����ô�����
	RootHeight = 768 * 550 / GetSystemMetrics(SM_CYSCREEN);//��ȡ��Ļ�߶Ȳ����ô���߶�
	Base = RootWidth > RootHeight ? RootHeight : RootWidth;//���ô������ݹ�ģ����Ӧ��ͬƽ̨
	ChessDiameter = Base / 10;//��������ֱ����Ҳ��������ÿ�����ӵı߳�
	///Ĭ��û��ʹ��AI
	Player1isAI = false;
	Player2isAI = false;
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
					 ///�ṹ�壬��������
	rect.left = 0;
	rect.top = 0;
	rect.right = Base;
	rect.bottom = Base;
	///������ɫ��ˢ������
	int i;
	//����ÿ������������ֵ
	for (i = 1; i < 10; i++)
	{
		CrossCross[i] = i * ChessDiameter;
	}
	Tail == NULL;
	SituaHead = NULL;
	TempTail = NULL;
	Init_cross();
	Init_Pace();
}
///��ʼ������״̬����
void ChessBoard::Init_cross()
{
	int i, j;
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {
			cross[i][j] = 0;
		}
	}
}
///��ʼ�������¼
void ChessBoard::Init_Pace()
{
	PACE *p = Tail;
	while (p != NULL)
	{
		Tail = Tail->perior;
		delete p;
		p = NULL;
		p = Tail;
	}
	SITUATION *s = SituaHead;
	while (p != NULL)
	{
		SituaHead = SituaHead->next;
		delete s;
		s = NULL;
		s = SituaHead;
	}
}