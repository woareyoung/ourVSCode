#include "../../stdafx.h"
#include "../../AI1_Header/AI1.h"
///记录数据
void AI1::CalculatePerf(int line, int column)
{
	GetMaxScorePosition();//获取棋盘最大分值
	RecordStep(line, column);//记录对手下棋数据
	if (StepNumber < MaxStepNumber)
		AttackPerformance = Score[line][column] / MaxScore * CorrectRate + AttackPerformance * (1 - CorrectRate);//修正进攻性能
	//每5步重新构造一次回归方程
	else if ((StepNumber - MaxStepNumber) % 5 == 0) BulidRegressionEquation();
	///根据回归方程确定进攻性能
	if (StepNumber > MaxStepNumber - 1) AttackPerformance = (StepNumber + 1) * b + a;
	Forecast(1);
}
