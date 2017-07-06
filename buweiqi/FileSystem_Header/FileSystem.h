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
	param[count]:�ܹ��ж��ٸ���¼���
	param[round]:��ǰ�غ���
	param[isWinner]:�Ƿ���ʤ����ƥ��
	return:���ء���һ��������
	*/
	std::shared_ptr<NEXTPACE> Match(SITUATION &StatusQuo, int &count, int round, bool isWinner = true);//ƥ��ָ������״�������ף����ء���һ�����ӡ�
	/*
	param[StatusQuo]:��¼��ǰ����״���Ľṹ��
	param[count]:�ܹ��ж��ٸ���¼���
	param[round]:��ǰ�غ���
	param[needptr]:�Ƿ���Ҫ���������ڼ���Ƶ����ʱ��Ϊfalse��
	return:����ָ�������л�û�����ӵ�λ��
	*/
	std::shared_ptr<NEXTPACE> GenerMatch(SITUATION &StatusQuo, int &count, int round, bool needptr = true);//ƥ�京��ָ������״�������ף�����ָ�������л�û�����ӵ�λ��
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
	bool CompareHigh(int FileValue, int CurrentValue, std::shared_ptr<NEXTPACE> head, std::shared_ptr<NEXTPACE> rear, bool needptr = true);//�Ƚ�������ֵ���鿴�ļ������Ƿ������ǰ״��
	//param[FileValue]:�ļ��е���ֵ
	//param[CurrentValue] : ��ǰ�������ֵ
	bool CompareLow(int FileValue, int CurrentValue);
	/*
	param[HeadNode]:�����ͷ���
	*/
	void ClearList(std::shared_ptr<NEXTPACE> HeadNode);//�������
	/*��Sourceƴ��Aim
	param[Aim]:Ŀ������
	param[Source]:Դ����
	*/
	void ConnectList(std::shared_ptr<NEXTPACE> Aim, std::shared_ptr<NEXTPACE> Source);//�������
};
