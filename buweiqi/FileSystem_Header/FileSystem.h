#pragma once
#include "../stdafx.h"
#include <fstream>
#include <string>
#include <memory>
#include <iostream>
#include <future>
#include <set>
#include <unordered_map>
#include <set>
#include <thread>
#include "../ChessBoard_Header/SIP.h"
#define OpenFileThreadNumber 4 //���ļ��߳���
#define PredictPaceNumber 7 //Ԥ�ⲽ����
class FileSystem
{
public:
	std::fstream TempFile;//��ʱ�ļ�����
	FileName FN;//�ļ����ṹ��

	std::fstream CountFile[OpenFileThreadNumber];//���ڶ��߳����ļ���ȡ
	//���ڶ��߳����ļ����ݵĴ洢
	std::unordered_multimap<std::string, int> CountMap[OpenFileThreadNumber]; //������
	std::unordered_multimap<std::string, int> CCountMap[OpenFileThreadNumber];//�������������������ò��������
	bool CountMapUseLack = false;//����������Ƿ񲻹���

	/*
	param[FileName]:��Ҫ�򿪵��ļ���
	param[f]:�������ļ����ļ�������
	*/
	void OpenFile(std::string FileName, std::fstream &f);//���ļ�
	/*
	param[header]:��¼����һ����Ϸ�����¼�������ͷ
	*/
	void AddMemory(std::shared_ptr<SITUATION> header, int winner);//����ǰ��һ����Ϸ��ӵ��ܼ�����ļ�����Ϸ����ʱ���ã�
	/*
	param[StatusQuo]:��¼��ǰ����״���Ľṹ��
	param[result]:��һ�����ӵļ���
	param[round]:��ǰ�غ���
	param[playerId]:��ұ��
	*/
	void Match(SITUATION &StatusQuo, std::set<int> &result, int round, int playerId);//ƥ��ָ������״�������ף���ȡ����һ�����ӡ�
	/*
	param[StatusQuo]:��¼��ǰ����״���Ľṹ��
	param[Parent]:�������е���ǰ����û�еġ��ҡ�������λ��
	param[round]:��ǰ�غ���
	param[playerId]:��ұ��
	*/
	void GenerMatch(SITUATION &StatusQuo, std::set<int> &Parent, int round, int playerId);//ƥ�京��ָ������״�������ף�����ָ�������л�û�����ӵ�λ��
	/*
	param[OneData]:��ѹ�����1������λ��
	param[TwoData]:��ѹ�����2������λ��
	*/
	void UnPack(std::string data, std::set<int> &OneData, std::set<int> &TwoData);//��ѹ
	/*
	param[FileValue]:�ļ��е�����״��
	param[CurrentValue]:��ǰ����״��
	param[stl]:�������е���ǰ����û�еġ��ҡ�������λ��
	return:�����Ƿ������ǰ����״��
	*/
	bool Compare(std::set<int> &FileValue, std::set<int> &CurrentValue, std::set<int> &stl);//�Ƚ���������״�����鿴�ļ������Ƿ������ǰ״��
	/*
	param[round]���غ���
	*/
	void ReadFile(int round);//��ĳ���غϿ�ʼ����ThreadNumber�����ļ��궨�壩���ļ���ȡ��������
};
