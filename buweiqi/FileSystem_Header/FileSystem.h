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
#define OpenFileThreadNumber 4 //打开文件线程数
#define PredictPaceNumber 7 //预测步骤数
class FileSystem
{
public:
	std::fstream TempFile;//临时文件对象
	FileName FN;//文件名结构体

	std::fstream CountFile[OpenFileThreadNumber];//用于多线程中文件读取
	//用于多线程中文件内容的存储
	std::unordered_multimap<std::string, int> CountMap[OpenFileThreadNumber]; //主容器
	std::unordered_multimap<std::string, int> CCountMap[OpenFileThreadNumber];//副容器（主容器不够用才用这个）
	bool CountMapUseLack = false;//标记主容器是否不够用

	/*
	param[FileName]:需要打开的文件名
	param[f]:关联此文件的文件流对象
	*/
	void OpenFile(std::string FileName, std::fstream &f);//打开文件
	/*
	param[header]:记录着整一局游戏走棋记录的链表表头
	*/
	void AddMemory(std::shared_ptr<SITUATION> header, int winner);//将当前这一局游戏添加到总记忆库文件（游戏结束时调用）
	/*
	param[StatusQuo]:记录当前盘面状况的结构体
	param[result]:下一步棋子的集合
	param[round]:当前回合数
	param[playerId]:玩家编号
	*/
	void Match(SITUATION &StatusQuo, std::set<int> &result, int round, int playerId);//匹配指定盘面状况的棋谱，获取“下一步棋子”
	/*
	param[StatusQuo]:记录当前盘面状况的结构体
	param[Parent]:棋谱中有但当前盘面没有的“我”的棋子位置
	param[round]:当前回合数
	param[playerId]:玩家编号
	*/
	void GenerMatch(SITUATION &StatusQuo, std::set<int> &Parent, int round, int playerId);//匹配含有指定盘面状况的棋谱，返回指定盘面中还没有棋子的位置
	/*
	param[OneData]:解压后玩家1的棋子位置
	param[TwoData]:解压后玩家2的棋子位置
	*/
	void UnPack(std::string data, std::set<int> &OneData, std::set<int> &TwoData);//解压
	/*
	param[FileValue]:文件中的盘面状况
	param[CurrentValue]:当前盘面状况
	param[stl]:棋谱中有但当前盘面没有的“我”的棋子位置
	return:棋谱是否包含当前盘面状况
	*/
	bool Compare(std::set<int> &FileValue, std::set<int> &CurrentValue, std::set<int> &stl);//比较两个盘面状况，查看文件棋谱是否包含当前状况
	/*
	param[round]：回合数
	*/
	void ReadFile(int round);//从某个回合开始，将ThreadNumber（本文件宏定义）个文件读取到容器中
};
