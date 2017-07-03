#pragma once
#ifndef WINDOWHWND_H_INCLUDED
#define WINDOWHWND_H_INCLUDED
#include "../stdafx.h"
#include <fstream>
#include <string>

class WindowHwnd
{
public:
	HWND BackHwnd;//回退一步按钮
	HWND RootHwnd;//应用程序主窗口
	HWND TimerAll1Hwnd;//玩家1总时间信息
	HWND TimerAll2Hwnd;//玩家2总时间信息
	HWND TimerRound1Hwnd;//玩家1步时信息
	HWND TimerRound2Hwnd;//玩家2步时信息
	HWND Timer1A;//玩家1剩余时间字样
	HWND Timer2A;//玩家2剩余时间字样
	HWND Timer1R;//玩家1剩余时间字样
	HWND Timer2R;//玩家2剩余时间字样
	HWND Player1Hwnd;//玩家1“player1”字样
	HWND Player2Hwnd;//玩家2“player2”字样
	HWND Player1UseAI;//玩家1使用AI按钮
	HWND Player2UseAI;//玩家2使用AI按钮
	HWND ChessBoardHwnd;//棋盘区域
	HWND StartGame;//开始游戏按钮
	HWND TurnToBlack;//轮到玩家1的字样
	HWND TurnToWhite;//轮到玩家2的字样

	HWND SelectAI;//选择AI窗口
	HWND SelectAI1;//选择AI1按钮，在选择AI窗口中
	HWND SelectAI2;//选择AI2按钮，在选择AI窗口中
	HWND SelectAI3;//选择AI3按钮，在选择AI窗口中

	HWND CombatRecordButton;//对弈战绩按钮
	HWND CombatRecord;//对弈战绩窗口
	HWND AI1_VS_AI1;
	HWND AI1_VS_AI2;
	HWND AI1_VS_AI3;
	HWND AI2_VS_AI1;
	HWND AI2_VS_AI2;
	HWND AI2_VS_AI3;
	HWND AI3_VS_AI1;
	HWND AI3_VS_AI2;
	HWND AI3_VS_AI3;
	HWND CurrentCR;//“当前战绩”字样
	HWND AllCR;//“历史战绩”字样
	HWND CurrentWin;
	HWND CurrentLose;
	HWND AllWin;
	HWND AllLose;
	HWND DataHwnd[36];//战绩数据

	HINSTANCE hInst;//应用程序实例句柄
	HANDLE AnoHandle;//计时器线程
	HANDLE MainProcedureThead;//画棋子线程
	bool PrintTime;
	RECT MainWinRect;//记录主窗口位置
	HDC hdcBuffer;//缓冲
	HBITMAP hBitMap;//内存位图
	RECT rect;//矩形坐标
	PAINTSTRUCT ps;
	HDC hdc;//显示设备
	HBITMAP BlackChess;//保存黑子的图片
	HBITMAP WhiteChess;//保存白子的图片
	HBITMAP Board;//保存棋盘的图片
	HBITMAP Tips;//保存提示位图
	//是否可以选AI
	bool CanSelectAI1;
	bool CanSelectAI2;
	bool CanSelectAI3;
	//刷新对战记录
	void UpdateCRdata()
	{
		using namespace std;
		string data[36];
		fstream tempfile;
		tempfile.open("CombatRecord.txt", ios::out | ios::in);
		if (!tempfile) 
		{
			tempfile.close();
			tempfile.open("CombatRecord.txt", ios::out);//创建文件
			tempfile.close();
			return; 
		}
		for (int i = 0; i < 36; i++)
		{
			tempfile >> data[i];
			SendMessageA(DataHwnd[i], WM_SETTEXT, (WPARAM)NULL, LPARAM(data[i].c_str()));
		}
		tempfile.close();
	}
	//刷新对战记录
	void UpdateCRdata(int Player1, int Player2, int winner)
	{
		using namespace std;
		int data[36] = { 0 };
		int i;
		fstream tempfile;
		tempfile.open("CombatRecord.txt", ios::out | ios::in);
		if (!tempfile)
		{
			tempfile.close();
			tempfile.open("CombatRecord.txt", ios::out);//创建文件
		}
		else
		{
			tempfile.seekg(0);
			for (i = 0; i < 36; i++) tempfile >> data[i];
		}
		if (winner == 1)
		{
			if (Player1 == 1)
			{
				if (Player2 == 1) { data[0]++; data[18]++; }
				else if (Player2 == 2) { data[1]++; data[19]++; }
				else { data[2]++; data[20]++; }
			}
			else if (Player1 == 2)
			{
				if (Player2 == 1) { data[3]++; data[21]++; }
				if (Player2 == 2) { data[4]++; data[22]++; }
				else { data[5]++; data[23]++; }
			}
			else if (Player1 == 3)
			{
				if (Player2 == 1) { data[6]++; data[24]++; }
				else if (Player2 == 2) { data[7]++; data[25]++; }
				else { data[8]++; data[26]++; }
			}
		}
		else 
		{
			if (Player1 == 1)
			{
				if (Player2 == 1) { data[9]++; data[27]++; }
				else if (Player2 == 2) { data[10]++; data[28]++; }
				else { data[11]++; data[29]++; }
			}
			else if (Player1 == 2)
			{
				if (Player2 == 1) { data[12]++; data[30]++; }
				if (Player2 == 2) { data[13]++; data[31]++; }
				else { data[14]++; data[32]++; }
			}
			else if (Player1 == 3)
			{
				if (Player2 == 1) { data[15]++; data[33]++; }
				else if (Player2 == 2) { data[16]++; data[34]++; }
				else { data[17]++; data[35]++; }
			}
		}
		tempfile.close();
		tempfile.open("CombatRecord.txt", ios::out);
		for (i = 0; i < 36; i++)
		{
			if (i % 6 == 0) tempfile << endl;
			tempfile << data[i] << " ";
		}
		tempfile.close();
		UpdateCRdata();
	}
};


#endif // WINDOWHWND_H_INCLUDED
