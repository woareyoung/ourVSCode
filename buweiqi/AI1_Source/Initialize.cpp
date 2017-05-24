#include "../stdafx.h"
#include "../AI1_Header/AI1.h"
#include "../AI1_Header/Arithmatic.h"

///��ʼ������
AI1::AI1()
{
	///��ʼ���ر���ֵ
	PointStyle[1] = -3;//���������Ϊ-3��
	PointStyle[2] = 13;//���Է����ڣ���������������
	PointStyle[17] = 7;//���Է�ȱ�ڣ���������������
	PointStyle[13] = -2;//��伺����������Ϊ-2��
	PointStyle[3] = 5;//�γɼ������ڣ��������𼶽���
	PointStyle[4] = 7;//�γɼ����ۣ��������𼶽���
	PointStyle[5] = 2;//�γɼ���ȱ�ڣ��������𼶽���
	PointStyle[6] = 5;//��ֹ�Է��γɻ��ڣ���������������
	PointStyle[7] = 8;//��ֹ�Է��γ��ۣ��������𼶽���
	PointStyle[8] = 2;//��ֹ�Է��γ�ȱ�ڡ����Է�����
	PointStyle[14] = -1;//��伺����������Ϊ-1��
	PointStyle[15] = 1;//��伺�����㣬�������𼶽���
	PointStyle[16] = 1;//��伺��ȱ��
	PointStyle[9] = -2;//�Է����˻�����λ������Ϊ-2��
	PointStyle[10] = -5;//�����ӵ�λ�õķ�ֵ����Ϊ-5��

	PointStyle[11] = 5;//�ڶԷ�ʣ��һ��λ��ʱ��������1�Ӽ�ȺΧɱ
	PointStyle[12] = 6;//�ڶԷ�ʣ��һ��λ��ʱ��������2�Ӽ�ȺΧɱ
	///��ʼ������
	RivalSinglePointPoor = 1;//�Է�����
	RivalChipPoor = 1;//�Է�ȱ��
	RivalTigerMouthPoor = 2.9;//�Է�����

	RivalFormatChipPoor = 1.6;//�Է��γ�ȱ��
	RivalFormatTigerMouthPoor = 3.2;//�Է��γɻ���
	RivalFormatEyePoor = 3;//�Է��γ���

	MySinglePointPoor = 2;//��������
	MyChipPoor = 100;//����ȱ��
	MyTigerMouthPoor = 100;//��������

	MyFormatChipPoor = 1.7;//�����γ�ȱ��
	MyFormatTigerMouthPoor = 3;//�����γɻ���
	MyFormatEyePoor = 3;//�����γ���

	BoundryRate = 0.25;//Ԥ�����
	MaxStepNumber = 15;//�����ع鷽�̵ĳ�������
	CorrectRate = 0.15;//������������ϵ��
	
	MaxParallel = 3;//ÿһ��Ԥ��5�ֿ�����
	MaxNext = 3;//������������Ϊ15��

	StepHead = NULL;
	HeadChain = NULL;
}
///��ʼ������
void AI1::InitializeD()
{
	ClearLimb(0, 0);
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
	Score[1][1] = 2;
	for (i = 1; i < 3; i++)
	{
		spa[i].Chip = 0;
		spa[i].TigerMouth = 0;
		spa[i].Eye = 0;
	}
	AttackPerformance = 1;//��ʼ�����ֽ�������
	///�������
	if (StepHead != NULL)
	{
		STEP *p;
		p = StepHead->next;
		p->pre = NULL;
		delete StepHead;
		StepHead = NULL;
		while (p != NULL)
		{
			p->pre = NULL;
			StepHead = p;
			p = StepHead->next;
			delete StepHead;
			StepHead = NULL;
		}
		StepHead = NULL;
	}
	if(StepHead == NULL)
	{
		StepHead = new STEP;
		StepHead->Number = 1;
		StepHead->ScoreRate = 0;
		StepHead->next = NULL;
		StepHead->pre = NULL;
		SSS = StepHead;
	}
}
