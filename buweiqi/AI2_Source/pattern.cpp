#include "../AI2_Header/AI2.h"
#include <algorithm>

// 匹配模式
int pattern_Total = 7;
// 匹配的次数
int pattern_Sum = 28;
// 模式分数尺度
int pattern_Score[] = { 60,50,40,30,30,20,20 };
// 模式内判断棋子点数
int pattern_Count[] = { 4, 4, 4, 4, 4, 4, 4 };
// 看不懂的请看种子填充算法
DIRECTION pattern_Background[] = {
	{ -1, 0 },	{ 1,0 },	{ 0,-1 },	{ 0,1 },
	{ -1, -1 },	{ 1, -1 },	{ 0,-1 },	{ 0,-2 },
	{ -1, 0 },	{ 0, -1 },	{ 0, 1 },	{ 1, 0 },
	{ -1, -1 },	{ 1, -1 },	{ 0,-1 },	{ 0,-2 },
	{ -1, -1 },	{ 1, -1 },	{ 0,-1 },	{ 0,-2 },
	{ -1, -1 },	{ 1, -1 },	{ 0,-1 },	{ 0,-2 },
	{ -1, -1 },	{ 1, -1 },	{ 0,-1 },	{ 0,-2 }
};
// 利用与或处理棋子点
// 匹配模式中棋子分布
int pattern_White[] = {
	/*********************************************
	对方的十字围杀，三角围杀，边角围杀都包含
	组织对方构成围杀是第一优先级
	**********************************************/
	Black | Edge,	Black | Edge,	Black | Edge,	NoChess,
	/*********************************************
	我方的十字围杀，三角围杀，边角围杀都包含
	我方构建围杀阵是第二优先级
	这个是差1构成围杀阵
	**********************************************/
	White | Edge,	White | Edge,	NoChess,		White | Edge,
	/*********************************************
	边角着子点
	**********************************************/
	Edge,			Edge,			NoChess,		NoChess,
	/*********************************************
	我方的十字围杀，三角围杀，边角围杀都包含
	我方构建围杀阵是第二优先级
	这是主动构成围杀阵(缺二构成围杀阵)
	**********************************************/
	White | Edge,	White | Edge,	NoChess,		NoChess,
	White | Edge,	NoChess,		NoChess,		White | Edge,
	/*********************************************
	缺三
	**********************************************/
	NoChess | Edge, White,			NoChess | Edge,	NoChess | Edge,
	NoChess | Edge,	NoChess | Edge,	NoChess | Edge,	White
};
// 匹配模式中棋子分布
int pattern_Black[] = {
	White | Edge,	White | Edge,	White | Edge,	NoChess,
	Black | Edge,	Black | Edge,	NoChess,		Black | Edge,
	Edge,			Edge,			NoChess,		NoChess,
	Black | Edge,	Black | Edge,	NoChess,		NoChess,
	Black | Edge,	NoChess,		NoChess,		Black | Edge,
	NoChess | Edge, Black,			NoChess | Edge,	NoChess | Edge,
	NoChess | Edge,	NoChess | Edge,	NoChess | Edge,	Black
};
int* Type[] = { pattern_Black,pattern_White };

/**
* [AI2::reverse 不反转]
* @param PatternType [匹配模式中棋子分布]
*/
void AI2::reverse(DIRECTION *PatternType) {
	;
}

/**
* [AI2::reverseXY XY交换位置]
* @param PatternType [匹配模式中棋子分布]
*/
void AI2::reverseXY(DIRECTION *PatternType) {
	register int temp;
	for (register int i = 0; i < pattern_Sum; i++) {
		temp = (*PatternType).x_offset;
		(*PatternType).x_offset = (*PatternType).y_offset;
		(*PatternType).y_offset = temp;
		++PatternType;
	}
}

/**
* [AI2::reverse_Y Y取反]
* @param PatternType [匹配模式中棋子分布]
*/
void AI2::reverse_Y(DIRECTION *PatternType) {
	for (register int i = 0; i < pattern_Sum; i++) {
		(*PatternType).y_offset = -(*PatternType).y_offset;
		++PatternType;
	}
}

/**
* [AI2::reverse_X X取反]
* @param PatternType [匹配模式中棋子分布]
*/
void AI2::reverse_X(DIRECTION *PatternType) {
	for (register int i = 0; i < pattern_Sum; i++) {
		(*PatternType).x_offset = -(*PatternType).x_offset;
		++PatternType;
	}
}

/**
* [AI2::reverse_X_Y XY取反后交换]
* @param PatternType [匹配模式中棋子分布]
*/
void AI2::reverse_X_Y(DIRECTION *PatternType) {
	register int temp;
	for (register int i = 0; i < pattern_Sum; i++) {
		temp = -(*PatternType).x_offset;
		(*PatternType).x_offset = -(*PatternType).y_offset;
		(*PatternType).y_offset = temp;
		++PatternType;
	}
}
/**
* [AI2::startPattern 模板匹配]
* @return        [无]
*/
void AI2::startPattern() {
	int *PatternType = Type[turn2Who - 1];
	(this->*Reverse[1])(pattern_Background);//第一个版本 (X, Y)
	Pattern(PatternType);
	(this->*Reverse[2])(pattern_Background);//第二个版本，Y轴反转 (X, -Y)
	Pattern(PatternType);
	(this->*Reverse[3])(pattern_Background);//第三个版本，X轴反转(-X, -Y)
	Pattern(PatternType);
	(this->*Reverse[4])(pattern_Background);// 第四个版本，Y轴反转(-X, Y)
	Pattern(PatternType);
	(this->*Reverse[5])(pattern_Background);// 第五个版本，XY轴交换 (Y, -X)
	Pattern(PatternType);
	(this->*Reverse[6])(pattern_Background);// 第六个版本，XY轴交换之后Y轴反转 (Y ,X)
	Pattern(PatternType);
	(this->*Reverse[7])(pattern_Background);// 第七个版本，XY轴交换之后X轴反转 (-Y, X)
	Pattern(PatternType);
	(this->*Reverse[8])(pattern_Background);// 第八个版本，XY轴交换之后Y轴反转 (-Y, -X)
	Pattern(PatternType);
	(this->*Reverse[9])(pattern_Background);// 第九个版本，-X，-Y轴交换 (X, Y),这里的主要作用是将模板还原
}

/**
* [AI2::Pattern 使用模板匹配棋盘棋子]
* @param PatternType [模板]
*/
void AI2::Pattern(int *PatternType) {
	register int x_offset, y_offset;
	register int i, j, x, y;
	register int pointer, start = 0, score;
	register int *pattern = PatternType;
	int mainColor = NoChess;
	Pos emptyPos[3];
	// 每一个棋子都要遍历一遍模板
	for (x = 1; x < 10; ++x) {
		for (y = 1; y < 10; ++y)
		{
			pointer = 0;
			for (i = 0; i < pattern_Total; i++)
			{
				j = pointer;
				pointer += pattern_Count[i];
				if (NoChess != cross[x][y]) continue;// 如果不是空着子点就跳转下一次循环
				start = 0;
				for (; j < pointer; ++j)
				{
					x_offset = pattern_Background[j].x_offset;
					y_offset = pattern_Background[j].y_offset;
					score = pattern_Score[i];
					// 棋子在棋盘内
					if (OnChessBoard(x + x_offset, y + y_offset)) {
						if (0 == (cross[x + x_offset][y + y_offset] & pattern[j])) goto mismatch;//不相同的
						else
						{
							if (cross[x + x_offset][y + y_offset] == NoChess) {
								// 假如当前空白点的分数值为0的时候，就直接跳过
								// 因为分数为0表示当前空白点的位置是敌方自杀点，没必要理会
								if (chessScore[x + x_offset][y + y_offset] == 0) goto mismatch;
								else {
									emptyPos[start].line = x + x_offset;
									emptyPos[start++].column = y + y_offset;
								}
							}
							else mainColor = cross[x + x_offset][y + y_offset];
						}
					}
					else {
						if (0 == (Edge & pattern[j])) goto mismatch;//不是边界
					}
				}
				// 匹配到一个模版
				// 对于匹配到的模板，我们需要进行模板环境的判断
				// 1、是否被围杀，2、是否围杀别人

				/******************************************
				判断当前匹配到的空位是否是敌方的自杀点，
				如果是的话，就把该点的分数设置为0，跳过匹配模式
				*******************************************/
				// 如果当前位置不为空的话，就直接跳出。
				if (cross[x][y] != NoChess) goto mismatch;
				for (int i = 0; i < start; ++i) {
					if (mainColor == Rival) {
						// 临时设置当前获得的位置为敌方着子点，判断是否是敌方的自杀点
						cross[emptyPos[i].line][emptyPos[i].column] = Rival;
						if (isGo2Dead(emptyPos[i].line, emptyPos[i].column, Rival)) {
							cross[emptyPos[i].line][emptyPos[i].column] = NoChess;
							// 如果是敌方的自杀点的话，这里就置零   -.-！！！
							chessScore[emptyPos[i].line][emptyPos[i].column] = 0;
							goto mismatch;
						}
					}
					else if (mainColor == turn2Who) {
						// 临时设置当前获得的位置为我方着子点，判断是否是我方的自杀点
						cross[x][y] = turn2Who;
						if (isGo2Dead(x, y, turn2Who)) {
							chessScore[x][y] = minLimit;
							cross[x][y] = NoChess;
							// 如果是我方的自杀点的话，就直接跳转，不用判断是否是敌方的自杀点了。
							goto mismatch;
						}
					}
					// 这里既不是我方自杀点，也不是敌方自杀点
					cross[emptyPos[i].line][emptyPos[i].column] = NoChess;
				}
				/******************************************
				这里有一个逻辑需要处理：
				假如一个点被匹配到多次的话，如何处理？
				1、采用累加分的方法进行处理
				2、需要注意该着子点是否是死棋点
				*******************************************/
				// 对于当前匹配到的着子点的环境进行分析
				// 临时设置当前获得的位置为我方着子点，判断是否是我方的自杀点
				cross[x][y] = turn2Who;
				if (isGo2Dead(x, y, turn2Who)) {
					chessScore[x][y] = minLimit;
					cross[x][y] = NoChess;
					// 如果是我方的自杀点的话，就直接跳转，不用判断是否是敌方的自杀点了。
					goto mismatch;
				}
				// 临时设置当前获得的位置为敌方着子点，判断是否是敌方的自杀点
				if (cross[x][y] == NoChess && chessScore[x][y] == 0) goto mismatch;
				cross[x][y] = Rival;
				if (isGo2Dead(x, y, Rival)) {
					cross[x][y] = NoChess;
					// 如果是敌方的自杀点的话，这里就置零   -.-！！！
					chessScore[x][y] = 0;
					goto mismatch;
				}
				// 这里既不是我方自杀点，也不是敌方自杀点
				cross[x][y] = NoChess;
				chessScore[x][y] += score;// 这里匹配到了一个模板，这个模板的位置就是这个
				if (!isContaint({ x,y,score })) {
					goodMoves[MovePointer] = { x,y,score };
					// 排序
					std::sort(goodMoves, goodMoves + MovePointer);
					MovePointer++;
				}
			mismatch:
				;
			}
		}
	}
}