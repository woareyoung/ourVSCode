#pragma once
#ifndef AI_H_INCLUDED
#define AI_H_INCLUDED
#include "SIP.h"
#include "../FileSystem_Header/FileSystem.h"
#include <unordered_map>

class AI
{
public:
	int NowLine;
	virtual void GetPosition(int &line, int &column, int onTurn) = 0;
};

#endif // AI_H_INCLUDED
