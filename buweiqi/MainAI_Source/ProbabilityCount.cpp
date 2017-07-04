#include "../ChessBoard_Header/AI.h"
#include "../ChessBoard_Header/SIP.h"
#include <future>

double AI::ProbabilityCount(int site)
{
	SITUATION SaveData = NowStatus;//�������ݣ��൱��ѹջ��
	double probability = 0;
	int line = site / 10;
	int column = site % 10;
	int winCount = 0;
	int loseCount = 0;
	bool wait = true;
	///ģ������
	cross[line][column] = PlayerId;//ģ������
	Statistic(line, column);
	GetCurrentStatus(Qua.GetMaxQuadrant());//��ȡ����״������
	//����ʧ��Ƶ��
	std::async(std::launch::async, [&] () 
	{
		FS.Match(NowStatus, loseCount, CurrentRound + 1, false);
		wait = false;
	});
	FS.GenerMatch(NowStatus, winCount, CurrentRound + 2);
	while (wait) {}//�ȴ��߳�ִ�����
	probability = winCount / ((double)winCount + (double)loseCount);//����ʤ��Ƶ��
	///�ָ�����
	cross[line][column] = 0;
	BackQua(line, column);
	NowStatus = SaveData;
	return probability;
}