#include "../stdafx.h"
#include "../AI2_Header/AI2.h"
/**
* [AI2::maxandmin ����С����]
* @param depth 		ִ�е����
*/
int AI2::maxandmin(int depth)
{
	int tempArrayW[10] =
	{
		2,1,2,1,2,1,2,1,2,1
	};
	int tempArrayB[10] =
	{
		1,2,1,2,1,2,1,2,1,2
	};
	int temp;
	for (int i = 0; i < depth; i++)
	{
		if (PlayerId == White) {
			turn2Who = tempArrayW[i];
		}
		else if (PlayerId == Black) {
			turn2Who = tempArrayB[i];
		}
		Rival = getRival(turn2Who);
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
	return DealWithScore(turn2Who == PlayerId);
}

int AI2::DealWithScore(bool isEqual) {
	int tempLine = 0;
	int tempColumn = 0;
	/************************************************
	ƥ��ɹ�
	*************************************************/
	this->chessCount++;
	if (isEqual) {
		getMaxScore(tempLine, tempColumn);// ��ȡ���ֵ
	}
	else {
		getMinScore(tempLine, tempColumn);// ��ȡ��Сֵ
	}
	if (tempLine != 0 && tempColumn != 0) {
		goto Find;
	}
	/************************************************
	ƥ�䲻�ɹ�������ѡ��һ��Ѱ����ѵ�
	*************************************************/
	return FindPosition();
Find:
	return tempLine * 100 + tempColumn;
}

void AI2::getMaxScore(int& tempLine, int& tempColumn)
{
	bool isFirst = true;
	for (int i = 1; i < 10; i++)
	{
		for (int j = 1; j < 10; j++)
		{
			// ������Ҫ�޸�
			if (chessScore[i][j] == minLimit || cross[i][j] != NoChess) continue;
			if (isGo2Dead(i, j, turn2Who)) continue;
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

void AI2::getMinScore(int& tempLine, int& tempColumn)
{
	bool isFirst = true;
	for (int i = 1; i < 10; i++)
	{
		for (int j = 1; j < 10; j++)
		{
			// ������Ҫ�޸�
			if (chessScore[i][j] == minLimit || cross[i][j] != NoChess) continue;
			if (isGo2Dead(i, j, turn2Who)) continue;
			if (isFirst)
			{
				tempLine = i;
				tempColumn = j;
				isFirst = false;
			}
			else if (!isFirst && chessScore[tempLine][tempColumn] > chessScore[i][j])
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
	if (this->chessCount < 2) {
		initChessScore(true);
	}
	else {
		initChessScore(false);
	}
	
	// ��ֵ���ӷ�
	// �������ģ��ƥ��
	startPattern();
}

int AI2::FindPosition() {
	register int x = 0;
	register int y = 0;

	for (register int i = 1; i < 10; i++) {
		for (register int j = 1; j < 10; j++) {
			x = i;
			y = j;
			if (cross[x][y] != NoChess || chessScore[x][y] == minLimit) continue;
			// ���ڵ�ǰƥ�䵽�����ӵ�Ļ������з���
			// ��ʱ���õ�ǰ��õ�λ��Ϊ�ҷ����ӵ㣬�ж��Ƿ����ҷ�����ɱ��
			cross[x][y] = turn2Who;
			if (isGo2Dead(x, y, turn2Who)) {
				chessScore[x][y] = minLimit;
				// ������ҷ�����ɱ��Ļ�����ֱ����ת�������ж��Ƿ��ǵз�����ɱ���ˡ�
				goto empty;
			}
			if (isFinal()) goto empty;
			// ��ʱ���õ�ǰ��õ�λ��Ϊ�ҷ����ӵ㣬�ж��Ƿ��ǵз�����ɱ��
			if (chessScore[x][y] == 0) goto empty;
			cross[x][y] = Rival;
			if (isGo2Dead(x, y, Rival)) {
				chessScore[x][y] = 0;
				goto empty;
			}
empty:
			// ����ʲô��û��ƥ�䵽�����Խ�������
			cross[x][y] = NoChess;
		}
	}
	return x * 100 + y;
}