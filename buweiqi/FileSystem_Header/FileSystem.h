#pragma once
#include "../stdafx.h"
#include <fstream>
#include "../ChessBoard_Header/PACE_struct.h"
///��¼��ǰ����״��
struct SITUATION
{
	double FIrstLine;
	double SecondLine;
	double ThirdLine;
	double ForthLine;
	double FivethLine;
	double SixthLine;
	double seventhLine;
	double EighthLine;
	double ninthLine;
};
class FileSystem
{
	std::fstream AIMemory; //��������ֿ��ļ�
	std::fstream TempFile1;//������ʱ���Ҽ����
	std::fstream TempFIle2;//������ʱ���Ҽ����

	char *AIMemoryFileName;//������ļ���������ֻ���Ӳ����ٵ��ļ���
	char *TempFile1Name;//��ʱ���Ҽ�����ļ�
	char *TempFile2Name;//��ʱ���Ҽ�����ļ�
public:
	/*
	param[FileName]:��Ҫ�򿪵��ļ���
	param[f]:�������ļ����ļ�������
	*/
	void OpenFile(char *FileName, std::fstream &f);//���ļ�
	void CloseFile();//�ر������ļ�����Ϸ����ʱ���ã�
	/*
	param[StatusQuo]:��¼��ǰ����״���Ľṹ��
	return value:���ط��ϵ�ǰ����״�����ļ�������
	*/
	std::fstream Match(SITUATION &StatusQuo);//���ҷ��ϵ�ǰ����״̬�ļ���
	/*
	param[header]:��¼����һ����Ϸ�����¼�������ͷ
	*/
	void AddMemory(PACE *header);//����ǰ��һ����Ϸ��ӵ��ܼ�����ļ�����Ϸ����ʱ���ã�
};