#include "../AI4_Header/AI4.h"

void AI4::SetScoreLevel(std::set<std::pair<int, int>> PositionLevel)
{
	for (int i = 1; i < 10; ++i)
	{
		for (int j = 1; j < 10; ++j)
		{
			if (cross[i][j] != 0) continue;//有棋子就跳过
			PositionLevel.insert(std::make_pair(Score[i][j], i * 100 + j));//由于set容器按照pair的第一个元素排序，所以把分数放前面
		}
	}
}