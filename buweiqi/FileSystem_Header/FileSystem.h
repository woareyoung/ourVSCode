#pragma once
#include "../stdafx.h"
#include <fstream>
#include <string>
#include "../ChessBoard_Header/SIP.h"

class FileSystem
{
public:
	std::fstream TempFile;//��ʱ�ļ�����
	FileName FN;//�ļ����ṹ��
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
	return:ת�������
	*/
	int DigitalChange(int num);//����ֵ���и�λ���λ���ֵĽ���
	/*
	param[StatusQuo]:��¼��ǰ����״���Ľṹ��
	param[player]:ƥ��������������1�������2
	param[round]:��ǰ�غ���
	return:���ء���һ��������
	*/
	NEXTPACE* Match(SITUATION &StatusQuo, int player, int round);//���ҷ��ϵ�ǰ����״̬�ļ������ļ�����ַ
	/*
	param[sit]:����״��
	param[change]:�Ƿ���Ҫ����ֵת��
	return:���ظ�������true�����򷵻�false
	*/
	bool Repeat(SITUATION *sit, bool change = false);//�鿴�Ƿ��ظ� ���ǣ�����true
};
