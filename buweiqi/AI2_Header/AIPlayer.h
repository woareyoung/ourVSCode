#pragma once

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
	//��¼��������ֵ��������ʴӡ�1����ʼ������˳��Ϊ�����к��С���

	int PlayerId;// ��ǰAI������Ǻ��ӻ��ǰ���
public:
	AIPlayer() : ourEyes(0), RivalEyes(0)
	{}
};

