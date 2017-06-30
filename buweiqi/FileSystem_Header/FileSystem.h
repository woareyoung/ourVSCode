#pragma once
#include "../stdafx.h"
#include <fstream>
#include <string>
#include <memory>
#include <iostream>
#include "../ChessBoard_Header/SIP.h"

class FileSystem
{
public:
	std::fstream TempFile;//��ʱ�ļ�����
	FileName FN;//�ļ����ṹ��
	int value[10];//�����ݴ���ļ���ȡ������ֵ��value[0]���ڼ�¼��һ��
	/*
	param[FileName]:��Ҫ�򿪵��ļ���
	param[f]:�������ļ����ļ�������
	*/
	void OpenFile(std::string FileName, std::fstream &f);//���ļ�
	/*
	param[header]:��¼����һ����Ϸ�����¼�������ͷ
	param[Winner]:ʤ����
	*/
	void AddMemory(std::shared_ptr<SITUATION> header, int Winner);//����ǰ��һ����Ϸ��ӵ��ܼ�����ļ�����Ϸ����ʱ���ã�
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
	std::shared_ptr<NEXTPACE> Match(SITUATION &StatusQuo, int player, int round);//ƥ��ָ������״�������ף����ء���һ�����ӡ�
	/*
	param[StatusQuo]:��¼��ǰ����״���Ľṹ��
	param[player]:ƥ��������������1�������2
	param[round]:��ǰ�غ���
	return:����ָ�������л�û�����ӵ�λ��
	*/
	std::shared_ptr<NEXTPACE> GenerMatch(SITUATION &StatusQuo, int player, int round);//ƥ�京��ָ������״�������ף�����ָ�������л�û�����ӵ�λ��
	/*
	param[sit]:����״��
	param[change]:�Ƿ���Ҫ����ֵת��
	return:���ظ�������true�����򷵻�false
	*/
	bool Repeat(std::shared_ptr<SITUATION> sit, bool change = false);//�鿴�Ƿ��ظ� ���ǣ�����true
	/*
	param[value]:��Ҫ��ѹ����
	return:�����е�����
	*/
	std::shared_ptr<NEXTPACE> UnPack(int value);//��ѹ
	/*
	param[FileValue]:�ļ��е���ֵ
	param[CurrentValue]:��ǰ�������ֵ
	param[head]:�Ӽ������ڵ�λ�õ�����ͷ���
	param[rear]:�Ӽ������ڵ�λ�õ�����β���
	*/
	bool Compare(int FileValue, int CurrentValue, std::shared_ptr<NEXTPACE> head, std::shared_ptr<NEXTPACE> rear);//�Ƚ�������ֵ���鿴�ļ������Ƿ������ǰ״��
	/*
	param[HeadNode]:�����ͷ���
	*/
	void ClearList(std::shared_ptr<NEXTPACE> HeadNode);//�������
};
