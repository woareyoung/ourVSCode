#pragma once
#ifndef PLAYERINFO_H_INCLUDED
#define PLAYERINFO_H_INCLUDED

class PlayerInfo
{
public:
	bool Start;//�Ƿ��ѿ�ʼ��Ϸ
	bool Player1isAI;//���1�Ƿ���AI
	bool Player2isAI;//���2�Ƿ���AI

	int AllTime1;//���1��ʣ��ʱ��
	int AllTime2;//���2��ʣ��ʱ��
	int Round1;//���1ʣ�ಽʱ
	int Round2;//���2ʣ�ಽʱ

	int Winner;//ʤ����
};
#endif // PLAYERINFO_H_INCLUDED
