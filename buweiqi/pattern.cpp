#include "./stdafx.h"
#include "./AI2_Header/AI2.h"

/**
* 黑方模板解释
* #黑子 O白子 ~不确定 !边界 $当前位置 *表示empty
* 每一个模板包含了三个块。
* 优先级别：	对方的十字围杀
*			己方的十字围杀
*			对方三角围杀
*			己方的三角围杀
*			己方的边角着子点
*/
int patterns_B[] = //BLACK方模式
{
	/*********************************************
	对方的十字围杀，三角围杀，边角围杀都包含
	组织对方构成围杀是第一优先级
	**********************************************/
	// 开始标志位,4个需要匹配的点，分数
	PATTERN, 4, 60,
	// 下面看不懂的看下种子填充算法
	-1, 0,White | Edge, // 			白子/边缘 
	1, 0, White | Edge, //白子/边缘	  当前	空位
	0,-1, White | Edge, //			白子/边缘 
	0,1, NoChess,

	/*********************************************
	我方的十字围杀，三角围杀，边角围杀都包含
	我方构建围杀阵是第二优先级
	这个是差1构成围杀阵
	**********************************************/
	PATTERN, 4, 50,
	-1, -1,Black | Edge, //		 黑子
	1, -1,Black | Edge,  //	黑子 空位 当前
	0,-1, NoChess,       // 	 黑子
	0,-2,Black | Edge,

	/*********************************************
	边角着子点
	**********************************************/
	PATTERN, 4, 40,
	-1, 0,Edge,		//         边界
	0, -1,Edge,		//  边界 当前位置  空位
	0, 1,NoChess,	//        空位
	1, 0,NoChess,	//

	/*********************************************
	我方的十字围杀，三角围杀，边角围杀都包含
	我方构建围杀阵是第二优先级
	这是主动构成围杀阵(缺二构成围杀阵)
	**********************************************/
	PATTERN, 4, 30,
	-1, -1,Black | Edge, //		 黑子
	1, -1,Black | Edge,  //	空位 空位 当前
	0,-1, NoChess,       // 	 黑子
	0,-2,NoChess,

	PATTERN, 4, 30,
	-1, -1,Black | Edge, //		 空位
	1, -1,NoChess,  	//	黑子 空位 当前
	0,-1, NoChess,       // 	 黑子
	0,-2,Black | Edge,

	/*********************************************
	匹配自杀点
	**********************************************/
	PATTERN, 4, minLimit,
	-1, -1,Black | Edge, //		 黑子
	1, -1,Black | Edge,  //	黑子 白子 当前
	0,-1, White,       	// 	 	黑子
	0,-2,Black | Edge,

	/*********************************************
	缺三
	**********************************************/
	PATTERN, 4, 20,
	-1, -1,NoChess | Edge, 		//		 	空位
	1, -1,Black,  				//	空位		空位 	当前
	0,-1, NoChess | Edge,        //		 	黑子
	0,-2, NoChess | Edge,

	PATTERN, 4, 20,
	-1, -1,NoChess | Edge, 		//		 	空位
	1, -1,NoChess | Edge,  		//	黑子 	空位 	当前
	0,-1, NoChess | Edge,        //		 	空位
	0,-2, Black,

	PATTEND //模式结束
};

int patterns_W[] =
{
	PATTERN, 4, 60,
	-1, 0, Black | Edge,
	1, 0, Black | Edge,
	0,-1, Black | Edge,
	0,1, NoChess,


	PATTERN, 4, 50,
	-1, -1,White | Edge,
	1, -1,White | Edge,
	0,-1,NoChess,
	0,-2,White | Edge,

	PATTERN, 4, 40,
	-1, 0,Edge,
	0, -1,Edge,
	0, 1,NoChess,
	1, 0,NoChess,

	PATTERN, 4, 30,
	-1, 0,White | Edge,
	1, 0, White | Edge,
	0,-1, NoChess,
	0,1, NoChess,

	PATTERN, 4, 30,
	-1, -1,White | Edge,
	1, -1,NoChess,
	0,-1, NoChess,
	0,-2,White | Edge,

	PATTERN, 4, minLimit,
	-1, -1,White | Edge,
	1, -1,White | Edge,
	0,-1, Black,
	0,-2,White | Edge,

	PATTERN, 4, 20,
	-1, -1,NoChess | Edge,
	1, -1, White,
	0,-1, NoChess | Edge,
	0,-2, NoChess | Edge,

	PATTERN, 4, 20,
	-1, -1,NoChess | Edge,
	1, -1,NoChess | Edge,
	0,-1, NoChess | Edge,
	0,-2, White,

	PATTEND
};


/**
* [AI2::startPattern 模板匹配]
* @param  patAdd [模板]
* @return        [无]
*/
void AI2::startPattern() {
	register int t, j, *is;
	int *patAdd = nullptr;
	if (turn2Who == Black) {
		patAdd = patterns_B;
	}
	else if (turn2Who == White) {
		patAdd = patterns_W;
	}
	int *patterns = patAdd;
	/********************************************************
	第一个版本     (X, Y)
	********************************************************/
	Pattern(patAdd, 1);

	/********************************************************
	第二个版本，Y轴反转 (X, -Y)
	********************************************************/
	for (is = patterns; PATTERN == *is; )
	{
		for (j = *++is, is += 2; j--; )
		{
			is += 1;
			*is = (-*is);
			is += 2;
		}
	}

	Pattern(patAdd, 2);

	/********************************************************
	第三个版本，X轴反转 (-X, -Y)
	********************************************************/
	for (is = patterns; PATTERN == *is; )
	{
		for (j = *++is, is += 2; j--; )
		{
			*is = (-*is);
			is += 3;
		}
	}

	Pattern(patAdd, 3);

	/********************************************************
	第四个版本，Y轴反转 (-X, Y)
	********************************************************/
	for (is = patterns; PATTERN == *is; )
	{
		for (j = *++is, is += 2; j--; )
		{
			is += 1;
			*is = (-*is);
			is += 2;
		}
	}

	Pattern(patAdd, 4);

	/********************************************************
	第五个版本，XY轴交换 (Y, -X)
	********************************************************/
	for (is = patterns; PATTERN == *is; )
	{
		for (j = *++is, is += 2; j--; )
		{
			t = *is;
			*is = is[1];
			is[1] = t;
			is += 3;
		}
	}

	Pattern(patAdd, 5);

	/********************************************************
	第六个版本，XY轴交换之后Y轴反转 (Y ,X)
	********************************************************/
	/*Invert y coordinates in pattern table.*/
	for (is = patterns; PATTERN == *is; )
	{
		for (j = *++is, is += 2; j--; )
		{
			is += 1;
			*is = (-*is);
			is += 2;
		}
	}

	Pattern(patAdd, 6);

	/********************************************************
	第七个版本，XY轴交换之后X轴反转 (-Y, X)
	********************************************************/
	/*Invert x coordinates in pattern table.*/
	for (is = patterns; PATTERN == *is; )
	{
		for (j = *++is, is += 2; j--; )
		{
			*is = (-*is);
			is += 3;
		}
	}

	Pattern(patAdd, 7);

	/********************************************************
	第八个版本，XY轴交换之后Y轴反转 (-Y, -X)
	********************************************************/
	/*Invert y coordinates in pattern table.*/
	for (is = patterns; PATTERN == *is; )
	{
		for (j = *++is, is += 2; j--; )
		{
			is += 1;
			*is = (-*is);
			is += 2;
		}
	}

	Pattern(patAdd, 8);

	/********************************************************
	第九个版本，-X，-Y轴交换 (X, Y),这里的主要作用是将模板还原
	********************************************************/
	/*Exchange -x and -y coordinates in pattern table.*/
	for (is = patterns; PATTERN == *is; )
	{
		for (j = *++is, is += 2; j--; )
		{
			t = (-*is);
			*is = (-is[1]);
			is[1] = t;
			is += 3;
		}
	}
}

/**
* [AI2::Pattern 使用模板匹配棋盘棋子]
* @param patAdd [模板]
* @param times  [次数，没什么用，用来记录第几次，方便看而已]
*/
void AI2::Pattern(int *patAdd, int times) {
	register int x, y, j;// xy是匹配到的空位，J使用用来控制模板遍历的结束标识符
	register int *is, *iis;// 指针，用于模板位置的指向
	register int xs, ys;
	Pos emptyPos[3];
	int start = 0;
	int score;
	int *patterns = patAdd;
	// 遍历整一个模板
	for (is = patterns; PATTERN == *is; is += 3 + 3 * (is[1])) //is[1]=*(is+1)
	{
		for (x = 1; x < 10; ++x) {
			for (y = 1; y < 10; ++y)
			{
				if (NoChess == cross[x][y])
				{
					start = 0;
					// 遍历模板的第一块内容
					for (iis = is + 1, j = *iis++, score = *iis++; j; --j)//首次循环 
					{
						xs = *iis++;
						ys = *iis++;
						// 棋子在棋盘内
						if (onboard(x + xs, y + ys)) {
							if (0 == (cross[x + xs][y + ys] & *iis++)) {
								goto mismatch;//不相同的
							}
							else
							{
								if (cross[x + xs][y + ys] == NoChess) {
									// 假如当前空白点的分数值为0的时候，就直接跳过
									// 因为分数为0表示当前空白点的位置是敌方自杀点，没必要理会
									if (chessScore[x + xs][y + ys] == 0) {
										goto mismatch;
									}
									else {
										emptyPos[start].line = x + xs;
										emptyPos[start].column = y + ys;
										++start;
									}
								}
							}
						}
						else {
							if (0 == (Edge & *iis++)) goto mismatch;//不是边界
						}
					}
					//如果执行到这个地方来了，我们就匹配到一个模版
					// 对于匹配到的模板，我们需要进行模板环境的判断
					// 1、是否被围杀，2、是否围杀别人

					/******************************************
					判断当前匹配到的空位是否是敌方的自杀点，
					如果是的话，就把该点的分数设置为0，跳过匹配模式
					*******************************************/
					for (int i = 0; i < start; ++i) {
						// 临时设置当前获得的位置为敌方着子点，判断是否是敌方的自杀点
						cross[emptyPos[i].line][emptyPos[i].column] = Rival;
						bool flag = isGo2Dead(emptyPos[i].line, emptyPos[i].column, Rival);
						if (flag) {
							cross[emptyPos[i].line][emptyPos[i].column] = NoChess;
							// 如果是敌方的自杀点的话，这里就置零   -.-！！！
							chessScore[emptyPos[i].line][emptyPos[i].column] = 0;
							goto mismatch;
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
					_cprintf("**************match up***********\n");
					// 对于当前匹配到的着子点的环境进行分析
					// 如果当前位置不为空的话，就直接跳出。
					if (cross[x][y] != NoChess) {
						goto mismatch;
					}
					// 临时设置当前获得的位置为我方着子点，判断是否是我方的自杀点
					cross[x][y] = turn2Who;
					if (isGo2Dead(x, y, turn2Who)) {
						chessScore[x][y] = minLimit;
						cross[x][y] = NoChess;
						// 如果是我方的自杀点的话，就直接跳转，不用判断是否是敌方的自杀点了。
						goto mismatch;
					}
					// 临时设置当前获得的位置为敌方着子点，判断是否是敌方的自杀点
					cross[x][y] = Rival;
					if (isGo2Dead(x, y, Rival)) {
						cross[x][y] = NoChess;
						// 如果是敌方的自杀点的话，这里就置零   -.-！！！
						chessScore[x][y] = 0;
						goto mismatch;
					}
					// 这里既不是我方自杀点，也不是敌方自杀点
					cross[x][y] = NoChess;
					_cprintf("add score = %d\n", score);
					chessScore[x][y] += score;// 这里匹配到了一个模板，这个模板的位置就是这个
				}
			mismatch:
				;
			}
		}
	}
}