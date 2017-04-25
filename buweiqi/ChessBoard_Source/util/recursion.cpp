#include "../../stdafx.h"
#include "../../ChessBoard_Header/Chess.h"

/**
* [Chess::reduceRecursionTimes 减少递归的循环次数]
* 说明：Cross[i][0]为状态位，当为true的时候表示该行有棋子被遍历过，反之，没有
*/
void Chess::reduceRecursionTimes()
{
	for (int i = 0; i < 10; i++)
	{
		if (Cross[i][0] == true)
			for (int j = 0; j < 10; j++)
				Cross[i][j] = false;
	}
}

/**
* [Chess::setStatus 设置当前]
* @param RivalLine   [description]
* @param RivalColumn [description]
*/
void Chess::setStatus(int RivalLine, int RivalColumn) {
	Cross[RivalLine][RivalColumn] = true;
	Cross[RivalLine][0] = true;//下标为0的那一行如果为true则初始化，否则不用初始化
}
