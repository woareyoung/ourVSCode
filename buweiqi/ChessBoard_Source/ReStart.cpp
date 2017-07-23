#include "../stdafx.h"
#include <cstring>
#include <sstream>  
#include <iomanip>
#include <list>
#include "../ChessBoard_Header/ChessBoard.h"
#include "../ChessBoard_Header/showUnicodeInfo.h"
#define COMBATNUMBER 250 //AI2ģ��Ծִ���
///��Ϸ����
void ChessBoard::ReStart()
{
	//��ʾ��DOS����
	if (DISPLAY == 2 || DISPLAY == 0)
	{
		showChessBoard(cross);
		if (Winner == 1)
		{
			if (CombatNumber == MAX_COMBAT) _cprintf("the Winner of the final(%d) is : Player1 ( Black )\n\n", CombatNumber);
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
			if (CombatNumber == MAX_COMBAT) _cprintf("the Winner of the final(%d) is : Player2 ( White )\n\n", CombatNumber);
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
//	FileS.AddMemory(MemBattle, Winner);
	if (Player1AI == &ai4)
	{
		if (Player2AI == &ai44) UpdateCRdata(4, 4, Winner);
		else if (Player2AI == &ai55) UpdateCRdata(4, 5, Winner);
		else if (Player2AI == &ai33) UpdateCRdata(4, 3, Winner);
	}
	else if (Player1AI == &ai5)
	{
		if (Player2AI == &ai44) UpdateCRdata(5, 4, Winner);
		else if (Player2AI == &ai55) UpdateCRdata(5, 5, Winner);
		else if (Player2AI == &ai33) UpdateCRdata(5, 3, Winner);
	}
	else if(Player1AI == &ai3)
	{
		if (Player2AI == &ai44) UpdateCRdata(3, 4, Winner);
		else if (Player2AI == &ai55) UpdateCRdata(3, 5, Winner);
		else if(Player2AI == &ai33) UpdateCRdata(3, 3, Winner);
	}
	//	_cprintf("Black Chess number : %d  White Chess number : %d\n", ChessAmount[1], ChessAmount[2]);
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
	//--------------------------------------�����ݶ�ר��--------------------------------------//
	if (Player1AI == &ai2 && Player2AI == &ai22)//���ж��ǲ�������AI2
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
		lastline1 = Player1AI->NowLine;
		lastline2 = Player2AI->NowLine;
		while (!FFF.eof())
		{
			FFF >> ScoreLine >> temp1 >> temp2;
			if (ScoreLine == "end") break;
			if (Player1AI->NowLine == stoi(ScoreLine.substr(10, 2)) && Player2AI->NowLine == stoi(ScoreLine.substr(13, 2)))
			{
				if (Winner == 1) ++temp1;
				else ++temp2;
				//���ﵽ�Ծִ���ʱ
				if (temp1 + temp2 > COMBATNUMBER - 1)
				{
					int lllll = Player1AI->NowLine > Player2AI->NowLine ? Player1AI->NowLine + 1 : Player2AI->NowLine + 1;
					if (lllll == sizeof(ai2.patternAllScore) / sizeof(ai2.patternAllScore[0]) / sizeof(ai2.patternAllScore[0][0])) lllll = 0;
					if (temp1 > temp2)
					{
						ai22.setPatternScore(lllll);
						ai22.NowLine = lllll;
					}
					else
					{
						ai2.setPatternScore(lllll);
						ai2.NowLine = lllll;
					}
				}
				Have = true;
			}
			OneWin.push_back(temp1);
			TwoWin.push_back(temp2);
			SC.push_back(ScoreLine);
		}
		if (!Have)
		{
			ScoreLine = "ScoreLine(";
			if (Winner == 1) OneWin.push_back(1), TwoWin.push_back(0);
			else TwoWin.push_back(1), OneWin.push_back(0);
			if (Player1AI->NowLine > 9) ScoreLine += std::to_string(Player1AI->NowLine);
			else ScoreLine += "0" + std::to_string(Player1AI->NowLine);
			if(Player2AI->NowLine > 9) ScoreLine += "," + std::to_string(Player2AI->NowLine) + ")";
			else ScoreLine += ",0" + std::to_string(Player2AI->NowLine) + "):";
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
		FFF << "end" << std::endl;
		FFF.close();
	}
	//--------------------------------------�����ݶ�ר��-------------------------------------//
	Start = false;
	Winner = 0;
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
