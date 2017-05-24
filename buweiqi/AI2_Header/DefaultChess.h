#pragma once

class DefaultChess
{
public:
	DefaultChess();
	~DefaultChess();

	/*******************************************
	成员函数
	********************************************/
	int getDefaultChessScore(int line, int column);
private:
	/*******************************************
						成员变量
	********************************************/
	int DefaultChessScore[10][10];
};

