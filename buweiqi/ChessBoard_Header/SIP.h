#pragma once
struct PACE
{
	int line;
	int column;
	int player;
	PACE *next;
	PACE *perior;
};
///��¼��ǰ����״��
struct SITUATION
{
	double Line[10] = { 0 };//9�еļ�¼�����������0����¼�����λ�ã�
	SITUATION *next;
	SITUATION *prior;
};