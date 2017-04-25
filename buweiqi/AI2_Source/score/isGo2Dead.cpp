#include "../../stdafx.h"
#include "../../AI2_Header/AI2.h"

/**
* [AI2::isGo2Dead 判断着子点是否有自杀倾向]
*/
void AI2::isGo2Dead(int type)
{
	int stack[81][2] = { 0 }; //存储栈元素，0为x,1为y
	int top = 0;// 栈顶元素
	int treated = 0;// 处理过的元素

					// 重置所有的状态位
	bool tempChess[10][10] = { 0 }; //用来存储当前位置的子是否被处理过
	for (int i = 1; i < 10; i++)
	{
		for (int j = 1; j < 10; j++)
		{
			if (cross[i][j] != type || !tempChess[i][j])
			{
				continue;
			}
			tempChess[i][j] = true;
			// 初始化栈顶元素
			stack[top][0] = i;
			stack[top][1] = j;
			top++;
			// 当栈顶元素的数量级小于处理过的元素的数量的时候，循环终止
			while (treated < top)
			{
				int x = stack[treated][0];
				int y = stack[treated][1];
				//如果当前元素上边有未处理元素，且该元素与当前元素同类，则将其入栈
				if (x > 0 && !tempChess[x - 1][y] && type == cross[x - 1][y])
				{
					tempChess[x - 1][y] = true;//标记当前位置的子已经被处理过
					stack[top][0] = x - 1;
					stack[top][1] = y;
					top++;
				}
				//如果当前元素下边有未处理元素，且该元素与当前元素同类，则将其入栈
				if (x < 10 && !tempChess[x + 1][y] && type == cross[x + 1][y])
				{
					tempChess[x + 1][y] = true;//标记当前位置的子已经被处理过
					stack[top][0] = x + 1;
					stack[top][1] = y;
					top++;
				}
				//如果当前元素左边有未处理元素，且该元素与当前元素同类，则将其入栈
				if (y > 0 && !tempChess[x][y - 1] && type == cross[x][y - 1])
				{
					tempChess[x][y - 1] = true;//标记当前位置的子已经被处理过
					stack[top][0] = x;
					stack[top][1] = y - 1;
					top++;
				}
				//如果当前元素右边有未处理元素，且该元素与当前元素同类，则将其入栈
				if (y < 10 && !tempChess[x][y + 1] && type == cross[x][y + 1])
				{
					tempChess[x][y + 1] = true;//标记当前位置的子已经被处理过
					stack[top][0] = x;
					stack[top][1] = y + 1;
					top++;
				}
				treated++;//处理下一个栈元素
			}
			// 假如是自杀行为，就将这个着子点的分数设置为min
			if (top > 0 && IsDeadChess(stack, top, type))
			{
				AddDeadChessScore(stack, top);
			}
			top = treated = 0;//清空
		}
	}
}

DIRECTION direction_8[] = { { -1, 0 },{ 0, 1 },{ 1, 0 },{ 0, -1 },{ -1, 1 },{ 1, 1 },{ 1, -1 },{ -1, -1 } };
/**
* [AI2::isGo2Dead 判断着子点是否有自杀倾向]
*/
bool AI2::isGo2Dead(int line, int column, int type)
{
	int player = type;//记录己方的编号（是1还是2）
	int rival = player == 1 ? 2 : 1;//记录对方的编号（是1还是2）
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
	// 设置Cross数组状态
	setStatus(RivalLine, RivalColumn);

	//若对方棋子的上方有对方的棋子且未到上边缘
	if (cross[RivalLine - 1][RivalColumn] == rival && RivalLine - 1 > 0
		&& isGo2DeadStatus[RivalLine - 1][RivalColumn] == false)
		tie[0] = Besieg(RivalLine - 1, RivalColumn, player, rival);
	//若对方棋子的上方没有棋子，直接返回false
	else if (cross[RivalLine - 1][RivalColumn] == 0 && RivalLine - 1 > 0) return false;
	//若对方棋子的上方有己方的棋子或已到边缘
	else tie[0] = true;
	//若对方棋子的下方有对方的棋子且未到下边缘
	if (cross[RivalLine + 1][RivalColumn] == rival && RivalLine + 1 < 10
		&& isGo2DeadStatus[RivalLine + 1][RivalColumn] == false)
		tie[1] = Besieg(RivalLine + 1, RivalColumn, player, rival);
	//若对方棋子的下方没有棋子
	else if (cross[RivalLine + 1][RivalColumn] == 0 && RivalLine + 1 < 10) return false;
	//若对方棋子的下方有己方的棋子或已到边缘
	else tie[1] = true;
	//若对方棋子的右方有对方的棋子且未到右边缘
	if (cross[RivalLine][RivalColumn + 1] == rival && RivalColumn + 1 < 10
		&& isGo2DeadStatus[RivalLine][RivalColumn + 1] == false)
		tie[2] = Besieg(RivalLine, RivalColumn + 1, player, rival);
	//若对方棋子的右方没有棋子
	else if (cross[RivalLine][RivalColumn + 1] == 0 && RivalColumn + 1 < 10) return false;
	//若对方棋子的右方有己方的棋子或已到边缘
	else tie[2] = true;
	//若对方棋子的左方有对方的棋子且未到左边缘
	if (cross[RivalLine][RivalColumn - 1] == rival && RivalColumn - 1 > 0
		&& isGo2DeadStatus[RivalLine][RivalColumn - 1] == false)
		tie[3] = Besieg(RivalLine, RivalColumn - 1, player, rival);
	//若对方棋子的左方没有棋子
	else if (cross[RivalLine][RivalColumn - 1] == 0 && RivalColumn - 1 > 0) return false;
	//若对方棋子的左方有己方的棋子或已到边缘
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
	isGo2DeadStatus[RivalLine][0] = true;//下标为0的那一行如果为true则初始化，否则不用初始化
}


/**
* 判断栈内元素所组成的串是否是自杀行为
* @param stach[][2] 栈
* @param len 栈的元素占用的长度
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
		//上边有空格
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
		//下边有空格
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
		//左边有空格
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
		//右边有空格
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
* 为死棋位加分
* @param stach[][2] 栈
* @param len 栈的元素占用的长度
*/
void AI2::AddDeadChessScore(int stack[][2], int len)
{
	for (int i = 0; i<len; i++)
	{
		chessScore[stack[i][0]][stack[i][1]] = minLimit;
		_cprintf("----------dead chess position:  line=%d, column=%d\n", stack[i][0], stack[i][1]);
	}
}

