#pragma once
#include "../stdafx.h"
#include <fstream>
#include "../ChessBoard_Header/SIP.h"

class FileSystem
{
public:
	std::fstream AIMemory; //关联总记忆仓库文件（记录中都是以玩家1为胜利者为基准）
	std::fstream TempFile;//关联辅助记忆库文件
	std::fstream TempHouse1;//关联临时记忆库文件
	std::fstream TempHouse2;//关联临时记忆库文件
	std::fstream *NowTempHouse;//当前临时源记忆库

	char *AIMemoryFileName = "AIMemory.txt";//总记忆库文件（即内容只增加不减少的文件）
	char *TempFileName = "TempFile.txt";//临时辅助记忆库文件（记录与当前盘面状况一模一样的记忆）
	char *TempHouse1Name = "TempHouse1.txt";//临时记忆库文件
	char *TempHouse2Name = "TempHouse2.txt";//临时记忆库文件
	double value[10];//用于暂存从文件读取到的数值

	FileSystem();//构造函数
	/*
	param[FileName]:需要打开的文件名
	param[f]:关联此文件的文件流对象
	*/
	void OpenFile(char *FileName, std::fstream &f);//打开文件
	void CloseFile();//关闭所有文件（游戏结束时调用）
	/*
	param[StatusQuo]:记录当前盘面状况的结构体
	param[player]:匹配记忆的玩家是玩家1还是玩家2
	*/
	void Match(SITUATION &StatusQuo, int player);//查找符合当前棋盘状态的记忆库的文件流地址
	/*
	param[fst]:目标临时记忆库文件
	param[filename]:目标记忆库文件名
	*/
	void AddFile(std::fstream &fst, char* filename);//将符合当前盘面状况或未出现过的盘面的记忆转移到另一个临时记忆库文件
	/*
	param[header]:记录着整一局游戏走棋记录的链表表头
	param[Winner]:胜利者
	*/
	void AddMemory(SITUATION *header, int Winner);//将当前这一局游戏添加到总记忆库文件（游戏结束时调用）
	/*
	param[num]:需要转换的数
	*/
	double DigitalChange(double num);//对数值进行小数与整数部分的交换
};
