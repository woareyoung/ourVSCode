#pragma once
#include "SIP.h"
#include "../FileSystem_Header/FileSystem.h"

class AI
{
public:
	int NowLine;
	virtual void GetPosition(int &line, int &column, int onTurn) = 0;

	AI() : NowLine(0)
	{}
};

