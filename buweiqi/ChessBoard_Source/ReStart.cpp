#include "../stdafx.h"
#include <cstring>
#include <sstream>  
#include "../ChessBoard_Header/ChessBoard.h"
#include "../ChessBoard_Header/showUnicodeInfo.h"
///��Ϸ����
void ChessBoard::ReStart()
{
	if (DISPLAY == 2 || DISPLAY == 0)
	{
		showChessBoard(cross);
		if (Winner == 1)
		{
			if (CombatNumber == MAX_COMBAT) _cprintf("the Winner of the final is : Player1 ( Black )\n\n", CombatNumber);
			else {
				std::string str;
				std::string temp;
				std::ostringstream convert;
				convert << CombatNumber;
				temp = convert.str();
				str = "***The Winner of the" + temp + "is : Player1 ( �� )***\n";
				showInfo(str.c_str());
			}

		}
		else
		{
			if (CombatNumber == MAX_COMBAT) _cprintf("the Winner of the final is : Player2 ( White )\n\n", CombatNumber);
			else {
				std::string str;
				std::string temp;
				std::ostringstream convert;
				convert << CombatNumber;
				temp = convert.str();
				str = "***The Winner of the" + temp + "is : Player2 ( �� )***\n";
				showInfo(str.c_str());
			}
		}
	}
	bool re = CombatNumber < MAX_COMBAT && Player1isAI && Player2isAI;
	if (!re)
	{
		if (Winner == isBlack)
			MessageBox(RootHwnd, _T("��ϲ���1�����ӣ�ʤ��"), _T("����"), MB_OK);
		else MessageBox(RootHwnd, _T("��ϲ���2�����ӣ�ʤ��"), _T("����"), MB_OK);
	}
	FileS.AddMemory(MemBattle, Winner);
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
	//	_cprintf("Black Chess number : %d  White Chess number : %d\n", ChessAmount[1], ChessAmount[2]);
	Winner = 0;
	Init_cross();
	Init_Pace();
	onTurn = -1;
	AllTime1 = 900;//��ʱ��15����
	AllTime2 = 900;
	Round1 = 60;//��ʱ60��
	Round2 = 60;
	line = 0;
	column = 0;
	///�����Quadrant����
	Qua.FirstQuadrant = 0;
	Qua.SecondQuadrant = 0;
	Qua.ThirdQuadrant = 0;
	Qua.ForthQuadrant = 0;
	Start = false;
	if (!re)
	{
		onTurn = 1;
		Player1isAI = false;
		Player2isAI = false;
		Player1AI = nullptr;
		Player2AI = nullptr;
		SendMessageA(Player1UseAI, WM_SETTEXT, (WPARAM)NULL, LPARAM("ʹ��AI"));
		SendMessageA(Player2UseAI, WM_SETTEXT, (WPARAM)NULL, LPARAM("ʹ��AI"));
		SendMessageA(StartGame, WM_SETTEXT, (WPARAM)NULL, LPARAM("��ʼ��Ϸ"));
	}
	else
	{
		int n, m;
		if (Player1AI != NULL) Player1AI->GetPosition(n, m, 0);
		if (Player2AI != NULL) Player2AI->GetPosition(n, m, 0);
	}
	PostMessage(RootHwnd, WM_PAINT, (WPARAM)NULL, (LPARAM)NULL);
	UpdateWindow(RootHwnd);
	ShowWindow(TurnToWhite, SW_HIDE);
	ShowWindow(TurnToBlack, SW_SHOW);
}
