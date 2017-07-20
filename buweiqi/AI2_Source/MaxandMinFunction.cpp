#include "../AI2_Header/AI2.h"
/**
* [AI2::maxandmin ����С����]
* @param depth 		ִ�е����
*/
int AI2::maxandmin(const int& depth)
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
	for (int i = 0; i < depth; ++i)
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
	/*if (!goodMoves.empty()) {
		std::vector<goodMove>().swap(goodMoves);
	}*/
	Revalute();
	return DealWithScore(turn2Who == PlayerId);
}

int AI2::DealWithScore(const bool& isEqual) {
	int tempLine = 0;
	int tempColumn = 0;
	/************************************************
	ƥ��ɹ�
	*************************************************/
	this->chessCount++;
	this->getScore(tempLine, tempColumn, isEqual);// ��ȡ���ֵ
	if (tempLine != 0 && tempColumn != 0) {
		goto Find;
	}
	/************************************************
	ƥ�䲻�ɹ���Ҳû�з�����ߵģ�����ѡ��һ��Ѱ����ѵ�
	*************************************************/
	return FindPosition();
Find:
	return tempLine * 100 + tempColumn;
}

#include "../ChessBoard_Header/WinCheck.h"

void AI2::getScore(int& tempLine, int& tempColumn, bool isGetMax)
{
//	if (goodMoves.size() >= 1) {
//		this->arraySort();
//	}
//	else {
//		goto other;
//	}
//	goodMove gm = isGetMax ? goodMoves.front() : goodMoves.back();
//	int size = goodMoves.size();
//	int maxsize = 0;
//
//	int i;
//	for (i = 0; i < size; ++i) {
//		if (goodMoves[i].Score == gm.Score) {
//			++maxsize;
//		}
//	}
//	i = isGetMax ? 0 : size - maxsize;
//	int range = isGetMax ? maxsize : size;
//	for (; i < range; ++i)
//	{
//		tempLine = goodMoves[i].line;
//		tempColumn = goodMoves[i].column;
//		return;
//	}
//other:
	WinCheck::ChessBoardOption option;
	option.black = Black;
	option.white = White;
	option.edge = Edge;
	option.emptyChess = NoChess;
	WinCheck::ChessInfo chessInfo(option);
	bool isFirst = true;
	for (int i = 1; i < 10; ++i)
	{
		for (int j = 1; j < 10; ++j)
		{
			// ������Ҫ�޸�
			if (chessScore[i][j] == minLimit || cross[i][j] != NoChess) continue;
			if (chessScore[i][j] != 0 && chessInfo.WinOrLoseCheck(i, j, turn2Who,cross)) continue;
			if (isFirst)
			{
				tempLine = i;
				tempColumn = j;
				isFirst = false;
			}
			else if (!isFirst)
			{
				if (isGetMax) {
					if (chessScore[tempLine][tempColumn] < chessScore[i][j]) {
						tempLine = i;
						tempColumn = j;
					}
				}
				else {
					if (chessScore[tempLine][tempColumn] > chessScore[i][j]) {
						tempLine = i;
						tempColumn = j;
					}
				}
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
	register int i;
	register int j;
	bool Notloop = false;//�Ƿ�Ͽ�ѭ��
	//ɨ�����̣��ж��Ƿ�ֻ������
	for (i = 1; i < 10; ++i)
	{
		for (j = 1; j < 10; ++j)
		{
			//������ǿ�λ����ɨ����һ��λ��
			if (cross[i][j] != NoChess) continue;
			//����������壬��ζ����ֻ�û����������ֹѭ��
			if (chessScore[i][j] != minLimit)
			{
				Notloop = true;
				break;
			}
			//���ɨ�赽һ������λ��
			else
			{
				x = i;
				y = j;
			}
		}
		if (Notloop) break;
	}
	//���������ֻʣ�����壬�򷵻�һ����λ
	if (i == 10 && j == 10)
	{
		return x * 100 + y;
	}
	WinCheck::ChessBoardOption option;
	option.black = Black;
	option.white = White;
	option.edge = Edge;
	option.emptyChess = NoChess;
	WinCheck::ChessInfo chessInfo(option);
	for (i = 1; i < 10; ++i) {
		for (j = 1; j < 10; ++j) {
			x = i;
			y = j;
			if (cross[x][y] != NoChess || chessScore[x][y] == minLimit) continue;
			// ���ڵ�ǰƥ�䵽�����ӵ�Ļ������з���
			// ��ʱ���õ�ǰ��õ�λ��Ϊ�ҷ����ӵ㣬�ж��Ƿ����ҷ�����ɱ��
			if (chessInfo.WinOrLoseCheck(x, y, turn2Who, cross)) {
				chessScore[x][y] = minLimit;
				// ������ҷ�����ɱ��Ļ�����ֱ����ת�������ж��Ƿ��ǵз�����ɱ���ˡ�
				continue;
			}
			if (isFinal()) continue;
			// ��ʱ���õ�ǰ��õ�λ��Ϊ�ҷ����ӵ㣬�ж��Ƿ��ǵз�����ɱ��
			if (chessScore[x][y] == 0) continue;
			if (chessInfo.WinOrLoseCheck(x, y, turn2Who, cross)) {
				chessScore[x][y] = 0;
				continue;
			}
			// ����ʲô��û��ƥ�䵽�����Խ�������
		}
	}
	return x * 100 + y;
}
