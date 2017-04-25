#pragma once
#ifndef PLAYERINFO_H_INCLUDED
#define PLAYERINFO_H_INCLUDED

class PlayerInfo
{
public:
	bool Start;//是否已开始游戏
	bool Player1isAI;//玩家1是否是AI
	bool Player2isAI;//玩家2是否是AI

	int AllTime1;//玩家1总剩余时间
	int AllTime2;//玩家2总剩余时间
	int Round1;//玩家1剩余步时
	int Round2;//玩家2剩余步时

	int Winner;//胜利者
};
#endif // PLAYERINFO_H_INCLUDED
