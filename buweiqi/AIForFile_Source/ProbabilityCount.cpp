#include "../stdafx.h"
#include "../AI1_Header/AIForFile.h"
#include "../ChessBoard_Header/SIP.h"
#include <future>
//ʤ�ʼ���
double AIForFile::ProbabilityCount(int site)
{
	double probability = 0;
	return probability;
}

void AIForFile::ForeReadFileToMemory(int round)
{
	ForeReadFinish = false;//���Ԥ����������ִ��
	std::string filename = FS.FN.ForeName + std::to_string(round) + FS.FN.TXT;//�ļ���
	std::fstream file(filename, std::ios::out | std::ios::in);//�ļ�����
	if (!file)
	{
		file.close();
		ForeReadHaveMem = false;
		ForeReadFinish = true;
		return;
	}
	ForeReadContent.clear();//���������
	std::string tempData;
	int site, winner;
	file.seekg(0);
	while (!file.eof())
	{
		file >> tempData >> site >> winner;
		if (winner == 1 || winner == 2)
			ForeReadContent.emplace(tempData, std::make_pair(site, winner));
		else break;
	}
	if (ForeReadContent.empty()) ForeReadHaveMem = false;
	else ForeReadHaveMem = true;
	ForeReadFinish = true;
}