#include "../stdafx.h"
#include "../AI1_Header/AI1.h"

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
}
///��ʼ������
void AI1::InitializeD()
{
	int i, j;
	///��ʼ������
	for (i = 0; i < 10; ++i)
	{
		for (j = 0; j < 10; ++j)
		{
			Score[i][j] = 1;
			cross[i][j] = 0.0;
			Cross[i][j] = false;
		}
	}
	Score[0][0] = PointStyle[1];
	Score[1][1] = 2;
	Score[1][2] = 2;
	Score[2][1] = 2;
	Score[1][8] = 2;
	Score[1][9] = 2;
	Score[2][9] = 2;
	Score[8][1] = 2;
	Score[9][1] = 2;
	Score[9][2] = 2;
	Score[8][9] = 2;
	Score[9][9] = 2;

	Qua.FirstQuadrant = 0;
	Qua.SecondQuadrant = 0;
	Qua.ThirdQuadrant = 0;
	Qua.ForthQuadrant = 0;
}
