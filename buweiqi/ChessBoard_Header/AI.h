#pragma once
#ifndef AI_H_INCLUDED
#define AI_H_INCLUDED
#include "SIP.h"
#include "../FileSystem_Header/FileSystem.h"

class AI
{
public:
	bool link;//标记是否已打开文件
	FileSystem FS; //文件操作对象
	QUADRANT Qua; //记录走棋纪录
	virtual void GetPosition(int &line, int &column, int onTurn) = 0;
};

#endif // AI_H_INCLUDED
