#pragma once
#ifndef AIPLAYER_H_INCLUDED
#define AIPLAYER_H_INCLUDED

class AIPlayer {
private:
protected:
	// �ֵ�˭
	int turn2Who;
	// ����
	int Rival;
	// �ҷ����۵�����
	int ourEyes;
	// �з����۵�����
	int RivalEyes;
public:
	AIPlayer() : ourEyes(0), RivalEyes(0)
	{}
};

#endif // AIPLAYER_H_INCLUDED
