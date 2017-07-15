#include "../AI2_Header/AI2.h"
#include "../ChessBoard_Header/Pattern.h"

int patternAllScore[9][11] = {
	{ 60, 50, 40, 35, 35, 30, 30, 25, 25, 20, 20 },// 6
	{ 55, 50, 40, 35, 35, 30, 30, 25, 25, 20, 20 },
	{ 54, 50, 40, 35, 35, 30, 30, 25, 25, 20, 20 },
	{ 53, 50, 40, 35, 35, 30, 30, 25, 25, 20, 20 },
	{ 52, 50, 40, 35, 35, 30, 30, 25, 25, 20, 20 },
	{ 51, 50, 40, 35, 35, 30, 30, 25, 25, 20, 20 },

	{ 45, 40, 35, 30, 30, 25, 25, 22, 22, 20, 20 },// 1

	{ 50, 60, 40, 35, 35, 30, 30, 25, 25, 20, 20 },
	{ 40, 50, 60, 35, 35, 30, 30, 25, 25, 20, 20 }
};

void AI2::initAll() {
	register int i;
	// 初始化函数指针数组
	void(AI2::*temp[])(DIRECTION*) = {
		nullptr,//(X,Y)     纯粹只是添加进来凑数而已
		&AI2::reverse,//(X,Y)
		&AI2::reverse_Y,//(X,-Y)
		&AI2::reverse_X,//(-X,-Y)
		&AI2::reverse_Y,//(-X,Y)
		&AI2::reverseXY,//(Y,-X)
		&AI2::reverse_Y,//(Y,X)
		&AI2::reverse_X,//(-Y,X)
		&AI2::reverse_Y,//(-Y,-X)
		&AI2::reverse_X_Y//(X,Y)
	};
	for (i = ChessInit; i < ChessEnd; ++i) {
		Reverse[i] = temp[i];
	}
	// 模式分数尺度
	int patternScore[] = { 60, 50, 40, 35, 35, 30, 30, 25, 25, 20, 20 };
	// 分数说明：
	// 对方自杀点且非我方自杀点的分数为0，我方自杀点为minLitmit。

	// 模式内判断棋子点数
	int patternCount[] = { 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 };
	// 看不懂的请看种子填充算法
	DIRECTION patternBackground[] = {
		{ -1, 0 },{ 1,0 },{ 0,-1 },{ 0,1 },

		{ -1, -1 },{ 1, -1 },{ 0,-1 },{ 0,-2 },

		{ -1, 0 },{ 0, -1 },{ 0, 1 },{ 1, 0 },

		{ -1, -1 },{ 1, -1 },{ 0,-1 },{ 0,-2 },
		{ -1, -1 },{ 1, -1 },{ 0,-1 },{ 0,-2 },

		{ -1, -1 },{ 1, -1 },{ 0,-1 },{ 0,-2 },
		{ -1, -1 },{ 1, -1 },{ 0,-1 },{ 0,-2 },

		{ -1, -1 },{ 1, -1 },{ 0,-1 },{ 0,-2 },
		{ -1, -1 },{ 1, -1 },{ 0,-1 },{ 0,-2 },
		{ -1, -1 },{ 1, -1 },{ 0,-1 },{ 0,-2 },
		{ -1, -1 },{ 1, -1 },{ 0,-1 },{ 0,-2 }
	};
	// 利用与或处理棋子点
	// 匹配模式中棋子分布
	int patternWhite[] = {
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
		敌方方的十字围杀，三角围杀，边角围杀都包含
		敌方构建围杀阵是第二优先级
		这是主动构成围杀阵(缺二构成围杀阵)
		**********************************************/
		Black | Edge,	Black | Edge,	NoChess,		NoChess,
		Black | Edge,	NoChess,		NoChess,		Black | Edge,
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
		NoChess | Edge, Black,			NoChess | Edge,	NoChess | Edge,
		NoChess | Edge,	NoChess | Edge,	NoChess | Edge,	Black,
		NoChess | Edge, White,			NoChess | Edge,	NoChess | Edge,
		NoChess | Edge,	NoChess | Edge,	NoChess | Edge,	White
	};
	// 匹配模式中棋子分布
	int patternBlack[] = {
		White | Edge,	White | Edge,	White | Edge,	NoChess,
		Black | Edge,	Black | Edge,	NoChess,		Black | Edge,
		Edge,			Edge,			NoChess,		NoChess,
		White | Edge,	White | Edge,	NoChess,		NoChess,
		White | Edge,	NoChess,		NoChess,		White | Edge,
		Black | Edge,	Black | Edge,	NoChess,		NoChess,
		Black | Edge,	NoChess,		NoChess,		Black | Edge,
		NoChess | Edge, White,			NoChess | Edge,	NoChess | Edge,
		NoChess | Edge,	NoChess | Edge,	NoChess | Edge,	White,
		NoChess | Edge, Black,			NoChess | Edge,	NoChess | Edge,
		NoChess | Edge,	NoChess | Edge,	NoChess | Edge,	Black
	};
	for (i = 0; i < pattern_Total; ++i) {
		this->pattern_Score[i] = patternScore[i];
		this->pattern_Count[i] = patternCount[i];
	}
	for (i = 0; i < pattern_Sum; ++i) {
		this->pattern_Background[i] = patternBackground[i];
		this->pattern_White[i] = patternWhite[i];
		this->pattern_Black[i] = patternBlack[i];
	}
	int* pType[] = { pattern_Black,pattern_White };
	for (i = 0; i < 2; ++i) {
		Type[i] = pType[i];
	}
}

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
	for (register int i = 0; i < pattern_Sum; ++i) {
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
	for (register int i = 0; i < pattern_Sum; ++i) {
		(*PatternType).y_offset = -(*PatternType).y_offset;
		++PatternType;
	}
}

/**
* [AI2::reverse_X X取反]
* @param PatternType [匹配模式中棋子分布]
*/
void AI2::reverse_X(DIRECTION *PatternType) {
	for (register int i = 0; i < pattern_Sum; ++i) {
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
	for (register int i = 0; i < pattern_Sum; ++i) {
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
	initCSPoint();
	int *PatternType = getPatternType();
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
void AI2::Pattern(const int *PatternType) {
	register int x_offset, y_offset;
	register int i, j, x, y;
	register int pointer, start = 0, score;
	register int mainColor = NoChess;
	register Pos emptyPos[10];
	// 每一个棋子都要遍历一遍模板
	for (x = ChessStart; x < ChessEnd; ++x) {
		for (y = ChessStart; y < ChessEnd; ++y) {
			pointer = 0;
			for (i = 0; i < pattern_Total; ++i)
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
						if (0 == (cross[x + x_offset][y + y_offset] & PatternType[j])) goto mismatch;//不相同的
						else
						{
							if (cross[x + x_offset][y + y_offset] == NoChess) {
								// 假如当前空白点的分数值为0的时候，就直接跳过
								// 因为分数为0表示当前空白点的位置是敌方自杀点，没必要理会
								if (CS[x + x_offset][y + y_offset] == 0) goto mismatch;
								else {
									emptyPos[start].line = x + x_offset;
									emptyPos[start++].column = y + y_offset;
								}
							}
							else mainColor = cross[x + x_offset][y + y_offset];
						}
					}
					else {
						if (0 == (Edge & PatternType[j])) goto mismatch;//不是边界
					}
				}
				// 匹配到一个模版
				// 对于匹配到的模板，我们需要进行模板环境的判断
				// 1、是否被围杀，2、是否围杀别人

				// 如果当前位置不为空的话，就直接跳出。
				if (cross[x][y] != NoChess) goto mismatch;
				if (!checkEmptyPos(x, y, start, mainColor, emptyPos)) {
					goto mismatch;
				}
				/******************************************
				这里有一个逻辑需要处理：
				假如一个点被匹配到多次的话，如何处理？
				1、采用累加分的方法进行处理
				2、需要注意该着子点是否是死棋点
				*******************************************/
				// 检查棋子是否有效，并对分析的结果进行相应的加分
				if (!checkStone(x, y)) {
					goto mismatch;
				};
				CS[x][y] += score;// 这里匹配到了一个模板，这个模板的位置就是这个
			mismatch:
				;
			}
		}
	}
}