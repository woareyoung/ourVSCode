#pragma once
#include "../stdafx.h"
#include <fstream>
#include <string>
#include "../ChessBoard_Header/SIP.h"

class FileSystem
{
public:
	std::fstream TempFile;
	FileName FN;
	double value[10];//�����ݴ���ļ���ȡ������ֵ

	/*
	param[FileName]:��Ҫ�򿪵��ļ���
	param[f]:�������ļ����ļ�������
	*/
	void OpenFile(std::string FileName, std::fstream &f);//���ļ�
	/*
	param[header]:��¼����һ����Ϸ�����¼�������ͷ
	param[Winner]:ʤ����
	*/
	void AddMemory(SITUATION *header, int Winner);//����ǰ��һ����Ϸ��ӵ��ܼ�����ļ�����Ϸ����ʱ���ã�
	/*
	param[num]:��Ҫת������
	*/
	double DigitalChange(double num);//����ֵ����С�����������ֵĽ���
	/*
	param[StatusQuo]:��¼��ǰ����״���Ľṹ��
	param[player]:ƥ��������������1�������2
	*/
	void Match(SITUATION &StatusQuo, int player, int round);//���ҷ��ϵ�ǰ����״̬�ļ������ļ�����ַ
};
