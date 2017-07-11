#pragma once
#include "../stdafx.h"
#include <fstream>
#include <string>
#include <memory>
#include <iostream>
#include <future>
#include <set>
#include "../ChessBoard_Header/SIP.h"

#define ProThreadNumber 4//线程数
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
	std::fstream TempFile;//临时文件对象
	std::fstream FinalFile[4];//用于游戏结束时写记录进文件的文件对象
	FileName FN;//文件名结构体
	std::shared_ptr<MEMO> mem;//头结点
	std::shared_ptr<DISKSTATUS> ProHeadWin;//用于计算胜率时的多线程
	std::shared_ptr<DISKSTATUS> ProHeadLose;//用于计算胜率时的多线程
	
	int value[10];//用于暂存从文件读取到的数值，value[0]用于记录下一步
	/*
	param[FileName]:需要打开的文件名
	param[f]:关联此文件的文件流对象
	*/
	void OpenFile(std::string FileName, std::fstream &f);//打开文件
	/*
	param[header]:记录着整一局游戏走棋记录的链表表头
	param[Winner]:胜利者
	*/
	void AddMemory(std::shared_ptr<SITUATION> header, int Winner);//将当前这一局游戏添加到总记忆库文件（游戏结束时调用）
	/*
	param[s]:对局记录链表的头结点
	param[change]:是否需要进行高低位转换
	*/
	void DistributeThread(std::shared_ptr<SITUATION> header, bool change);//创建多线程并分配工作
	/*
	param[header]:对局记录链表中的结点
	param[j]:回合数
	param[change]:是否需要进行高低位转换
	param[ThreadNumber]:线程编号
	*/
	void ReadMemoryToFile(std::shared_ptr<SITUATION> s, int j, bool change, int ThreadNumber);//修改文件中的内容
	/*
	param[num]:需要转换的数
	return:转换后的数
	*/
	int DigitalChange(int num);//对数值进行高位与低位部分的交换
	/*
	param[StatusQuo]:记录当前盘面状况的结构体
	param[count]:总共有多少个记录相符
	param[round]:当前回合数
	param[isWinner]:是否以胜利方匹配
	return:返回“下一步”链表
	*/
	int Match(SITUATION &StatusQuo, std::set<int> &result, int round, bool isWinner);//匹配指定盘面状况的棋谱，返回“下一步棋子”
	/*
	param[StatusQuo]:记录当前盘面状况的结构体
	param[count]:总共有多少个记录相符
	param[round]:当前回合数
	param[needptr]:是否需要链表（当用于计算频数的时候为false）
	return:返回指定盘面中还没有棋子的位置
	*/
	int GenerMatch(SITUATION &StatusQuo, std::set<int> &Parent, int round, bool needptr);//匹配含有指定盘面状况的棋谱，返回指定盘面中还没有棋子的位置
	/*
	param[sit]:盘面状况
	param[change]:是否需要将数值转换
	return:若重复，返回true；否则返回false
	*/
	bool Repeat(std::shared_ptr<SITUATION> sit, std::shared_ptr<MEMO> tempnode, bool change = false);//查看是否重复 若是，返回true
	/*
	param[value]:需要解压的数
	return:返回列的链表
	*/
	std::shared_ptr<NEXTPACE> UnPack(int value);//解压
	/*
	param[FileValue]:文件中的数值
	param[CurrentValue]:当前盘面的数值
	param[head]:子集不存在的位置的链表头结点
	param[rear]:子集不存在的位置的链表尾结点
	*/
	bool CompareHigh(int FileValue, int CurrentValue, std::set<int> &stl, bool needptr = true);//比较两个数值，查看文件棋谱是否包含当前状况
	//param[FileValue]:文件中的数值
	//param[CurrentValue] : 当前盘面的数值
	bool CompareLow(int FileValue, int CurrentValue);
	/*
	param[HeadNode]:链表的头结点
	*/
	void ClearList(std::shared_ptr<NEXTPACE> HeadNode);//清空链表
	/*将Source拼入Aim
	param[Aim]:目的链表
	param[Source]:源链表
	*/
	void ConnectList(std::set<int> &Aim, std::shared_ptr<NEXTPACE> Source);//链表接力
	/*
	param[NowRound]：回合数
	*/
	void ReadFileToMemory(int NowRound);//将文件内容读取到内存中
	/*
	param[Head]：链表头结点
	param[filename]：文件名
	*/
	void ReadFile(int HeadNumber, std::string filename);//读取文件并建立链表
	/*
	param[Head]：链表头结点
	*/
	void ClearLIST(std::shared_ptr<DISKSTATUS> Head);//清空链表
	/*
	param[sit]:盘面状况
	*/
	int CountNumber(SITUATION &sit, bool win);
};
