#include "../../ChessBoard_Header/Pattern_Moves.h"

std::list<std::pair<int, int>> Pattern_Moves::getMoves(const bool& isFirst, const int cro[][ChessEnd]) {
	auto translate = [](int move) {
		if (move == noChess) {
			return NoChess;
		}
		return move;
	};
	for (int i = ChessStart; i < ChessEnd; ++i) {
		for (int j = ChessStart; j < ChessEnd; ++j) {
			this->cross[i][j] = translate(cro[i][j]);
		}
	}
	initChessScore(isFirst);
	startPattern();
	return patternMoves;
}

void Pattern_Moves::initAll() {
	register int i;
	// 初始化函数指针数组
	void(Pattern_Moves::*temp[])(DIRECTION*) = {
		nullptr,//(X,Y)     纯粹只是添加进来凑数而已
		&Pattern_Moves::reverse,//(X,Y)
		&Pattern_Moves::reverse_Y,//(X,-Y)
		&Pattern_Moves::reverse_X,//(-X,-Y)
		&Pattern_Moves::reverse_Y,//(-X,Y)
		&Pattern_Moves::reverseXY,//(Y,-X)
		&Pattern_Moves::reverse_Y,//(Y,X)
		&Pattern_Moves::reverse_X,//(-Y,X)
		&Pattern_Moves::reverse_Y,//(-Y,-X)
		&Pattern_Moves::reverse_X_Y//(X,Y)
	};
	for (i = ChessInit; i < ChessEnd; ++i) {
		Reverse[i] = temp[i];
	}
	// 模式分数尺度
	int patternScore[] = {
		25, 10, 5, 5, 5, 6, 6, 3, 3, 3, 3,// 11
		12, 12, 12, 13, 13, 13,// 6
		14, 14, 14, 14, 14, 14
	};
	// 分数说明：
	// 对方自杀点且非我方自杀点的分数为0，我方自杀点为minLitmit。

	DIRECTION patternAddScorePos[] = {
		{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },{ 0, 0 },// 11
		{ 0, -1 },{ 1, 0 },{ 1, 1 }/*3 敌方位置，默认内部填充阻止*/,{ 0, -1 },{ 1, 0 },{ 1, 1 },// 3，己方形成围杀，默认采用外部空位
		{ 0, 1 },{ 1, 1 },{ 2, -1 },{ 0, 1 },{ 1, 1 },{ 2, -1 }// 6
	};

	// 模式内判断棋子点数
	int patternCount[] = {
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,// 44
		7, 7, 7, 7, 7, 7,// 42
		11,11,11,11,11,11// 66
	};
	// 看不懂的请看种子填充算法
	DIRECTION patternBackground[] = {
		{ -1, 0 },{ 1,0 },{ 0,-1 },{ 0,1 },

		{ -1, -1 },{ 1, -1 },{ 0,-1 },{ 0,-2 },

		{ -1, 0 },{ 0, -1 },{ 0, 1 },{ 1, 0 },

		{ -1, -1 },{ 1, -1 },{ 0,-1 },{ 0,-2 },
		{ -1, -1 },{ 1, -1 },{ 0,-1 },{ 0,-2 },// 2

		{ -1, -1 },{ 1, -1 },{ 0,-1 },{ 0,-2 },
		{ -1, -1 },{ 1, -1 },{ 0,-1 },{ 0,-2 },// 2

		{ -1, -1 },{ 1, -1 },{ 0,-1 },{ 0,-2 },
		{ -1, -1 },{ 1, -1 },{ 0,-1 },{ 0,-2 },
		{ -1, -1 },{ 1, -1 },{ 0,-1 },{ 0,-2 },
		{ -1, -1 },{ 1, -1 },{ 0,-1 },{ 0,-2 },// 4 44

		{ 0, -1 },{ 0, 1 },{ 0, 2 },{ -1, 0 },{ -1, 1 },{ 1, 0 },{ 1, 1 },
		{ 0, -1 },{ 0, 1 },{ 0, 2 },{ -1, 0 },{ -1, 1 },{ 1, 0 },{ 1, 1 },
		{ 0, -1 },{ 0, 1 },{ 0, 2 },{ -1, 0 },{ -1, 1 },{ 1, 0 },{ 1, 1 },
		{ 0, -1 },{ 0, 1 },{ 0, 2 },{ -1, 0 },{ -1, 1 },{ 1, 0 },{ 1, 1 },
		{ 0, -1 },{ 0, 1 },{ 0, 2 },{ -1, 0 },{ -1, 1 },{ 1, 0 },{ 1, 1 },
		{ 0, -1 },{ 0, 1 },{ 0, 2 },{ -1, 0 },{ -1, 1 },{ 1, 0 },{ 1, 1 },// 6 42 

		{ 0, -2 },{ 1, -2 },{ -1, -1 },{ 0, -1 },{ 1, -1 },{ 2, -1 },{ -1 ,0 },{ 1, 0 },{ 2, 0 },{ 0, 1 },{ 1, 1 },
		{ 0, -2 },{ 1, -2 },{ -1, -1 },{ 0, -1 },{ 1, -1 },{ 2, -1 },{ -1 ,0 },{ 1, 0 },{ 2, 0 },{ 0, 1 },{ 1, 1 },
		{ 0, -2 },{ 1, -2 },{ -1, -1 },{ 0, -1 },{ 1, -1 },{ 2, -1 },{ -1 ,0 },{ 1, 0 },{ 2, 0 },{ 0, 1 },{ 1, 1 },
		{ 0, -2 },{ 1, -2 },{ -1, -1 },{ 0, -1 },{ 1, -1 },{ 2, -1 },{ -1 ,0 },{ 1, 0 },{ 2, 0 },{ 0, 1 },{ 1, 1 },
		{ 0, -2 },{ 1, -2 },{ -1, -1 },{ 0, -1 },{ 1, -1 },{ 2, -1 },{ -1 ,0 },{ 1, 0 },{ 2, 0 },{ 0, 1 },{ 1, 1 },
		{ 0, -2 },{ 1, -2 },{ -1, -1 },{ 0, -1 },{ 1, -1 },{ 2, -1 },{ -1 ,0 },{ 1, 0 },{ 2, 0 },{ 0, 1 },{ 1, 1 }// 6 66
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
		NoChess | Edge,	NoChess | Edge,	NoChess | Edge,	White,

		/*********************************************
		菱形围杀缺一，阻止敌方
		**********************************************/
		// { 0, -1 },{ 0, 1 },{ 0, 2 },{ -1, 0 },{ -1, 1 },{ 1, 0 },{ 1, 1 },
		NoChess,	White,		Black | Edge,	Black | Edge,	Black | Edge,	Black,		Black,
		Black,		White,		Black | Edge,	Black | Edge,	Black | Edge,	NoChess,	Black,
		Black,		White,		Black | Edge,	Black | Edge,	Black | Edge,	Black,		NoChess,
		NoChess,	Black,		White | Edge,	White | Edge,	White | Edge,	White,		White,
		White,		Black,		White | Edge,	White | Edge,	White | Edge,	NoChess,	White,
		White,		Black,		White | Edge,	White | Edge,	White | Edge,	White,		NoChess,

		/*********************************************
		块围杀缺一
		**********************************************/
		Black | Edge,Black | Edge,Black | Edge,	White,White,Black | Edge,Black | Edge,White,Black | Edge,NoChess,Black | Edge,
		Black | Edge,Black | Edge,Black | Edge,	White,White,Black | Edge,Black | Edge,White,Black | Edge,Black | Edge,NoChess,
		Black | Edge,Black | Edge,Black | Edge,	White,White,NoChess,Black | Edge,White,Black | Edge,Black | Edge,Black | Edge,
		White | Edge,White | Edge,White | Edge,	Black,Black,White | Edge,White | Edge,Black,White | Edge,NoChess,White | Edge,
		White | Edge,White | Edge,White | Edge,	Black,Black,White | Edge,White | Edge,Black,White | Edge,White | Edge,NoChess,
		White | Edge,White | Edge,White | Edge,	Black,Black,NoChess,White | Edge,Black,White | Edge,White | Edge,White | Edge
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
		NoChess | Edge,	NoChess | Edge,	NoChess | Edge,	Black,// 11
		NoChess,	Black,		White | Edge,	White | Edge,	White | Edge,	White,		White,
		White,		Black,		White | Edge,	White | Edge,	White | Edge,	NoChess,	White,
		White,		Black,		White | Edge,	White | Edge,	White | Edge,	White,		NoChess,
		NoChess,	White,		Black | Edge,	Black | Edge,	Black | Edge,	Black,		Black,
		Black,		White,		Black | Edge,	Black | Edge,	Black | Edge,	NoChess,	Black,
		Black,		White,		Black | Edge,	Black | Edge,	Black | Edge,	Black,		NoChess,// 6
		White | Edge,White | Edge,White | Edge,	Black,Black,White | Edge,White | Edge,Black,White | Edge,NoChess,White | Edge,
		White | Edge,White | Edge,White | Edge,	Black,Black,White | Edge,White | Edge,Black,White | Edge,White | Edge,NoChess,
		White | Edge,White | Edge,White | Edge,	Black,Black,NoChess,White | Edge,Black,White | Edge,White | Edge,White | Edge,
		Black | Edge,Black | Edge,Black | Edge,	White,White,Black | Edge,Black | Edge,White,Black | Edge,NoChess,Black | Edge,
		Black | Edge,Black | Edge,Black | Edge,	White,White,Black | Edge,Black | Edge,White,Black | Edge,Black | Edge,NoChess,
		Black | Edge,Black | Edge,Black | Edge,	White,White,NoChess,Black | Edge,White,Black | Edge,Black | Edge,Black | Edge
	};
	for (i = 0; i < pattern_Total; ++i) {
		this->pattern_Score_Pos[i] = patternAddScorePos[i];
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
* [Pattern_Moves::reverse 不反转]
* @param PatternType [匹配模式中棋子分布]
*/
void Pattern_Moves::reverse(DIRECTION *PatternType) {
	;
}

/**
* [Pattern_Moves::reverseXY XY交换位置]
* @param PatternType [匹配模式中棋子分布]
*/
void Pattern_Moves::reverseXY(DIRECTION *PatternType) {
	register int temp;
	register DIRECTION* ScorePos = pattern_Score_Pos;
	for (register int i = 0; i < pattern_Sum; ++i) {
		temp = (*PatternType).x_offset;
		(*PatternType).x_offset = (*PatternType).y_offset;
		(*PatternType).y_offset = temp;
		++PatternType;
	}
	for (register int i = 0; i < pattern_Total; ++i) {
		temp = (*ScorePos).x_offset;
		(*ScorePos).x_offset = (*ScorePos).y_offset;
		(*ScorePos).y_offset = temp;
		++ScorePos;
	}
}

/**
* [Pattern_Moves::reverse_Y Y取反]
* @param PatternType [匹配模式中棋子分布]
*/
void Pattern_Moves::reverse_Y(DIRECTION *PatternType) {
	register DIRECTION* ScorePos = pattern_Score_Pos;
	for (register int i = 0; i < pattern_Sum; ++i) {
		(*PatternType).y_offset = -(*PatternType).y_offset;
		++PatternType;
	}
	for (register int i = 0; i < pattern_Total; ++i) {
		(*ScorePos).y_offset = -(*ScorePos).y_offset;
		++ScorePos;
	}
}

/**
* [Pattern_Moves::reverse_X X取反]
* @param PatternType [匹配模式中棋子分布]
*/
void Pattern_Moves::reverse_X(DIRECTION *PatternType) {
	register DIRECTION* ScorePos = pattern_Score_Pos;
	for (register int i = 0; i < pattern_Sum; ++i) {
		(*PatternType).x_offset = -(*PatternType).x_offset;
		++PatternType;
	}
	for (register int i = 0; i < pattern_Total; ++i) {
		(*ScorePos).x_offset = -(*ScorePos).x_offset;
		++ScorePos;
	}
}

/**
* [Pattern_Moves::reverse_X_Y XY取反后交换]
* @param PatternType [匹配模式中棋子分布]
*/
void Pattern_Moves::reverse_X_Y(DIRECTION *PatternType) {
	register int temp;
	register DIRECTION* ScorePos = pattern_Score_Pos;
	for (register int i = 0; i < pattern_Sum; ++i) {
		temp = -(*PatternType).x_offset;
		(*PatternType).x_offset = -(*PatternType).y_offset;
		(*PatternType).y_offset = temp;
		++PatternType;
	}
	for (register int i = 0; i < pattern_Total; ++i) {
		temp = -(*ScorePos).x_offset;
		(*ScorePos).x_offset = -(*ScorePos).y_offset;
		(*ScorePos).y_offset = temp;
		++ScorePos;
	}
}
/**
* [Pattern_Moves::startPattern 模板匹配]
* @return        [无]
*/
void Pattern_Moves::startPattern() {
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
* [Pattern_Moves::Pattern 使用模板匹配棋盘棋子]
* @param PatternType [模板]
*/
void Pattern_Moves::Pattern(const int *PatternType) {
	register int x_offset, y_offset;
	register int i, j, x, y;
	register int pointer, start = 0, score;
	register int mainColor = NoChess;
	register Pos emptyPos[ChessEnd];
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
				score = pattern_Score[i];
				for (; j < pointer; ++j)
				{
					x_offset = pattern_Background[j].x_offset;
					y_offset = pattern_Background[j].y_offset;
					// 棋子在棋盘内
					if (OnChessBoard(x + x_offset, y + y_offset)) {
						if (0 == (cross[x + x_offset][y + y_offset] & PatternType[j])) goto mismatch;//不相同的
						else
						{
							if (cross[x + x_offset][y + y_offset] == NoChess) {
								// 如果当前空白点是死棋位的话，就直接跳过匹配了
								if (chessScore[x + x_offset][y + y_offset] == minLimit) goto mismatch;
								// 假如当前空白点的分数值为0的时候，就直接跳过
								// 因为分数为0表示当前空白点的位置是敌方自杀点，没必要理会
								else if (chessScore[x + x_offset][y + y_offset] == 0) goto mismatch;
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
				if (pattern_Score_Pos[i].x_offset == 0  && 
					pattern_Score_Pos[i].y_offset == 0 &&
					!checkStone(x, y, pattern_Count[i] <= 4)) {
					goto mismatch;
				};
				if (chessScore[x + pattern_Score_Pos[i].x_offset][y + pattern_Score_Pos[i].y_offset] != 0 && 
					chessScore[x + pattern_Score_Pos[i].x_offset][y + pattern_Score_Pos[i].y_offset] != minLimit) {
					patternMoves.emplace_back(getMove(x + pattern_Score_Pos[i].x_offset, y + pattern_Score_Pos[i].y_offset), score);
				}
			mismatch:
				;
			}
		}
	}
}

void Pattern_Moves::initChessScore(const bool& isFirst)
{
	for (int i = ChessStart; i < ChessEnd; ++i)
	{
		for (int j = ChessStart; j < ChessEnd; ++j)
		{
			if (cross[i][j] == NoChess && chessScore[i][j] == minLimit) continue;
			if (!isFirst && cross[i][j] == NoChess && chessScore[i][j] == 0) continue;
			chessScore[i][j] = getDefaultChessScore(i, j);
		}
	}
}

#include <stack>
#include <tuple>

void Pattern_Moves::resetGo2DeadStatus() {
	for (register int i = ChessInit; i < ChessEnd; ++i) {
		for (register int j = ChessInit; j < ChessEnd; ++j) {
			isGo2DeadStatus[i][j] = false;
		}
	}
}

/**
* [AI2::isGo2Dead 判断着子点是否有自杀倾向]
*/
bool Pattern_Moves::isGo2Dead(const int line, const int column, const int& type)
{
	bool simulate = false;//标记是否是模拟下棋
	int rival = getRival(type);//对方
	int tempLine = line, tempColumn = column;

	std::stack<std::pair<int, int>> position;
	std::stack<bool> PosNumber;//用于计算遍历数量
							   //弹出数据
	auto POPdata = [&]() {
		if (position.empty()) return false;
		std::tie(tempLine, tempColumn) = position.top();
		position.pop();
		return true;
	};
	//压栈，并设置遍历
	auto PUSHdata = [&](int l, int c) {
		position.push(std::make_pair(tempLine, tempColumn));
		tempLine = l;
		tempColumn = c;
	};
	//还原数据
	auto BackData = [&]() {
		tempLine = line;
		tempColumn = column;
		while (!position.empty()) position.pop();
	};
	//判断是否是模拟下棋
	if (cross[tempLine][tempColumn] == NoChess)
	{
		cross[tempLine][tempColumn] = type;
		simulate = true;
	}
	//跟踪遍历（判断有没有被围死）
	//param[Player]:检查 谁 的棋子被围死
	auto VisitAll = [&](const int& Player)
	{
		resetGo2DeadStatus();
		int Rival = getRival(Player);
		while (true)
		{
			isGo2DeadStatus[tempLine][tempColumn] = true;
			//遇到空位，活下去了
			if (cross[tempLine][tempColumn] == NoChess && OnChessBoard(tempLine, tempColumn))
			{
				BackData();
				return false;
			}
			//如果上面有自己的棋子且未遍历过
			if (cross[tempLine - 1][tempColumn] == Player && !isGo2DeadStatus[tempLine - 1][tempColumn] && tempLine > 1)
			{
				PUSHdata(tempLine - 1, tempColumn);
				continue;
			}
			else if (cross[tempLine - 1][tempColumn] == NoChess && tempLine > 1)
			{
				--tempLine;
				continue;
			}
			//如果下面有自己的棋子且未遍历过
			if (cross[tempLine + 1][tempColumn] == Player && !isGo2DeadStatus[tempLine + 1][tempColumn] && tempLine < 9)
			{
				PUSHdata(tempLine + 1, tempColumn);
				continue;
			}
			else if (cross[tempLine + 1][tempColumn] == NoChess && tempLine < 9)
			{
				++tempLine;
				continue;
			}
			//如果左边有自己的棋子且未遍历过
			if (cross[tempLine][tempColumn - 1] == Player && !isGo2DeadStatus[tempLine][tempColumn - 1] && tempColumn > 1)
			{
				PUSHdata(tempLine, tempColumn - 1);
				continue;
			}
			else if (cross[tempLine][tempColumn - 1] == NoChess && tempColumn > 1)
			{
				--tempColumn;
				continue;
			}
			//如果右边有自己的棋子且未遍历过
			if (cross[tempLine][tempColumn + 1] == Player && !isGo2DeadStatus[tempLine][tempColumn + 1] && tempColumn < 9)
			{
				PUSHdata(tempLine, tempColumn + 1);
				continue;
			}
			else if (cross[tempLine][tempColumn + 1] == NoChess && tempColumn < 9)
			{
				++tempColumn;
				continue;
			}
			if (POPdata()) continue;
			BackData();
			return true;//能到这里证明挂了
		}
	};
	if (cross[tempLine - 1][tempColumn] == rival && tempLine > 1)
	{
		--tempLine;
		if (VisitAll(rival)) {
			if (simulate) cross[line][column] = NoChess;
			return true;
		}
	}
	if (cross[tempLine + 1][tempColumn] == rival && tempLine < 9)
	{
		++tempLine;
		if (VisitAll(rival)) {
			if (simulate) cross[line][column] = NoChess;
			return true;
		}
	}
	if (cross[tempLine][tempColumn - 1] == rival && tempColumn > 1)
	{
		--tempColumn;
		if (VisitAll(rival)) {
			if (simulate) cross[line][column] = NoChess;
			return true;
		}
	}
	if (cross[tempLine][tempColumn + 1] == rival && tempColumn < 9)
	{
		++tempColumn;
		if (VisitAll(rival)) {
			if (simulate) cross[line][column] = NoChess;
			return true;
		}
	}
	if (VisitAll(type)) {
		if (simulate) cross[line][column] = NoChess;
		return true;
	}
	if (simulate) cross[line][column] = NoChess;
	return false;
}

