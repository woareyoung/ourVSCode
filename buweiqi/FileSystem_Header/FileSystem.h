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
	std::fstream TempFile;//临时文件对象
	FileName FN;//文件名结构体
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
	param[num]:需要转换的数
	return:转换后的数
	*/
	int DigitalChange(int num);//对数值进行高位与低位部分的交换
	/*
	param[StatusQuo]:记录当前盘面状况的结构体
	param[player]:匹配记忆的玩家是玩家1还是玩家2
	param[round]:当前回合数
	return:返回“下一步”链表
	*/
	std::shared_ptr<NEXTPACE> Match(SITUATION &StatusQuo, int player, int round);//匹配指定盘面状况的棋谱，返回“下一步棋子”
	/*
	param[StatusQuo]:记录当前盘面状况的结构体
	param[player]:匹配记忆的玩家是玩家1还是玩家2
	param[round]:当前回合数
	return:返回指定盘面中还没有棋子的位置
	*/
	std::shared_ptr<NEXTPACE> GenerMatch(SITUATION &StatusQuo, int player, int round);//匹配含有指定盘面状况的棋谱，返回指定盘面中还没有棋子的位置
	/*
	param[sit]:盘面状况
	param[change]:是否需要将数值转换
	return:若重复，返回true；否则返回false
	*/
	bool Repeat(std::shared_ptr<SITUATION> sit, bool change = false);//查看是否重复 若是，返回true
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
	bool Compare(int FileValue, int CurrentValue, std::shared_ptr<NEXTPACE> head, std::shared_ptr<NEXTPACE> rear);//比较两个数值，查看文件棋谱是否包含当前状况
	/*
	param[HeadNode]:链表的头结点
	*/
	void ClearList(std::shared_ptr<NEXTPACE> HeadNode);//清空链表
};
