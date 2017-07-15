#include "../stdafx.h"
#include <cstring>
#include <sstream>  
#include <iomanip>
#include <list>
#include "../ChessBoard_Header/ChessBoard.h"
#include "../ChessBoard_Header/showUnicodeInfo.h"
#define COMBATNUMBER 500 //AI2模拟对局次数
///游戏结束
void ChessBoard::ReStart()
{
	//显示在DOS界面
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
				str = "***The Winner of the" + temp + "is : Player1 ( ● )***\n";
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
				str = "***The Winner of the" + temp + "is : Player2 ( 〇 )***\n";
				showInfo(str.c_str());
			}
		}
	}
	bool re = CombatNumber < MAX_COMBAT && Player1isAI && Player2isAI;
	if (!re)
	{
		if (Winner == isBlack)
			MessageBox(RootHwnd, _T("恭喜玩家1（黑子）胜出"), _T("凯旋"), MB_OK);
		else MessageBox(RootHwnd, _T("恭喜玩家2（白子）胜出"), _T("凯旋"), MB_OK);
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
	AllTime1 = 900;//总时长15分钟
	AllTime2 = 900;
	Round1 = 60;//步时60秒
	Round2 = 60;
	line = 0;
	column = 0;
	///界面的Quadrant对象
	Qua.FirstQuadrant = 0;
	Qua.SecondQuadrant = 0;
	Qua.ThirdQuadrant = 0;
	Qua.ForthQuadrant = 0;
	//--------------------------------------测试梯度专用--------------------------------------//
	if (Player1AI == &ai2 && Player2AI == &ai22)//先判断是不是两个AI2
	{		
		int lastline1, lastline2;
		std::fstream FFF;
		std::string FFFName = "AI2CombatRec.txt";
		FileS.OpenFile(FFFName, FFF);
		std::string PlayerWin1, PlayerWin2, ScoreLine, NW;
		std::list<int> OneWin;
		std::list<int> TwoWin;
		std::list<std::string> SC;
		int temp1, temp2;
		bool Have = false;
		FFF >> ScoreLine >> lastline1 >> lastline2 >> PlayerWin1 >> PlayerWin2;
		while (!FFF.eof())
		{
			FFF >> ScoreLine >> temp1 >> temp2;
			if (ScoreLine == "LastLine:") break;
			if (Player1AI->NowLine == stoi(ScoreLine.substr(10, 2)) && Player2AI->NowLine == stoi(ScoreLine.substr(13, 2)))
			{
				if (Winner == 1) ++temp1;
				else ++temp2;
				//当达到对局次数时
				if (temp1 + temp2 > COMBATNUMBER - 1)
				{
					int lllll = Player1AI->NowLine > Player2AI->NowLine ? Player1AI->NowLine + 1 : Player2AI->NowLine + 1;
					if (temp1 > temp2) ai22.setPatternScore(lllll);
					else ai2.setPatternScore(lllll);
				}
				Have = true;
			}
			OneWin.push_back(temp1);
			TwoWin.push_back(temp2);
			SC.push_back(ScoreLine);
		}
		OneWin.pop_back();
		TwoWin.pop_back();
		SC.pop_back();
		if (!Have)
		{
			if(Winner == 1) OneWin.push_back(1);
			else TwoWin.push_back(1);
			if (Player1AI->NowLine > 9) ScoreLine = "ScoreLine(" + std::to_string(Player1AI->NowLine);
			else ScoreLine += "0" + std::to_string(Player1AI->NowLine);
			if(Player2AI->NowLine > 9) ScoreLine += "," + std::to_string(Player2AI->NowLine) + ")";
			else ScoreLine += "0" + std::to_string(Player2AI->NowLine) + ")";
			SC.push_back(ScoreLine);
		}
		FFF.close();
		FFF.open(FFFName, std::ios::out);
		std::list<int>::iterator it1 = OneWin.begin(), it2 = TwoWin.begin();
		std::list<std::string>::iterator itstr = SC.begin();
		NW = "LastLine: " + std::to_string(lastline1) + " " + std::to_string(lastline2);
		FFF << std::setw(20) << NW  << std::setw(20) << PlayerWin1 << std::setw(20) << PlayerWin2 << std::endl;
		for (; it1 != OneWin.end(); ++it1, ++it2, ++itstr)
		{
			FFF << std::setw(20) << *itstr << std::setw(11) << *it1 << std::setw(23) << *it2 << std::endl;
		}
		FFF.close();
	}
	//--------------------------------------测试梯度专用-------------------------------------//
	Start = false;
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
	UpdateWindow(RootHwnd);
	ShowWindow(TurnToWhite, SW_HIDE);
	ShowWindow(TurnToBlack, SW_SHOW);
}
