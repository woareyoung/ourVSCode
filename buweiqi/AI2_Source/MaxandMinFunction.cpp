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
			Rival = (tempArrayW[i] == isWhite ? isBlack : isWhite);
		}
		else if (PlayerId == Black) {
			turn2Who = tempArrayB[i];
			Rival = (tempArrayB[i] == isWhite ? isBlack : isWhite);
		}
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
			if (chessScore[i][j] == minLimit || cross[i][j] != NoChess)
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

void AI2::getMinScore(int& tempLine, int& tempColumn)
{
	bool isFirst = true;
	for (int i = 1; i < 10; i++)
	{
		for (int j = 1; j < 10; j++)
		{
			// ������Ҫ�޸�
			if (chessScore[i][j] == minLimit || cross[i][j] != NoChess)
				continue;
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
	initChessScore();
	// ��ֵ���ӷ�
//	chessStatusShaped();// ʮ��Χɱ
//	AcrossCorners();// �߽�Χɱ
//	Tirangle();// ����Χɱ

	startPattern();
	// �������ģ��ƥ��
}

int AI2::FindPosition() {
	int x = 0;
	int y = 0;

	for (int i = 1; i < 10; i++) {
		for (int j = 1; j < 1; j++) {
			if (chessScore[i][j] == minLimit) 
				continue;
			// ���ڵ�ǰƥ�䵽�����ӵ�Ļ������з���
			// ��ʱ���õ�ǰ��õ�λ��Ϊ�ҷ����ӵ㣬�ж��Ƿ����ҷ�����ɱ��
			cross[x][y] = turn2Who;
			if (isGo2Dead(x, y, turn2Who)) {
				chessScore[x][y] = minLimit;
				cross[x][y] = NoChess;
				// ������ҷ�����ɱ��Ļ�����ֱ����ת�������ж��Ƿ��ǵз�����ɱ���ˡ�
				goto Pos;
			}
			// ��ʱ���õ�ǰ��õ�λ��Ϊ�ҷ����ӵ㣬�ж��Ƿ��ǵз�����ɱ��
			cross[x][y] = Rival;
			if (isGo2Dead(x, y, Rival)) {
				cross[x][y] = NoChess;
				// ����ǵз�����ɱ��Ļ�������Ͳ����мӷִ�����   -.-������
				goto Pos;
			}
			// ����ʲô��û��ƥ�䵽�����Խ�������
			cross[x][y] = NoChess;
		}
	}
Pos:
	return x * 100 + y;
}
