#include "../stdafx.h"
#include "../ChessBoard_Header/ChessBoard.h"
///游戏结束
void ChessBoard::ReStart()
{
	bool re = CombatNumber < MAX_COMBAT && Player1isAI && Player2isAI;
	if (!re)
	{
		if (Winner == isBlack)
			MessageBox(RootHwnd, _T("恭喜玩家1（黑子）胜出"), _T("凯旋"), MB_OK);
		else MessageBox(RootHwnd, _T("恭喜玩家2（白子）胜出"), _T("凯旋"), MB_OK);
	}
	FileS.AddMemory(SituaHead, Winner);
	if (Player1AI == &ai1)
	{
		if (Player2AI == &ai11) UpdateCRdata(1, 1, Winner);
		else if (Player2AI == &ai22) UpdateCRdata(1, 2, Winner);
		else  UpdateCRdata(1, 3, Winner);
	}
	else if (Player1AI == &ai2)
	{
		if (Player2AI == &ai11) UpdateCRdata(2, 1, Winner);
		else if (Player2AI == &ai22) UpdateCRdata(2, 2, Winner);
		else  UpdateCRdata(2, 3, Winner);
	}
	else
	{
		if (Player2AI == &ai11) UpdateCRdata(3, 1, Winner);
		else if (Player2AI == &ai22) UpdateCRdata(3, 2, Winner);
		else  UpdateCRdata(3, 3, Winner);
	}
	Winner = 0;
	Init_cross();
	Init_Pace();
	onTurn = -1;
	AllTime1 = 900;//总时长15分钟
	AllTime2 = 900;
	Round1 = 60;//步时60秒
	Round2 = 60;
	Start = false;
	line = 0;
	column = 0;
	///界面的Quadrant对象
	Qua.FirstQuadrant = 0;
	Qua.SecondQuadrant = 0;
	Qua.ThirdQuadrant = 0;
	Qua.ForthQuadrant = 0;
	if (!re)
	{
		onTurn = 1;
		Player1isAI = false;
		Player2isAI = false;
		Player1AI = nullptr;
		Player2AI = nullptr;
		SendMessageA(Player1UseAI, WM_SETTEXT, (WPARAM)NULL, LPARAM("使用AI"));
		SendMessageA(Player2UseAI, WM_SETTEXT, (WPARAM)NULL, LPARAM("使用AI"));
		SendMessageA(StartGame, WM_SETTEXT, (WPARAM)NULL, LPARAM("开始游戏"));
	}
	else
	{
		int n, m;
		if (Player1AI != NULL) Player1AI->GetPosition(n, m, 0);
		if (Player2AI != NULL) Player2AI->GetPosition(n, m, 0);
	}
	PostMessage(RootHwnd, WM_PAINT, (WPARAM)NULL, (LPARAM)NULL);
	ShowWindow(TurnToWhite, SW_HIDE);
	ShowWindow(TurnToBlack, SW_SHOW);
}
