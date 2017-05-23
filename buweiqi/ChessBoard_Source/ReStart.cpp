#include "../stdafx.h"
#include "../ChessBoard_Header/ChessBoard.h"
///游戏结束
void ChessBoard::ReStart()
{
	CloseHandle(TimeHandle);
	if (Winner == isBlack)
		MessageBox(RootHwnd, _T("恭喜玩家1（黑子）胜出"), _T("凯旋"), MB_OK);
	else MessageBox(RootHwnd, _T("恭喜玩家2（白子）胜出"), _T("凯旋"), MB_OK);
	ai1.FileIO.CloseFile();
	FileS.AddMemory(SituaHead, Winner);
	Init_cross();
	Init_Pace();
	onTurn = 1;
	Winner = 0;
	AllTime1 = 900;//总时长15分钟
	AllTime2 = 900;
	Round1 = 60;//步时60秒
	Round2 = 60;
	Start = false;
	Player1isAI = false;
	Player2isAI = false;
	ai1.GetPosition(line, column, 0);
	ai2.GetPosition(line, column, 0);
	ai3.GetPosition(line, column, 0);
	line = 0;
	column = 0;
	Player1AI = NULL;
	Player2AI = NULL;
	Qua.FirstQuadrant = 0;
	Qua.SecondQuadrant = 0;
	Qua.ThirdQuadrant = 0;
	Qua.ForthQuadrant = 0;
	ai1.Qua.FirstQuadrant = 0;
	ai1.Qua.SecondQuadrant = 0;
	ai1.Qua.ThirdQuadrant = 0;
	ai1.Qua.ForthQuadrant = 0;
	PostMessage(RootHwnd, WM_PAINT, (WPARAM)NULL, (LPARAM)NULL);

	SendMessageA(Player1UseAI, WM_SETTEXT, (WPARAM)NULL, LPARAM("使用AI"));
	SendMessageA(Player2UseAI, WM_SETTEXT, (WPARAM)NULL, LPARAM("使用AI"));
	SendMessageA(StartGame, WM_SETTEXT, (WPARAM)NULL, LPARAM("开始游戏"));
	ShowWindow(TurnToWhite, SW_HIDE);
	ShowWindow(TurnToBlack, SW_SHOW);

}
