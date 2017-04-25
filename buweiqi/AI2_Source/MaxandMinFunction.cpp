#include "../stdafx.h"
#include "../AI2_Header/AI2.h"
/**
* [AI2::maxandmin ����С����]
* @param depth 		ִ�е����
*/
int AI2::maxandmin(int depth)
{
	int tempArray[10] =
	{
		2,1,2,1,2,1,2,1,2,1
	};
	int temp;
	for (int i = 0; i < depth; i++)
	{
		turn2Who = tempArray[i];
		Rival = (tempArray[i] == isWhite ? isBlack : isWhite);
		temp = singleLayer();
	}
	return temp;
}

/**
* [AI2::singleLayer ����ִ��]
*/
int AI2::singleLayer()
{
	Revalute();
	if (turn2Who == PlayerId)
	{
		return MaxScore();
	}
	else
	{
		return MinScore();
	}
}

/**
* [AI2::MaxScore ��ȡ����ֵ]
*/
int AI2::MaxScore()
{
	this->chessCount++;
	bool isFirst = true;
	int tempLine = 0;
	int tempColumn = 0;
	int changePoint = 0;
	int RivalKillCount = 0;
	int endCount = 81 - chessCount;
	int minNum = isFinal();
	bool no = minNum == chessCount;
	while (true)
	{
		_cprintf("**************chessStatusShaped******************\n");
		for (int i = 1; i < 10; i++)
		{
			for (int j = 1; j < 10; j++)
			{
				if (chessScore[i][j] < 10 && chessScore[i][j] > 0)
				{
					_cprintf("0\t");
				}
				else
				{
					_cprintf("%d\t", chessScore[i][j]);
				}
			}
			_cprintf("\n");
		}
		_cprintf("**************chessStatusShaped cross******************\n");
		for (int i = 1; i < 10; i++)
		{
			for (int j = 1; j < 10; j++)
				_cprintf("%d\t", cross[i][j]);
			_cprintf("\n");
		}

		getMaxScore(tempLine, tempColumn);// ��ȡ���ֵ
		if (no && changePoint == tempLine * 100 + tempColumn && endCount > 0)
		{
			--endCount;
		}
		changePoint = tempLine * 100 + tempColumn;
		// ��ʱ���õ�ǰ��õ�λ��Ϊ�з����ӵ㣬�ж��Ƿ��ǶԷ�����ɱ��
		cross[tempLine][tempColumn] = Rival;
		if (tempLine != 0 && 0 != tempColumn && isGo2Dead(tempLine, tempColumn, Rival))
		{
			if (no)
			{
				chessScore[tempLine][tempColumn] = 23;
				RivalKillCount++;
				_cprintf("----------(%d, %d)\n", tempLine, tempColumn);
				break;
			}
			else
				chessScore[tempLine][tempColumn] = -23;
			cross[tempLine][tempColumn] = noChess;
			continue;
		}
		// ��ʱ���õ�ǰ��õ�λ��Ϊ�ҷ����ӵ㣬�ж��Ƿ����ҷ�����ɱ��
		cross[tempLine][tempColumn] = turn2Who;
		if (tempLine != 0 && 0 != tempColumn && isGo2Dead(tempLine, tempColumn, turn2Who))
		{
			chessScore[tempLine][tempColumn] = minLimit;
			cross[tempLine][tempColumn] = noChess;
			continue;
		}
		else
		{
			break;
		}
		if (RivalKillCount == 0)
		{
			tempLine = 0;
			tempColumn = 0;
			break;
		}
	}
	return tempLine * 100 + tempColumn;
}

/**
* [AI2::MinScore ��ȡ��Сֵ]
*/
int AI2::MinScore()
{
	bool isFirst = true;
	int tempLine;
	int tempColumn;
	// �������
	for (int i = 1; i < 10; i++)
	{
		for (int j = 1; j < 10; j++)
		{
			if (chessScore[i][j] == minLimit || cross[i][j] != noChess)
			{
				continue;
			}
			if (isFirst)
			{
				tempLine = i;
				tempColumn = j;
				isFirst = false;
			}
			else if (!isFirst && chessScore[tempLine][tempColumn] < chessScore[i][j])
			{
				tempLine = i;
				tempColumn = j;
			}
		}
	}
	return tempLine * 100 + tempColumn;
}

void AI2::getMaxScore(int& tempLine, int& tempColumn)
{
	bool isFirst = true;
	for (int i = 1; i < 10; i++)
	{
		for (int j = 1; j < 10; j++)
		{
			if (chessScore[i][j] == minLimit || cross[i][j] != noChess)
				continue;
			if (isFirst)
			{
				tempLine = i;
				tempColumn = j;
				isFirst = false;
			}
			else if (!isFirst && chessScore[tempLine][tempColumn] < chessScore[i][j])
			{
				tempLine = i;
				tempColumn = j;
			}
		}
	}
}

/**
* [AI2::Revalute ��ֵ����]
*/
void AI2::Revalute()
{
	// ��ʼ�����̵ķ���
	initChessScore();
	// ��ֵ���ӷ�
	chessStatusShaped();// ʮ��Χɱ
	AcrossCorners();// �߽�Χɱ
	Tirangle();// ����Χɱ
}
