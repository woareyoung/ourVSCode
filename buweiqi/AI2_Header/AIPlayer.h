#pragma once

class AIPlayer {
private:
protected:
	// �ֵ�˭
	int turn2Who;
	// ����
	int Rival;
	//��¼��������ֵ��������ʴӡ�1����ʼ������˳��Ϊ�����к��С���

	int PlayerId;// ��ǰAI������Ǻ��ӻ��ǰ���
public:

	int getRole() {
		return PlayerId;
	}
	void setRole(int id) {
		PlayerId = id;
	}
};

