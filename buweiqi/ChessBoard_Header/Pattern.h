#pragma once

namespace Pattern {

// 用于匹配模式
#define Black 1 // 黑子
#define White 2// 白子
#define NoChess 4// 没有棋子
#define Edge 8 // 边界点

	typedef struct tagDIRECTION
	{
		int x_offset;
		int y_offset;
	} DIRECTION;

	struct Pos {
		int line;
		int column;
	};

	class ChessPattern {
	private:
		void(Pattern::ChessPattern::*Reverse[10])(DIRECTION*);
		/**
		* [reverse 不反转]
		* @param PatternType [匹配模式中棋子分布]
		*/
		void reverse(DIRECTION *PatternType) {
			;
		}

		/**
		* [reverseXY XY交换位置]
		* @param PatternType [匹配模式中棋子分布]
		*/
		void reverseXY(DIRECTION *PatternType) {
			register int temp;
			for (register int i = 0; i < pattern_Sum; ++i) {
				temp = (*PatternType).x_offset;
				(*PatternType).x_offset = (*PatternType).y_offset;
				(*PatternType).y_offset = temp;
				++PatternType;
			}
		}

		/**
		* [reverse_Y Y取反]
		* @param PatternType [匹配模式中棋子分布]
		*/
		void reverse_Y(DIRECTION *PatternType) {
			for (register int i = 0; i < pattern_Sum; ++i) {
				(*PatternType).y_offset = -(*PatternType).y_offset;
				++PatternType;
			}
		}

		/**
		* [reverse_X X取反]
		* @param PatternType [匹配模式中棋子分布]
		*/
		void reverse_X(DIRECTION *PatternType) {
			for (register int i = 0; i < pattern_Sum; ++i) {
				(*PatternType).x_offset = -(*PatternType).x_offset;
				++PatternType;
			}
		}

		/**
		* [reverse_X_Y XY取反后交换]
		* @param PatternType [匹配模式中棋子分布]
		*/
		void reverse_X_Y(DIRECTION *PatternType) {
			register int temp;
			for (register int i = 0; i < pattern_Sum; ++i) {
				temp = -(*PatternType).x_offset;
				(*PatternType).x_offset = -(*PatternType).y_offset;
				(*PatternType).y_offset = temp;
				++PatternType;
			}
		}
	protected:
		////记录各交叉点的值，数组访问从“1”开始，访问顺序为“先行后列”，
		////“0”表示没有棋子，“1”表示黑子，“2”表示白子
		//mutable int cross[10][10];
		//// 分数
		//int chessScore[10][10];

		bool Position[4];

		// 填充数组进行变换
		virtual int* getPatternType(int turn2Who) {
			return Type[turn2Who - 1];
		}

		// 匹配模式
		static const int pattern_Total = 11;
		// 匹配的次数
		static const int pattern_Sum = 44;
		// 模式分数尺度
		int pattern_Score[pattern_Total];
		// 分数说明：
		// 对方自杀点且非我方自杀点的分数为0，我方自杀点为minLitmit。

		// 模式内判断棋子点数
		int pattern_Count[pattern_Total];
		// 看不懂的请看种子填充算法
		DIRECTION pattern_Background[pattern_Sum];
		// 利用与或处理棋子点
		// 匹配模式中棋子分布
		int pattern_White[pattern_Sum];
		// 匹配模式中棋子分布
		int pattern_Black[pattern_Sum];
		int *Type[2];
	public:
		/**
		* [startPattern 模板匹配]
		* @return        [无]
		*/
		void startPattern(int turn2Who, int(&cross)[10][10], int(&chessScore)[10][10]) {
			int *PatternType = getPatternType(turn2Who);
			(this->*Reverse[1])(pattern_Background);//第一个版本 (X, Y)
			Pattern(PatternType, cross, chessScore);
			(this->*Reverse[2])(pattern_Background);//第二个版本，Y轴反转 (X, -Y)
			Pattern(PatternType, cross, chessScore);
			(this->*Reverse[3])(pattern_Background);//第三个版本，X轴反转(-X, -Y)
			Pattern(PatternType, cross, chessScore);
			(this->*Reverse[4])(pattern_Background);// 第四个版本，Y轴反转(-X, Y)
			Pattern(PatternType, cross, chessScore);
			(this->*Reverse[5])(pattern_Background);// 第五个版本，XY轴交换 (Y, -X)
			Pattern(PatternType, cross, chessScore);
			(this->*Reverse[6])(pattern_Background);// 第六个版本，XY轴交换之后Y轴反转 (Y ,X)
			Pattern(PatternType, cross, chessScore);
			(this->*Reverse[7])(pattern_Background);// 第七个版本，XY轴交换之后X轴反转 (-Y, X)
			Pattern(PatternType, cross, chessScore);
			(this->*Reverse[8])(pattern_Background);// 第八个版本，XY轴交换之后Y轴反转 (-Y, -X)
			Pattern(PatternType, cross, chessScore);
			(this->*Reverse[9])(pattern_Background);// 第九个版本，-X，-Y轴交换 (X, Y),这里的主要作用是将模板还原
		}

		/**
		* [Pattern 使用模板匹配棋盘棋子]
		* @param PatternType [模板]
		*/
		void Pattern(int *PatternType, int(&cross)[10][10], int(&chessScore)[10][10]) {
			register int x_offset, y_offset;
			register int i, j, x, y;
			register int pointer, start = 0, score = 0;
			register int *pattern = PatternType;
			int mainColor = NoChess;
			Pos emptyPos[3];
			auto OnChessBoard = [](int x, int y) {
				auto isOnChessBoard = [](int x) {
					return 0 < x && x < 10;
				};
				return isOnChessBoard(x) && isOnChessBoard(y);
			};
			// 每一个棋子都要遍历一遍模板
			for (x = 1; x < 10; ++x) {
				for (y = 1; y < 10; ++y) {
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
						chessScore[x][y] += score;// 这里匹配到了一个模板，这个模板的位置就是这个
						// arraySort(x, y, score);// 数组排序
					mismatch:
						;
					}
				}
			}
		}

		ChessPattern() {
			register int i;
			// 初始化函数指针数组
			void(Pattern::ChessPattern::*temp[])(Pattern::DIRECTION*) = {
				nullptr,//(X,Y)     纯粹只是添加进来凑数而已
				&Pattern::ChessPattern::reverse,//(X,Y)
				&Pattern::ChessPattern::reverse_Y,//(X,-Y)
				&Pattern::ChessPattern::reverse_X,//(-X,-Y)
				&Pattern::ChessPattern::reverse_Y,//(-X,Y)
				&Pattern::ChessPattern::reverseXY,//(Y,-X)
				&Pattern::ChessPattern::reverse_Y,//(Y,X)
				&Pattern::ChessPattern::reverse_X,//(-Y,X)
				&Pattern::ChessPattern::reverse_Y,//(-Y,-X)
				&Pattern::ChessPattern::reverse_X_Y//(X,Y)
			};
			for (i = 0; i < 10; ++i) {
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

		virtual void arraySort(int& x, int& y, int& score) = 0;
		virtual bool checkStone(int& x, int& y) = 0;
		virtual bool checkEmptyPos(int& x, int& y, int& start, int& mainColor, Pos emptyPos[]) = 0;
	};
	
}

