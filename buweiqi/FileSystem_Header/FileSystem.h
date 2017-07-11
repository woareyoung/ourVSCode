#pragma once
#include "../stdafx.h"
#include <fstream>
#include <string>
#include <memory>
#include <iostream>
#include <future>
#include <set>
#include "../ChessBoard_Header/SIP.h"

#define ProThreadNumber 4//�߳���
struct MEMO
{
	int value[10] = { 0 };
	int count = 0;
	std::shared_ptr<MEMO> next;
	std::shared_ptr<MEMO> prior;
};
class FileSystem
{
public:
	std::fstream TempFile;//��ʱ�ļ�����
	std::fstream FinalFile[4];//������Ϸ����ʱд��¼���ļ����ļ�����
	FileName FN;//�ļ����ṹ��
	std::shared_ptr<MEMO> mem;//ͷ���
	std::shared_ptr<DISKSTATUS> ProHeadWin;//���ڼ���ʤ��ʱ�Ķ��߳�
	std::shared_ptr<DISKSTATUS> ProHeadLose;//���ڼ���ʤ��ʱ�Ķ��߳�
	
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
	param[s]:�Ծּ�¼�����ͷ���
	param[change]:�Ƿ���Ҫ���иߵ�λת��
	*/
	void DistributeThread(std::shared_ptr<SITUATION> header, bool change);//�������̲߳����乤��
	/*
	param[header]:�Ծּ�¼�����еĽ��
	param[j]:�غ���
	param[change]:�Ƿ���Ҫ���иߵ�λת��
	param[ThreadNumber]:�̱߳��
	*/
	void ReadMemoryToFile(std::shared_ptr<SITUATION> s, int j, bool change, int ThreadNumber);//�޸��ļ��е�����
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
	int Match(SITUATION &StatusQuo, std::set<int> &result, int round, bool isWinner);//ƥ��ָ������״�������ף����ء���һ�����ӡ�
	/*
	param[StatusQuo]:��¼��ǰ����״���Ľṹ��
	param[count]:�ܹ��ж��ٸ���¼���
	param[round]:��ǰ�غ���
	param[needptr]:�Ƿ���Ҫ���������ڼ���Ƶ����ʱ��Ϊfalse��
	return:����ָ�������л�û�����ӵ�λ��
	*/
	int GenerMatch(SITUATION &StatusQuo, std::set<int> &Parent, int round, bool needptr);//ƥ�京��ָ������״�������ף�����ָ�������л�û�����ӵ�λ��
	/*
	param[sit]:����״��
	param[change]:�Ƿ���Ҫ����ֵת��
	return:���ظ�������true�����򷵻�false
	*/
	bool Repeat(std::shared_ptr<SITUATION> sit, std::shared_ptr<MEMO> tempnode, bool change = false);//�鿴�Ƿ��ظ� ���ǣ�����true
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
	bool CompareHigh(int FileValue, int CurrentValue, std::set<int> &stl, bool needptr = true);//�Ƚ�������ֵ���鿴�ļ������Ƿ������ǰ״��
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
	void ConnectList(std::set<int> &Aim, std::shared_ptr<NEXTPACE> Source);//�������
	/*
	param[NowRound]���غ���
	*/
	void ReadFileToMemory(int NowRound);//���ļ����ݶ�ȡ���ڴ���
	/*
	param[Head]������ͷ���
	param[filename]���ļ���
	*/
	void ReadFile(int HeadNumber, std::string filename);//��ȡ�ļ�����������
	/*
	param[Head]������ͷ���
	*/
	void ClearLIST(std::shared_ptr<DISKSTATUS> Head);//�������
	/*
	param[sit]:����״��
	*/
	int CountNumber(SITUATION &sit, bool win);
};
