#pragma once
#ifndef CHESSBOARD_H_INCLUDED
#define CHESSBOARD_H_INCLUDED
#include "WindowSize.h"
#include "WindowHwnd.h"
#include "PlayerInfo.h"
#include "Chess.h"
#include "AI.h"
#include "../AI1_Header/AI1.h"
#include "../AI2_Header/AI2.h"
#include "../AI3_Header/AI3.h"
#include<tchar.h>

class ChessBoard : public WindowSize, public WindowHwnd, public PlayerInfo, public Chess
{
public:
	///在InitData.cpp文件中
	ChessBoard();//构造函数
	void Init_cross();//初始化棋盘状态数组
	void Init_Pace();//初始化下棋记录
					 ///在PaintBoard.cpp文件中
	void PaintBoard();//画棋盘
	void BackPace();//回退一步功能函数
					///在PaintTimer.cpp文件中
	void PaintTimer(HWND All, HWND Round, int Player, bool FirstPrint = false);//输出时间信息
																			   ///在PaintChess.cpp文件中
	void PaintChess();//画棋子函数
	void PaintAChess(int type);//实行画棋子
	bool GetPointPosition(DWORD lParam);//获取下棋位置
	///在AddList.cpp文件中
	void AddList(int type);
	void AddSituationList();
	///在WinOrLose.cpp文件中
	bool WinOrLose();//判断胜负
	bool Check(int OriLine, int OriColumn, int line, int column, int player, int i);//
	bool Besieged(int RivalLine, int RivalColumn, int player, int rival);//判断是否被围死
	bool CheBesie(int RivalLine, int RivalColumn, int player, int rival, bool &tie);
	///在ReStart.cpp文件中
	void ReStart();
	///在RePaint.cpp文件中
	void RePaint();//响应窗口重绘消息
	void SetHandle();//设置缓冲区
	void DelHandle();//释放缓冲区

	POINT point;//用于保存鼠标位置
	int onTurn;//轮到谁

	AI *Player1AI;
	AI *Player2AI;
	AI1 ai1;
	AI2 ai2;
	AI3 ai3;

	int line;//下棋位置的行数
	int column;//下棋位置的列数
private:
	int CrossCross[10];//画棋盘用的坐标值

	bool Position[4];

	///类型转换 在int2String.cpp文件中
	std::string String;//用于类型转换
	std::string Int2String(int num);//整型换换为字符型
	void addChar(int mid);//用于辅助转换
	void showChessBroadInfoOnDOS();

};

#endif // CHESSBOARD_H_INCLUDED
