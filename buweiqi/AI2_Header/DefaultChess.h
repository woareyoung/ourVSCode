#pragma once

class DefaultChess
{
public:
	DefaultChess();
	~DefaultChess();

	/*******************************************
	��Ա����
	********************************************/
	int getDefaultChessScore(int line, int column);
private:
	/*******************************************
						��Ա����
	********************************************/
	int DefaultChessScore[10][10];
};

