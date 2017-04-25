#include "../stdafx.h"
#include "../AI1_Header/AI1.h"

///应对“异常”状态的重设分
void AI1::RateResetScore(float ResetRate)
{
	int i, j;
	for (i = 1; i < 10; i++)
	{
		for (j = 1; j < 10; j++)
		{
			if (Score[i][j] > 1) Score[i][j] *= ResetRate;
		}
	}
}
