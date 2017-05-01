#include "../../stdafx.h"
#include "../../AI1_Header/AI1.h"

///构造回归方程
void AI1::BulidRegressionEquation()
{
	CalculateX();
	CalculateY();
	CalculateB();
	CalculateA();
}
///计算x平均值
void AI1::CalculateX()
{
	_x = SSS->Number / 2;
}
///计算y平均值
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
///计算回归方程常数项
void AI1::CalculateA()
{
	a = _y - b * _x;
}
///计算回归方程斜率
void AI1::CalculateB()
{
	STEP *s = StepHead;
	double Parent = 0;//分母
	double Son = 0;//分子
	while (s->ScoreRate != 0)
	{
		Parent += (s->Number - _x) * (s->ScoreRate - _y);
		Son += (s->Number - _x) * (s->Number - _x);
		s = s->next;
	}
	b = Parent / Son;
}
