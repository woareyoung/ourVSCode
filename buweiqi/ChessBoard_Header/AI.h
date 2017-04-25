#pragma once
#ifndef AI_H_INCLUDED
#define AI_H_INCLUDED

class AI
{
public:
	virtual void GetPosition(int &line, int &column, int onTurn) = 0;
};

#endif // AI_H_INCLUDED
