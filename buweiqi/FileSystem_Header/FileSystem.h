#pragma once
#include "../stdafx.h"
#include <fstream>
#include "../ChessBoard_Header/SIP.h"

class FileSystem
{
public:
	std::fstream AIMemory; //�����ܼ���ֿ��ļ�����¼�ж��������1Ϊʤ����Ϊ��׼��
	std::fstream TempFile;//��������������ļ�
	std::fstream TempHouse1;//������ʱ������ļ�
	std::fstream TempHouse2;//������ʱ������ļ�
	std::fstream *NowTempHouse;//��ǰ��ʱԴ�����

	char *AIMemoryFileName = "AIMemory.txt";//�ܼ�����ļ���������ֻ���Ӳ����ٵ��ļ���
	char *TempFileName = "TempFile.txt";//��ʱ����������ļ�����¼�뵱ǰ����״��һģһ���ļ��䣩
	char *TempHouse1Name = "TempHouse1.txt";//��ʱ������ļ�
	char *TempHouse2Name = "TempHouse2.txt";//��ʱ������ļ�
	double value[10];//�����ݴ���ļ���ȡ������ֵ

	FileSystem();//���캯��
	/*
	param[FileName]:��Ҫ�򿪵��ļ���
	param[f]:�������ļ����ļ�������
	*/
	void OpenFile(char *FileName, std::fstream &f);//���ļ�
	void CloseFile();//�ر������ļ�����Ϸ����ʱ���ã�
	/*
	param[StatusQuo]:��¼��ǰ����״���Ľṹ��
	param[player]:ƥ��������������1�������2
	*/
	void Match(SITUATION &StatusQuo, int player);//���ҷ��ϵ�ǰ����״̬�ļ������ļ�����ַ
	/*
	param[fst]:Ŀ����ʱ������ļ�
	param[filename]:Ŀ�������ļ���
	*/
	void AddFile(std::fstream &fst, char* filename);//�����ϵ�ǰ����״����δ���ֹ�������ļ���ת�Ƶ���һ����ʱ������ļ�
	/*
	param[header]:��¼����һ����Ϸ�����¼�������ͷ
	param[Winner]:ʤ����
	*/
	void AddMemory(SITUATION *header, int Winner);//����ǰ��һ����Ϸ��ӵ��ܼ�����ļ�����Ϸ����ʱ���ã�
	/*
	param[num]:��Ҫת������
	*/
	double DigitalChange(double num);//����ֵ����С�����������ֵĽ���
};
