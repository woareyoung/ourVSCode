#include "../stdafx.h"
#include "../AI1_Header/AI1.h"
#include "../AI1_Header/Arithmatic.h"

///��ʼ������
AI1::AI1()
{
	///��ʼ���ر���ֵ
	PointStyle[1] = -3;//���������Ϊ-3��
	PointStyle[2] = 8;//���Է����ڣ���������������
	PointStyle[17] = 5;//���Է�ȱ�ڣ���������������
	PointStyle[13] = -2;//��伺����������Ϊ-2��
	PointStyle[3] = 4;//�γɼ������ڣ��������𼶽���
	PointStyle[4] = 5;//�γɼ����ۣ��������𼶽���
	PointStyle[5] = 3;//�γɼ���ȱ�ڣ��������𼶽���
	PointStyle[6] = 4;//��ֹ�Է��γɻ��ڣ���������������
	PointStyle[7] = 5;//��ֹ�Է��γ��ۣ��������𼶽���
	PointStyle[8] = 3;//��ֹ�Է��γ�ȱ�ڡ����Է�����
	PointStyle[14] = -1;//��伺����������Ϊ-1��
	PointStyle[15] = 2;//��伺�����㣬�������𼶽���
	PointStyle[16] = 1;//��伺��ȱ��
	PointStyle[9] = -2;//�Է����˻�����λ������Ϊ-2��
	PointStyle[10] = -5;//�����ӵ�λ�õķ�ֵ����Ϊ-5��

	PointStyle[11] = 5;//�ڶԷ�ʣ��һ��λ��ʱ��������1�Ӽ�ȺΧɱ
	PointStyle[12] = 6;//�ڶԷ�ʣ��һ��λ��ʱ��������2�Ӽ�ȺΧɱ
					   ///��ʼ������
	RivalSinglePointPoor = 1;//�Է�����
	RivalChipPoor = 1;//�Է�ȱ��
	RivalTigerMouthPoor = 2.9;//�Է�����

	RivalFormatChipPoor = 1;//�Է��γ�ȱ��
	RivalFormatTigerMouthPoor = 1;//�Է��γɻ���
	RivalFormatEyePoor = 1;//�Է��γ���

	MySinglePointPoor = 1;//��������
	MyChipPoor = 100;//����ȱ��
	MyTigerMouthPoor = 100;//��������

	MyFormatChipPoor = 2;//�����γ�ȱ��
	MyFormatTigerMouthPoor = 1;//�����γɻ���
	MyFormatEyePoor = 1;//�����γ���

	BoundryRate = 0.3;//Ԥ�����
	MaxStepNumber = 15;//�����ع鷽�̵ĳ�������
	CorrectRate = 0.15;//������������ϵ��

	Step = new STEP;
	InitializeD();
}
///��ʼ������
void AI1::InitializeD()
{
	a = 0;//��ʼ��������
	b = 0;//��ʼ��б��
	int i, j;
	///��ʼ������
	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 10; j++)
		{
			Score[i][j] = 1;
			cross[i][j] = 0.0;
			Cross[i][j] = false;
		}
	}
	Score[0][0] = -3;
	for (i = 1; i < 3; i++)
	{
		spa[i].Chip = 0;
		spa[i].TigerMouth = 0;
		spa[i].Eye = 0;
	}
	AttackPerformance = 1;//��ʼ�����ֽ�������
	///�������
	if (Step->Number > 0)
	{
		STEP *p = Step->next;
		delete Step;
		while (p != NULL)
		{
			delete Step;
			Step = p;
			p = Step->next;
		}
	}
	if (Step == NULL) Step = new STEP;
	Step->Number = 1;
	Step->ScoreRate = 0;
	Step->next = NULL;
	SSS = Step;
}
