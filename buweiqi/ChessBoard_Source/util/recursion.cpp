#include "../../stdafx.h"
#include "../../ChessBoard_Header/Chess.h"

/**
* [Chess::reduceRecursionTimes ���ٵݹ��ѭ������]
* ˵����Cross[i][0]Ϊ״̬λ����Ϊtrue��ʱ���ʾ���������ӱ�����������֮��û��
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
* [Chess::setStatus ���õ�ǰ]
* @param RivalLine   [description]
* @param RivalColumn [description]
*/
void Chess::setStatus(int RivalLine, int RivalColumn) {
	Cross[RivalLine][RivalColumn] = true;
	Cross[RivalLine][0] = true;//�±�Ϊ0����һ�����Ϊtrue���ʼ���������ó�ʼ��
}
