#pragma once

#define ChessInit 0
#define ChessStart 1
#define ChessEnd 10

#define maxLimit 32767
#define minLimit -32767

#define isParent 1
#define isSiblings 2

#define isMax 0
#define isMin 1

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

class DefaultChess
{
public:
	DefaultChess() {
		int temp[10][10] =
		{
			{ 0,0,0,0,0,0,0,0,0,0 },
			{ 0,1,1,1,1,1,1,1,1,1 },
			{ 0,1,2,2,2,2,2,2,2,1 },
			{ 0,1,2,3,3,3,3,3,2,1 },
			{ 0,1,2,3,4,4,4,3,2,1 },
			{ 0,1,2,3,4,5,4,3,2,1 },
			{ 0,1,2,3,4,4,4,3,2,1 },
			{ 0,1,2,3,3,3,3,3,2,1 },
			{ 0,1,2,2,2,2,2,2,2,1 },
			{ 0,1,1,1,1,1,1,1,1,1 }
		};
		for (int i = 1; i < 10; ++i)
		{
			for (int j = 1; j < 10; ++j)
			{
				DefaultChessScore[i][j] = temp[i][j];
			}
		}
	}
	~DefaultChess() {
	
	}

	/*******************************************
	成员函数
	********************************************/
	int getDefaultChessScore(int line, int column) {
		return DefaultChessScore[line][column];
	}
private:
	/*******************************************
						成员变量
	********************************************/
	int DefaultChessScore[10][10];
};

