#pragma once
#ifndef CHESS_H_INCLUDED
#define CHESS_H_INCLUDED
#include "SIP.h"
#include "../FileSystem_Header/FileSystem.h"
#include <list>
class Chess
{
public:
	int cross[10][10];
	FileSystem FileS;
	std::list<SITUATION> MemBattle;
	std::list<PACE> MemPace;

	QUADRANT Qua;
	
};
#endif // CHESS_H_INCLUDED
