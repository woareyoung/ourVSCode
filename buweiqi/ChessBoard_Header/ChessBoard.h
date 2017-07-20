#pragma once

#ifdef __ChessBoard

#endif // __ChessBoard

#include "../stdafx.h"
#include "WindowSize.h"
#include "WindowHwnd.h"
#include "PlayerInfo.h"
#include "Chess.h"
#include "../AI1_Header/AI1.h"
#include "../AI2_Header/AI2.h"
#include "../AI3_Header/AI3.h"
#include "../AI4_Header/AI4.h"

#ifdef __cplusplus
extern "C" {
#endif
	class ChessBoard;
#ifdef __cplusplus
}
#endif
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
	void GetPointPosition(DWORD lParam);//获取下棋位置
	void DisplayOnTurn(int type);
	///在AddList.cpp文件中
	void AddList(int type);
	void AddSituationList();
	///在ReStart.cpp文件中
	void ReStart();
	///在RePaint.cpp文件中
	void RePaint();//响应窗口重绘消息
	void SetHandle();//设置缓冲区
	void DelHandle();//释放缓冲区

	POINT point;//用于保存鼠标位置
	int onTurn;//轮到谁
	int CombatNumber;
	int CurrentRoundNumber;

	AI *Player1AI;
	AI *Player2AI;
	
	AI1 ai11;
	AI2 ai22;
	AI3 ai33;
	AI4 ai44;
	AI1 ai1;
	AI2 ai2;
	AI3 ai3;
	AI4 ai4;

	int line;//下棋位置的行数
	int column;//下棋位置的列数
private:
	int CrossCross[10];//画棋盘用的坐标值

	///类型转换 在int2String.cpp文件中
	std::string String;//用于类型转换
	std::string Int2String(int num);//整型换换为字符型
	void addChar(int mid);//用于辅助转换
};
