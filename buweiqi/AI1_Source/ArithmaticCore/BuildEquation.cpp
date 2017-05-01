#include "../../stdafx.h"
#include "../../AI1_Header/AI1.h"

///����ع鷽��
void AI1::BulidRegressionEquation()
{
	CalculateX();
	CalculateY();
	CalculateB();
	CalculateA();
}
///����xƽ��ֵ
void AI1::CalculateX()
{
	_x = SSS->Number / 2;
}
///����yƽ��ֵ
void AI1::CalculateY()
{
	STEP *s = StepHead;
	_y = 0;
	while (s->ScoreRate != 0)
	{
		_y += s->ScoreRate;
		s = s->next;
	}
	_y = _y / (SSS->Number - 1);
}
///����ع鷽�̳�����
void AI1::CalculateA()
{
	a = _y - b * _x;
}
///����ع鷽��б��
void AI1::CalculateB()
{
	STEP *s = StepHead;
	double Parent = 0;//��ĸ
	double Son = 0;//����
	while (s->ScoreRate != 0)
	{
		Parent += (s->Number - _x) * (s->ScoreRate - _y);
		Son += (s->Number - _x) * (s->Number - _x);
		s = s->next;
	}
	b = Parent / Son;
}
