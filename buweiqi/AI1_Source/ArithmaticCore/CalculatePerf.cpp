#include "../../stdafx.h"
#include "../../AI1_Header/AI1.h"
///��¼����
void AI1::CalculatePerf(int line, int column)
{
	GetMaxScorePosition();//��ȡ��������ֵ
	RecordStep(line, column);//��¼������������
	if (StepNumber < MaxStepNumber)
		AttackPerformance = Score[line][column] / MaxScore * CorrectRate + AttackPerformance * (1 - CorrectRate);//������������
	//ÿ5�����¹���һ�λع鷽��
	else if ((StepNumber - MaxStepNumber) % 5 == 0) BulidRegressionEquation();
	///���ݻع鷽��ȷ����������
	if (StepNumber > MaxStepNumber - 1) AttackPerformance = (StepNumber + 1) * b + a;
	Forecast(1);
}
