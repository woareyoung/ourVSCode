#pragma once
#include "../stdafx.h"
#include <fstream>
#include "../ChessBoard_Header/PACE_struct.h"
///记录当前盘面状况
struct SITUATION
{
	double FIrstLine;
	double SecondLine;
	double ThirdLine;
	double ForthLine;
	double FivethLine;
	double SixthLine;
	double seventhLine;
	double EighthLine;
	double ninthLine;
};
class FileSystem
{
	std::fstream AIMemory; //关联记忆仓库文件
	std::fstream TempFile1;//关联临时查找记忆库
	std::fstream TempFIle2;//关联临时查找记忆库

	char *AIMemoryFileName;//记忆库文件（即内容只增加不减少的文件）
	char *TempFile1Name;//临时查找记忆库文件
	char *TempFile2Name;//临时查找记忆库文件
public:
	/*
	param[FileName]:需要打开的文件名
	param[f]:关联此文件的文件流对象
	*/
	void OpenFile(char *FileName, std::fstream &f);//打开文件
	void CloseFile();//关闭所有文件（游戏结束时调用）
	/*
	param[StatusQuo]:记录当前盘面状况的结构体
	return value:返回符合当前盘面状况的文件流对象
	*/
	std::fstream Match(SITUATION &StatusQuo);//查找符合当前棋盘状态的记忆
	/*
	param[header]:记录着整一局游戏走棋记录的链表表头
	*/
	void AddMemory(PACE *header);//将当前这一局游戏添加到总记忆库文件（游戏结束时调用）
};