#pragma once
#ifndef AI_H_INCLUDED
#define AI_H_INCLUDED
#include "SIP.h"
#include "../FileSystem_Header/FileSystem.h"

class AI
{
public:
	bool link;//����Ƿ��Ѵ��ļ�
	FileSystem FS; //�ļ���������
	QUADRANT Qua; //��¼�����¼
	virtual void GetPosition(int &line, int &column, int onTurn) = 0;
};

#endif // AI_H_INCLUDED
