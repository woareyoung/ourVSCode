#pragma once
#include "../stdafx.h"
#include <fstream>
#include <string>
#include "../ChessBoard_Header/SIP.h"

class FileSystem
{
public:
	std::fstream TempFile;//临时文件对象
	FileName FN;//文件名结构体
	double value[10];//用于暂存从文件读取到的数值

	/*
	param[FileName]:需要打开的文件名
	param[f]:关联此文件的文件流对象
	*/
	void OpenFile(std::string FileName, std::fstream &f);//打开文件
	/*
	param[header]:记录着整一局游戏走棋记录的链表表头
	param[Winner]:胜利者
	*/
	void AddMemory(SITUATION *header, int Winner);//将当前这一局游戏添加到总记忆库文件（游戏结束时调用）
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
	NEXTPACE* Match(SITUATION &StatusQuo, int player, int round);//查找符合当前棋盘状态的记忆库的文件流地址
	/*
	param[sit]:盘面状况
	param[change]:是否需要将数值转换
	return:若重复，返回true；否则返回false
	*/
	bool Repeat(SITUATION *sit, bool change = false);//查看是否重复 若是，返回true
};
