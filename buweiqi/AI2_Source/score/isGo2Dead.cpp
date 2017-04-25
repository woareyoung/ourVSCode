#include "../../stdafx.h"
#include "../../AI2_Header/AI2.h"

/**
* [AI2::isGo2Dead �ж����ӵ��Ƿ�����ɱ����]
*/
void AI2::isGo2Dead(int type)
{
	int stack[81][2] = { 0 }; //�洢ջԪ�أ�0Ϊx,1Ϊy
	int top = 0;// ջ��Ԫ��
	int treated = 0;// �������Ԫ��

					// �������е�״̬λ
	bool tempChess[10][10] = { 0 }; //�����洢��ǰλ�õ����Ƿ񱻴����
	for (int i = 1; i < 10; i++)
	{
		for (int j = 1; j < 10; j++)
		{
			if (cross[i][j] != type || !tempChess[i][j])
			{
				continue;
			}
			tempChess[i][j] = true;
			// ��ʼ��ջ��Ԫ��
			stack[top][0] = i;
			stack[top][1] = j;
			top++;
			// ��ջ��Ԫ�ص�������С�ڴ������Ԫ�ص�������ʱ��ѭ����ֹ
			while (treated < top)
			{
				int x = stack[treated][0];
				int y = stack[treated][1];
				//�����ǰԪ���ϱ���δ����Ԫ�أ��Ҹ�Ԫ���뵱ǰԪ��ͬ�࣬������ջ
				if (x > 0 && !tempChess[x - 1][y] && type == cross[x - 1][y])
				{
					tempChess[x - 1][y] = true;//��ǵ�ǰλ�õ����Ѿ��������
					stack[top][0] = x - 1;
					stack[top][1] = y;
					top++;
				}
				//�����ǰԪ���±���δ����Ԫ�أ��Ҹ�Ԫ���뵱ǰԪ��ͬ�࣬������ջ
				if (x < 10 && !tempChess[x + 1][y] && type == cross[x + 1][y])
				{
					tempChess[x + 1][y] = true;//��ǵ�ǰλ�õ����Ѿ��������
					stack[top][0] = x + 1;
					stack[top][1] = y;
					top++;
				}
				//�����ǰԪ�������δ����Ԫ�أ��Ҹ�Ԫ���뵱ǰԪ��ͬ�࣬������ջ
				if (y > 0 && !tempChess[x][y - 1] && type == cross[x][y - 1])
				{
					tempChess[x][y - 1] = true;//��ǵ�ǰλ�õ����Ѿ��������
					stack[top][0] = x;
					stack[top][1] = y - 1;
					top++;
				}
				//�����ǰԪ���ұ���δ����Ԫ�أ��Ҹ�Ԫ���뵱ǰԪ��ͬ�࣬������ջ
				if (y < 10 && !tempChess[x][y + 1] && type == cross[x][y + 1])
				{
					tempChess[x][y + 1] = true;//��ǵ�ǰλ�õ����Ѿ��������
					stack[top][0] = x;
					stack[top][1] = y + 1;
					top++;
				}
				treated++;//������һ��ջԪ��
			}
			// ��������ɱ��Ϊ���ͽ�������ӵ�ķ�������Ϊmin
			if (top > 0 && IsDeadChess(stack, top, type))
			{
				AddDeadChessScore(stack, top);
			}
			top = treated = 0;//���
		}
	}
}

DIRECTION direction_8[] = { { -1, 0 },{ 0, 1 },{ 1, 0 },{ 0, -1 },{ -1, 1 },{ 1, 1 },{ 1, -1 },{ -1, -1 } };
/**
* [AI2::isGo2Dead �ж����ӵ��Ƿ�����ɱ����]
*/
bool AI2::isGo2Dead(int line, int column, int type)
{
	int player = type;//��¼�����ı�ţ���1����2��
	int rival = player == 1 ? 2 : 1;//��¼�Է��ı�ţ���1����2��
	for (int i = 0; i < 4; i++) Position[i] = false;
	for (int i = 0; i < 4; i++)
	{
		if (cross[line + direction_8[i].x_offset][column + direction_8[i].y_offset] == Rival)
		{
			reduceRecursionTimes();
			if (Besieg(line + direction_8[i].x_offset, column + direction_8[i].y_offset, turn2Who, Rival))
				return true;
			Position[i] = true;
		}
		else if (line + direction_8[i].x_offset < 1 || line + direction_8[i].x_offset > 9
			|| column + direction_8[i].y_offset < 1 || column + direction_8[i].y_offset > 9)
		{
			Position[i] = true;
		}
		else if (cross[line + direction_8[i].x_offset][column + direction_8[i].y_offset] == turn2Who)
		{
			reduceRecursionTimes();
			Position[i] = Besieg(line, column, Rival, turn2Who);
		}
	}
	return Position[0] && Position[1] && Position[2] && Position[3];
}

bool AI2::Besieg(int RivalLine, int RivalColumn, int player, int rival)
{
	bool tie[4] = { false, false, false, false };
	// ����Cross����״̬
	setStatus(RivalLine, RivalColumn);

	//���Է����ӵ��Ϸ��жԷ���������δ���ϱ�Ե
	if (cross[RivalLine - 1][RivalColumn] == rival && RivalLine - 1 > 0
		&& isGo2DeadStatus[RivalLine - 1][RivalColumn] == false)
		tie[0] = Besieg(RivalLine - 1, RivalColumn, player, rival);
	//���Է����ӵ��Ϸ�û�����ӣ�ֱ�ӷ���false
	else if (cross[RivalLine - 1][RivalColumn] == 0 && RivalLine - 1 > 0) return false;
	//���Է����ӵ��Ϸ��м��������ӻ��ѵ���Ե
	else tie[0] = true;
	//���Է����ӵ��·��жԷ���������δ���±�Ե
	if (cross[RivalLine + 1][RivalColumn] == rival && RivalLine + 1 < 10
		&& isGo2DeadStatus[RivalLine + 1][RivalColumn] == false)
		tie[1] = Besieg(RivalLine + 1, RivalColumn, player, rival);
	//���Է����ӵ��·�û������
	else if (cross[RivalLine + 1][RivalColumn] == 0 && RivalLine + 1 < 10) return false;
	//���Է����ӵ��·��м��������ӻ��ѵ���Ե
	else tie[1] = true;
	//���Է����ӵ��ҷ��жԷ���������δ���ұ�Ե
	if (cross[RivalLine][RivalColumn + 1] == rival && RivalColumn + 1 < 10
		&& isGo2DeadStatus[RivalLine][RivalColumn + 1] == false)
		tie[2] = Besieg(RivalLine, RivalColumn + 1, player, rival);
	//���Է����ӵ��ҷ�û������
	else if (cross[RivalLine][RivalColumn + 1] == 0 && RivalColumn + 1 < 10) return false;
	//���Է����ӵ��ҷ��м��������ӻ��ѵ���Ե
	else tie[2] = true;
	//���Է����ӵ����жԷ���������δ�����Ե
	if (cross[RivalLine][RivalColumn - 1] == rival && RivalColumn - 1 > 0
		&& isGo2DeadStatus[RivalLine][RivalColumn - 1] == false)
		tie[3] = Besieg(RivalLine, RivalColumn - 1, player, rival);
	//���Է����ӵ���û������
	else if (cross[RivalLine][RivalColumn - 1] == 0 && RivalColumn - 1 > 0) return false;
	//���Է����ӵ����м��������ӻ��ѵ���Ե
	else tie[3] = true;
	if (tie[0] && tie[1] && tie[2] && tie[3]) return true;
	return false;
}

void AI2::reduceRecursionTimes()
{
	for (int i = 0; i < 10; i++)
	{
		if (isGo2DeadStatus[i][0] == true)
			for (int j = 0; j < 10; j++)
				isGo2DeadStatus[i][j] = false;
	}
}

void AI2::setStatus(int RivalLine, int RivalColumn)
{
	isGo2DeadStatus[RivalLine][RivalColumn] = true;
	isGo2DeadStatus[RivalLine][0] = true;//�±�Ϊ0����һ�����Ϊtrue���ʼ���������ó�ʼ��
}


/**
* �ж�ջ��Ԫ������ɵĴ��Ƿ�����ɱ��Ϊ
* @param stach[][2] ջ
* @param len ջ��Ԫ��ռ�õĳ���
*/
bool AI2::IsDeadChess(int stack[][2], int len, int type)
{
	int x, y;
	int tempCount = 0;
	int tie[4] = { false,false,false,false };
	for (int i = 0; i<len; i++)
	{
		x = stack[i][0];
		y = stack[i][1];
		//�ϱ��пո�
		if (x > 1 && cross[x - 1][y] == Rival)
		{
			tempCount++;
		}
		else if (x > 1 && cross[x - 1][y] == noChess)
		{
			return false;
		}
		else
		{
			tie[0] = true;
		}
		//�±��пո�
		if (x < 9 && cross[x + 1][y] == Rival)
		{
			tempCount++;
		}
		else if (x < 9 && cross[x + 1][y] == noChess)
		{
			return false;
		}
		else
		{
			tie[1] = true;
		}
		//����пո�
		if (y > 1 && cross[x][y - 1] == Rival)
		{
			tempCount++;
		}
		else if (y > 1 && cross[x][y - 1] == noChess)
		{
			return false;
		}
		else
		{
			tie[2] = true;
		}
		//�ұ��пո�
		if (y < 9 && cross[x][y + 1] == Rival)
		{
			tempCount++;
		}
		else if (y < 9 && cross[x][y + 1] == noChess)
		{
			return false;
		}
		else
		{
			tie[3] = true;
		}

		if (tempCount == 4)
		{
			return true;
		}

		if (tie[0] && tie[1] && tie[2] && tie[3])
		{
			return true;
		}

	}
	return true;
}

/**
* Ϊ����λ�ӷ�
* @param stach[][2] ջ
* @param len ջ��Ԫ��ռ�õĳ���
*/
void AI2::AddDeadChessScore(int stack[][2], int len)
{
	for (int i = 0; i<len; i++)
	{
		chessScore[stack[i][0]][stack[i][1]] = minLimit;
		_cprintf("----------dead chess position:  line=%d, column=%d\n", stack[i][0], stack[i][1]);
	}
}

