#include "../stdafx.h"
#include "../AI1_Header/AI1.h"

///Ӧ�ԡ��쳣��״̬�������
void AI1::RateResetScore(double ResetRate, bool add)
{
	int i, j;
	if (add)
	{
		for (i = 1; i < 10; ++i)
		{
			for (j = 1; j < 10; ++j)
			{
				if (Score[i][j] > 1) Score[i][j] *= ResetRate;
			}
		}
	}
	else
	{
		for (i = 1; i < 10; ++i)
		{
			for (j = 1; j < 10; ++j)
			{
				if (Score[i][j] > 1) Score[i][j] /= ResetRate;
			}
		}
	}
}
