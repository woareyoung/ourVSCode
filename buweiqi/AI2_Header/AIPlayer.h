#pragma once

class AIPlayer {
private:
protected:
	// 轮到谁
	int turn2Who;
	// 队手
	int Rival;
	//记录各交叉点的值，数组访问从“1”开始，访问顺序为“先行后列”，

	int PlayerId;// 当前AI的身份是黑子还是白子
public:

	int getRole() {
		return PlayerId;
	}
	void setRole(int id) {
		PlayerId = id;
	}
};

