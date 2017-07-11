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
	///ģ������
	cross[line][column] = PlayerId;//ģ������
	Statistic(line, column);
	GetCurrentStatus(Qua.GetMaxQuadrant());//��ȡ����״������
	SITUATION TempData = NowStatus;
	TempData.DigitalChange();
	loseCount = FS.CountNumber(TempData, false);//����ʧ��Ƶ��
	winCount = FS.CountNumber(NowStatus, true);//����ʤ��Ƶ��
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
