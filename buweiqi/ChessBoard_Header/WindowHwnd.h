#pragma once
#ifndef WINDOWHWND_H_INCLUDED
#define WINDOWHWND_H_INCLUDED
#include "../stdafx.h"

class WindowHwnd
{
public:
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
	HINSTANCE hInst;//应用程序实例句柄
	HANDLE TimeHandle;//计时器线程
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
};


#endif // WINDOWHWND_H_INCLUDED
