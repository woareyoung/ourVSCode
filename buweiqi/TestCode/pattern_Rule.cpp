#include <iostream>
#include <string>
using namespace std;
#define ChessInit 0
#define ChessStart 1
#define ChessEnd 10

#define maxLimit 32767
#define minLimit -32767

// 用于匹配模式
#define Black 1 // 黑子
#define White 2// 白子
#define NoChess 4// 没有棋子
#define Edge 8 // 边界点
// 判断棋子是否落在棋盘内
#define isOnChessBoard(x) (0 < (x) && (x) < 10)
#define OnChessBoard(x, y) (isOnChessBoard(x) && isOnChessBoard(y))  //棋子落在棋盘里

#define getRival(onTurn) (onTurn == White ? Black : White)

#define getLine(temp) ( (temp) <= 0 || (temp) > 990 ? 0 : (temp / 100))
#define getColumn(temp) ( (temp) <= 0 || (temp) > 990 ? 0 : (temp % 100))
#define getMove(line, column) (OnChessBoard(line, column) ? (line * 100 + column) : 0)

typedef struct tagDIRECTION
{
	int x_offset;
	int y_offset;
} DIRECTION;

struct Pos {
	int line;
	int column;
};

class pattern_Rule
{
private:
	// 匹配模式
	static const int pattern_Total = 6;
	// 匹配的次数
	static const int pattern_Sum = 42;
	// 模式分数尺度
	int pattern_Score[pattern_Total];
	// 分数说明：
	// 对方自杀点且非我方自杀点的分数为0，我方自杀点为minLitmit。

	// pattern加分位置
	DIRECTION pattern_Score_Pos[pattern_Total];

	// 模式内判断棋子点数
	int pattern_Count[pattern_Total];
	// 看不懂的请看种子填充算法
	DIRECTION pattern_Background[pattern_Sum];
	// 利用与或处理棋子点
	// 匹配模式中棋子分布
	int pattern_White[pattern_Sum];
	// 匹配模式中棋子分布
	int pattern_Black[pattern_Sum];

	void (pattern_Rule::*Reverse[10])(DIRECTION*);
	void reverse(DIRECTION *PatternType);
	void reverseXY(DIRECTION *PatternType);
	void reverse_Y(DIRECTION *PatternType);
	void reverse_X(DIRECTION *PatternType);
	void reverse_X_Y(DIRECTION *PatternType);

	void initAll();
	void init_cross() {
		for (int i = ChessStart; i < ChessEnd; ++i) {
			for (int j = ChessStart; j < ChessEnd; ++j) {
				cross[i][j] = NoChess;
			}
		}
	}
	void Pattern(const int *PatternType, const int& line, const int& column, const int& turn2Who);
protected:
	int *Type[2];
	//记录各交叉点的值，数组访问从“1”开始，访问顺序为“先行后列”，
	//“0”表示没有棋子，“1”表示黑子，“2”表示白子
	mutable int cross[10][10];
public:
	pattern_Rule() {
		initAll();
		init_cross();
	}
	~pattern_Rule() {}

	void startPattern(const int& turn2Who, const int& line, const int& column);
	// -----------------------------------棋盘界面----------------------------------
	void showUnicode(int pos) {
		string ch[20] = { "０","１","２","３","４","５","６","７","８","９" };
		cout<<ch[pos] << "  ";
	}

	void showChessBoard() {
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				if (i == 0) {
					showUnicode(j);
				}
				else if (j == 0) {
					showUnicode(i);
				}
				else {
					if (cross[i][j] == NoChess) {
						cout << "  " << "  ";
					}
					else if (cross[i][j] == Black) {
						cout << "●" << "  ";
					}
					else if (cross[i][j] == White) {
						cout << "〇" << "  ";
					}
				}
			}
			cout << endl << endl;
		}
	}
};

void pattern_Rule::initAll() {
	register int i;
	// 初始化函数指针数组
	void(pattern_Rule::*temp[])(DIRECTION*) = {
		nullptr,//(X,Y)     纯粹只是添加进来凑数而已
		&pattern_Rule::reverse,//(X,Y)
		&pattern_Rule::reverse_Y,//(X,-Y)
		&pattern_Rule::reverse_X,//(-X,-Y)
		&pattern_Rule::reverse_Y,//(-X,Y)
		&pattern_Rule::reverseXY,//(Y,-X)
		&pattern_Rule::reverse_Y,//(Y,X)
		&pattern_Rule::reverse_X,//(-Y,X)
		&pattern_Rule::reverse_Y,//(-Y,-X)
		&pattern_Rule::reverse_X_Y//(X,Y)
	};
	for (i = ChessInit; i < ChessEnd; ++i) {
		Reverse[i] = temp[i];
	}
	// 模式分数尺度
	int patternScore[] = {
		60, 50, 40, 35, 35, 30, 30, 25, 25, 20, 20,// 11
		65, 65, 65, 55, 55, 55// 6
	};
	// 分数说明：
	// 对方自杀点且非我方自杀点的分数为0，我方自杀点为minLitmit。

	DIRECTION patternAddScorePos[] = {
		{ 0, 0 },{ 0, 0 },{ 0, 0 }/*3 敌方位置，默认内部填充阻止*/,{ 0, -1 },{ 1, 0 },{ 1, 1 }// 3，己方形成围杀，默认采用外部空位
	};

	// 模式内判断棋子点数
	int patternCount[] = {
		7, 7, 7, 7, 7, 7// 42

	};
	// 看不懂的请看种子填充算法
	DIRECTION patternBackground[] = {
		{ 0, -1 },{ 0, 1 },{ 0, 2 },{ -1, 0 },{ -1, 1 },{ 1, 0 },{ 1, 1 },
		{ 0, -1 },{ 0, 1 },{ 0, 2 },{ -1, 0 },{ -1, 1 },{ 1, 0 },{ 1, 1 },
		{ 0, -1 },{ 0, 1 },{ 0, 2 },{ -1, 0 },{ -1, 1 },{ 1, 0 },{ 1, 1 },
		{ 0, -1 },{ 0, 1 },{ 0, 2 },{ -1, 0 },{ -1, 1 },{ 1, 0 },{ 1, 1 },
		{ 0, -1 },{ 0, 1 },{ 0, 2 },{ -1, 0 },{ -1, 1 },{ 1, 0 },{ 1, 1 },
		{ 0, -1 },{ 0, 1 },{ 0, 2 },{ -1, 0 },{ -1, 1 },{ 1, 0 },{ 1, 1 },// 6 42 
	};
	// 利用与或处理棋子点
	// 匹配模式中棋子分布
	int patternWhite[] = {
		/*********************************************
		菱形围杀缺一，阻止敌方
		**********************************************/
		// { 0, -1 },{ 0, 1 },{ 0, 2 },{ -1, 0 },{ -1, 1 },{ 1, 0 },{ 1, 1 },
		NoChess,	White,		Black | Edge,	Black | Edge,	Black | Edge,	Black,		Black,
		Black,		White,		Black | Edge,	Black | Edge,	Black | Edge,	NoChess,	Black,
		Black,		White,		Black | Edge,	Black | Edge,	Black | Edge,	Black,		NoChess,
		NoChess,	Black,		White | Edge,	White | Edge,	White | Edge,	White,		White,
		White,		Black,		White | Edge,	White | Edge,	White | Edge,	NoChess,	White,
		White,		Black,		White | Edge,	White | Edge,	White | Edge,	White,		NoChess
	};
	// 匹配模式中棋子分布
	int patternBlack[] = {
		NoChess,	Black,		White | Edge,	White | Edge,	White | Edge,	White,		White,
		White,		Black,		White | Edge,	White | Edge,	White | Edge,	NoChess,	White,
		White,		Black,		White | Edge,	White | Edge,	White | Edge,	White,		NoChess,
		NoChess,	White,		Black | Edge,	Black | Edge,	Black | Edge,	Black,		Black,
		Black,		White,		Black | Edge,	Black | Edge,	Black | Edge,	NoChess,	Black,
		Black,		White,		Black | Edge,	Black | Edge,	Black | Edge,	Black,		NoChess// 6
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
* [pattern_Rule::reverse 不反转]
* @param PatternType [匹配模式中棋子分布]
*/
void pattern_Rule::reverse(DIRECTION *PatternType) {
	;
}

/**
* [pattern_Rule::reverseXY XY交换位置]
* @param PatternType [匹配模式中棋子分布]
*/
void pattern_Rule::reverseXY(DIRECTION *PatternType) {
	register int temp;
	for (register int i = 0; i < pattern_Sum; ++i) {
		temp = (*PatternType).x_offset;
		(*PatternType).x_offset = (*PatternType).y_offset;
		(*PatternType).y_offset = temp;
		++PatternType;
	}
}

/**
* [pattern_Rule::reverse_Y Y取反]
* @param PatternType [匹配模式中棋子分布]
*/
void pattern_Rule::reverse_Y(DIRECTION *PatternType) {
	for (register int i = 0; i < pattern_Sum; ++i) {
		(*PatternType).y_offset = -(*PatternType).y_offset;
		++PatternType;
	}
}

/**
* [pattern_Rule::reverse_X X取反]
* @param PatternType [匹配模式中棋子分布]
*/
void pattern_Rule::reverse_X(DIRECTION *PatternType) {
	for (register int i = 0; i < pattern_Sum; ++i) {
		(*PatternType).x_offset = -(*PatternType).x_offset;
		++PatternType;
	}
}

/**
* [pattern_Rule::reverse_X_Y XY取反后交换]
* @param PatternType [匹配模式中棋子分布]
*/
void pattern_Rule::reverse_X_Y(DIRECTION *PatternType) {
	register int temp;
	for (register int i = 0; i < pattern_Sum; ++i) {
		temp = -(*PatternType).x_offset;
		(*PatternType).x_offset = -(*PatternType).y_offset;
		(*PatternType).y_offset = temp;
		++PatternType;
	}
}
/**
* [pattern_Rule::startPattern 模板匹配]
* @return        [无]
*/
void pattern_Rule::startPattern(const int& turn2Who, const int& line, const int& column) {
	int *PatternType = Type[turn2Who - 1];
	cout << "**************第一个版本 (X, Y)******************" << endl << endl;
	(this->*Reverse[1])(pattern_Background);//第一个版本 (X, Y)
	Pattern(PatternType, line, column, turn2Who);
	cout << "**************第二个版本，Y轴反转 (X, -Y)******************" << endl << endl;
	(this->*Reverse[2])(pattern_Background);//第二个版本，Y轴反转 (X, -Y)
	Pattern(PatternType, line, column, turn2Who);
	cout << "**************第三个版本，X轴反转(-X, -Y)******************" << endl << endl;
	(this->*Reverse[3])(pattern_Background);//第三个版本，X轴反转(-X, -Y)
	Pattern(PatternType, line, column, turn2Who);
	cout << "**************第四个版本，Y轴反转(-X, Y)******************" << endl << endl;
	(this->*Reverse[4])(pattern_Background);// 第四个版本，Y轴反转(-X, Y)
	Pattern(PatternType, line, column, turn2Who);
	cout << "**************第五个版本，XY轴交换 (Y, -X)******************" << endl << endl;
	(this->*Reverse[5])(pattern_Background);// 第五个版本，XY轴交换 (Y, -X)
	Pattern(PatternType, line, column, turn2Who);
	cout << "**************第六个版本，XY轴交换之后Y轴反转 (Y ,X)******************" << endl << endl;
	(this->*Reverse[6])(pattern_Background);// 第六个版本，XY轴交换之后Y轴反转 (Y ,X)
	Pattern(PatternType, line, column, turn2Who);
	cout << "**************第七个版本，XY轴交换之后X轴反转 (-Y, X)******************" << endl << endl;
	(this->*Reverse[7])(pattern_Background);// 第七个版本，XY轴交换之后X轴反转 (-Y, X)
	Pattern(PatternType, line, column, turn2Who);
	cout << "**************第八个版本，XY轴交换之后Y轴反转 (-Y, -X)******************" << endl << endl;
	(this->*Reverse[8])(pattern_Background);// 第八个版本，XY轴交换之后Y轴反转 (-Y, -X)
	Pattern(PatternType, line, column, turn2Who);
	(this->*Reverse[9])(pattern_Background);// 第九个版本，-X，-Y轴交换 (X, Y),这里的主要作用是将模板还原
}

void pattern_Rule::Pattern(const int *PatternType, const int& line, const int& column, const int& turn2Who) {
	int x_offset, y_offset;
	int i, j;
	int pointer = 0;
	for (i = 0; i < pattern_Total; ++i) {
		j = pointer;
		pointer += pattern_Count[i];
		for (; j < pointer; ++j) {
			x_offset = pattern_Background[j].x_offset;
			y_offset = pattern_Background[j].y_offset;
			if ((Edge & PatternType[j]) == 0) continue;
			if (PatternType[j] == NoChess) continue;
			cross[line + x_offset][column + y_offset] = PatternType[j] > Edge ? PatternType[j] - Edge : PatternType[j];
		}
	}
	showChessBoard();
	init_cross();
}

int main() {
	pattern_Rule p;
	// 说明：第一个参数是围杀方，第二和第三是原点位置，建议选中间的点。
	// pattern不能带Edge
	p.startPattern(Black, 8, 1);

	return 0;
}