#pragma once
#ifndef AIPLAYER_H_INCLUDED
#define AIPLAYER_H_INCLUDED

class AIPlayer {
private:
protected:
	// 轮到谁
	int turn2Who;
	// 队手
	int Rival;
	// 我方的眼的数量
	int ourEyes;
	// 敌方的眼的数量
	int RivalEyes;
public:
	AIPlayer() : ourEyes(0), RivalEyes(0)
	{}
};

#endif // AIPLAYER_H_INCLUDED
