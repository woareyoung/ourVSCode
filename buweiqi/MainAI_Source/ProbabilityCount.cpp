#include "../ChessBoard_Header/AI.h"
#include "../ChessBoard_Header/SIP.h"
#include <future>

double AI::ProbabilityCount(int site)
{
	SITUATION SaveData = NowStatus;//�������ݣ��൱��ѹջ��

	std::shared_ptr<NEXTPACE> tmp1 = nullptr, tmp2 = nullptr;
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
	SITUATION TempData = NowStatus;
	TempData.DigitalChange();
	//����ʧ��Ƶ��
	std::async(std::launch::async, [&] () 
	{
		NowStatus.DigitalChange();
		for (int i = CurrentRound + 3; i < 81; i = i + 2) tmp1 = FS.GenerMatch(NowStatus, loseCount, i, false);
		wait = false;
	});
	//����ʤ��Ƶ��
	for(int j = CurrentRound + 2; j < 81; j = j + 2) tmp2 = FS.GenerMatch(TempData, winCount, j, false);
	while (wait) {}//�ȴ��߳�ִ�����
	probability = winCount / ((double)winCount + (double)loseCount);//����ʤ��Ƶ��
	///�ָ�����
	cross[line][column] = 0;
	BackQua(line, column);
	NowStatus = SaveData;
	tmp1 = nullptr;
	tmp2 = nullptr;
	return probability;
}