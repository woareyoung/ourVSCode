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
		tmp1 = FS.GenerMatch(NowStatus, loseCount, CurrentRound + 3, false);
		wait = false;
	});
	tmp2 = FS.GenerMatch(TempData, winCount, CurrentRound + 2, false);
	while (wait) {}//�ȴ��߳�ִ�����
	if (winCount == 0 && loseCount == 0) probability = 0.000001;
	else probability = winCount / ((double)winCount + (double)loseCount);//����ʤ��Ƶ��
	///�ָ�����
	cross[line][column] = 0;
	BackQua(line, column);
	NowStatus = SaveData;
	tmp1 = nullptr;
	tmp2 = nullptr;
	return probability;
}