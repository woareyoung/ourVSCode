#include "../stdafx.h"
#include "../AI1_Header/AI1.h"
#include "../AI1_Header/Arithmatic.h"

///初始化数据
AI1::AI1()
{
	///初始化特别点分值
	PointStyle[1] = -3;//死棋点设置为-3分
	PointStyle[2] = 13;//填充对方虎口，并向外跳级降低
	PointStyle[17] = 7;//填充对方缺口，并向外跳级降低
	PointStyle[13] = -2;//填充己方的眼设置为-2分
	PointStyle[3] = 5;//形成己方虎口，并向外逐级降低
	PointStyle[4] = 7;//形成己方眼，并向外逐级降低
	PointStyle[5] = 2;//形成己方缺口，并向外逐级降低
	PointStyle[6] = 5;//阻止对方形成虎口，并向外跳级降低
	PointStyle[7] = 8;//阻止对方形成眼，并向外逐级降低
	PointStyle[8] = 2;//阻止对方形成缺口、填充对方单点
	PointStyle[14] = -1;//填充己方虎口设置为-1分
	PointStyle[15] = 1;//填充己方单点，并向外逐级降低
	PointStyle[16] = 1;//填充己方缺口
	PointStyle[9] = -2;//对方下了会死的位置设置为-2分
	PointStyle[10] = -5;//有棋子的位置的分值设置为-5分

	PointStyle[11] = 5;//在对方剩下一个位置时己方还差1子集群围杀
	PointStyle[12] = 6;//在对方剩下一个位置时己方还差2子集群围杀
	///初始化层差分
	RivalSinglePointPoor = 1;//对方单点
	RivalChipPoor = 1;//对方缺口
	RivalTigerMouthPoor = 2.9;//对方虎口

	RivalFormatChipPoor = 1.6;//对方形成缺口
	RivalFormatTigerMouthPoor = 3.2;//对方形成虎口
	RivalFormatEyePoor = 3;//对方形成眼

	MySinglePointPoor = 2;//己方单点
	MyChipPoor = 100;//己方缺口
	MyTigerMouthPoor = 100;//己方虎口

	MyFormatChipPoor = 1.7;//己方形成缺口
	MyFormatTigerMouthPoor = 3;//己方形成虎口
	MyFormatEyePoor = 3;//己方形成眼

	BoundryRate = 0.25;//预测界限
	MaxStepNumber = 15;//构建回归方程的出发条件
	CorrectRate = 0.15;//进攻性能修正系数
	
	MaxParallel = 3;//每一步预测5种可能性
	MaxNext = 3;//设置树最大深度为15层

	StepHead = NULL;
	HeadChain = NULL;
}
///初始化数据
void AI1::InitializeD()
{
	ClearLimb(0, 0);
	a = 0;//初始化常数项
	b = 0;//初始化斜率
	int i, j;
	///初始化数组
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
	AttackPerformance = 1;//初始化对手进攻性能
	///清空链表
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
