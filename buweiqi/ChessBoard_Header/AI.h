#pragma once
#include "SIP.h"
#include "../FileSystem_Header/FileSystem.h"
#include <unordered_map>

class AI
{
public:
	int NowLine;
	virtual void GetPosition(int &line, int &column, int onTurn) = 0;

	AI() : NowLine(0)
	{}
};

