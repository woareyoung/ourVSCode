#pragma once
#ifndef CHESS_H_INCLUDED
#define CHESS_H_INCLUDED
#include "SIP.h"
#include "../FileSystem_Header/FileSystem.h"
class Chess
{
public:
	bool Cross[10][10];
	int cross[10][10];
	///����״̬ ��recursion.cpp�ļ���
	void reduceRecursionTimes();
	void setStatus(int RivalLine, int RivalColumn);
	std::shared_ptr<PACE> Tail;
	FileSystem FileS;
	std::shared_ptr<SITUATION> SituaHead;
	std::shared_ptr<SITUATION> TempTail;

	QUADRANT Qua;
	
	void ClearSitua();
};
#endif // CHESS_H_INCLUDED
