#include "stdafx.h"
#include "buweiqi.h"
#include "ChessBoard_Header/ChessBoard.h"
#include "AI1_Header/AI1.h"
#include "AI2_Header/AI2.h"
#include "AI3_Header/AI3.h"
#include <psapi.h>

#define MID_ONE 1
#define MID_TWO 2
#define MID_START 3
#define AI_1 4
#define AI_2 5
#define AI_3 6
#define TURNBACK 7
#define CHECK_RECORD 8

ChessBoard CB;
HCURSOR UPARROWcursor;//竖直向上箭头的光标图像
HCURSOR ARROWcursor;//一般的光标图像
MSG msg;
int Type = 0;//玩家
int Num = 0;//第几个AI
bool ShowSelect1 = false;//是否显示AI
bool ShowSelect2 = false;//是否显示AI
bool ShowSelect3 = false;//是否显示AI
LPARAM Param;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);//窗口过程函数
LRESULT CALLBACK WndProcB(HWND, UINT, WPARAM, LPARAM);//窗口过程函数
LRESULT CALLBACK WndCombatRecord(HWND, UINT, WPARAM, LPARAM);//窗口过程函数
DWORD WINAPI TimerProc(PVOID pParam);//计时器处理函数
DWORD WINAPI PlayProc(PVOID pParam);//游戏处理函数
void SelectFun();//选择AI功能
void Select();//选择之后的处理
void InitWndClass(TCHAR szAppName[], WNDCLASS &wndclass, HINSTANCE hInstance, int callback);
void SeeCombatRecord();

//监控进程占用内存
void ListenProceMemory()
{
	HANDLE handle = GetCurrentProcess();//获取当前进程句柄
	PROCESS_MEMORY_COUNTERS pmc;
	while (true)
	{
		Sleep(10000);//每10秒检测一次
		GetProcessMemoryInfo(handle, &pmc, sizeof(pmc));//获取进程信息
		//当进程占用内存达到100M时，退出程序
		if (pmc.PeakPagefileUsage / 1024 > 102400)
			SendMessage(CB.RootHwnd, WM_DESTROY, (WPARAM)NULL, (LPARAM)NULL);
//		EmptyWorkingSet(GetCurrentProcess());//回收所有可回收的内存
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{
	CB.ai2.NowLine = 1;
	CB.ai22.NowLine = 2;
	CB.ai2.setPatternScore(CB.ai2.NowLine);
	CB.ai22.setPatternScore(CB.ai22.NowLine);

	std::thread t(ListenProceMemory);
	t.detach();

	CB.hInst = hInstance;
	UPARROWcursor = LoadCursor(NULL, IDC_UPARROW);
	ARROWcursor = LoadCursor(NULL, IDC_ARROW);
	///注册主程序窗口
	WNDCLASS wndclass;
	TCHAR szAppName[] = TEXT("NoGo");
	InitWndClass(szAppName, wndclass, hInstance, 1);
	///注册组件窗口（选择AI的窗口）
	WNDCLASS wndclassB;
	TCHAR szAppNameB[] = _T("Select");
	InitWndClass(szAppNameB, wndclassB, hInstance, 2);
	///注册组件窗口（对弈战绩的窗口）
	WNDCLASS wndclassC;
	TCHAR szAppNameC[] = _T("CombatRecord");
	InitWndClass(szAppNameC, wndclassC, hInstance, 3);

	if (!RegisterClass(&wndclass) || !RegisterClass(&wndclassB) || !RegisterClass(&wndclassC))
	{
		MessageBox(NULL, _T("运行程序失败"),
			szAppName, MB_ICONERROR);
		return 0;
	}

	CB.RootHwnd = CreateWindow(szAppName, _T("不围棋"),
		WS_MINIMIZEBOX | WS_SYSMENU,
		100, 100,
		CB.RootWidth, CB.RootHeight + 15,
		NULL, NULL, hInstance, NULL);
	ShowWindow(CB.RootHwnd, iCmdShow);
	UpdateWindow(CB.RootHwnd);
	
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	FreeConsole();
	return static_cast<int>(msg.wParam);
}
void InitWndClass(TCHAR szAppName[], WNDCLASS &wndclass, HINSTANCE hInstance, int callback)
{
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	if(callback == 1) wndclass.lpfnWndProc = WndProc;
	else if(callback == 2) wndclass.lpfnWndProc = WndProcB;
	else wndclass.lpfnWndProc = WndCombatRecord;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;
}
///主窗口过程函数
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
		//按下ESC键时，使当前对局成为最后一局
		if(static_cast<int>(wParam) == 27) CB.CombatNumber = MAX_COMBAT;break;
	case WM_PAINT://重绘消息
		if (CB.Repaint)
		{
			BeginPaint(hwnd, &CB.ps);
			CB.RePaint();
			EndPaint(hwnd, &CB.ps);
		}
		return 0;
	case WM_COMMAND://窗口下组件的事件
		switch (LOWORD(wParam))
		{
			//玩家1使用了AI
		case MID_ONE:
			if (CB.Start == true) break;
			if (!CB.Player1isAI)
			{
				Type = 1;
				SelectFun();
				Param = lParam;
			}
			else
			{
				SendMessageA((HWND)lParam, WM_SETTEXT, (WPARAM)NULL, LPARAM("使用AI"));
				CB.Player1AI = NULL;
				CB.Player1isAI = false;
				if (CB.onTurn == -1) CB.onTurn = 1;
				else if (CB.onTurn == -2) CB.onTurn = 2;
			}
			break;
			//玩家2使用了AI
		case MID_TWO:
			if (CB.Start == true) break;
			if (!CB.Player2isAI)
			{
				Type = 2;
				SelectFun();
				Param = lParam;
			}
			else
			{
				SendMessageA((HWND)lParam, WM_SETTEXT, (WPARAM)NULL, LPARAM("使用AI"));
				CB.Player2AI = NULL;
				CB.Player2isAI = false;
				if (CB.onTurn == -1) CB.onTurn = 1;
				else if (CB.onTurn == -2) CB.onTurn = 2;
			}
			break;
			//开始游戏按钮按下
		case MID_START:
			if (!CB.Start)
			{
				CB.CombatNumber = 1;
				ShowWindow(CB.TurnToBlack, SW_SHOW);
				UpdateWindow(CB.TurnToBlack);
				CB.PrintTime = true;
				SendMessageA((HWND)lParam, WM_SETTEXT, (WPARAM)NULL, LPARAM("正在游戏中"));
				CB.Start = true;
				if (CB.Player1isAI) CB.onTurn = isAI1onTurn;
				CB.AnoHandle = CreateThread(NULL, 0, TimerProc, NULL, 0, NULL);
				CB.MainProcedureThead = CreateThread(NULL, 0, PlayProc, NULL, 0, NULL);
				SetFocus(hwnd);
			}
			break;
		case TURNBACK:
			//两个都是AI则不能悔棋
			if ((!CB.Player1isAI || !CB.Player2isAI) && CB.ControlLoop && CB.Start)
			{
				CB.ControlLoop = false; 
				CB.BackPace();
				CB.ControlLoop = true;
			} break;
		case CHECK_RECORD:SeeCombatRecord(); break;
		}
		break;
	case WM_CREATE://创建窗口组件
		AllocConsole();
		GetStdHandle(STD_OUTPUT_HANDLE);
		CB.TimerAll1Hwnd = CreateWindow(_T("Static"), _T("游戏总剩余时间"), WS_CHILD | WS_VISIBLE | ES_CENTER, CB.Base + CB.ChessDiameter * 50 / 130, CB.ChessDiameter * 520 / 200, 120, 17, hwnd, NULL, CB.hInst, NULL);
		CB.TimerRound1Hwnd = CreateWindow(_T("Static"), _T("回合剩余时间"), WS_CHILD | WS_VISIBLE | ES_CENTER, CB.Base + CB.ChessDiameter * 50 / 130, CB.ChessDiameter * 650 / 200, 120, 17, hwnd, NULL, CB.hInst, NULL);
		CB.TimerAll2Hwnd = CreateWindow(_T("Static"), _T("游戏总剩余时间"), WS_CHILD | WS_VISIBLE | ES_CENTER, CB.Base + CB.ChessDiameter * 50 / 130, CB.ChessDiameter * 1490 / 200, 120, 17, hwnd, NULL, CB.hInst, NULL);
		CB.TimerRound2Hwnd = CreateWindow(_T("Static"), _T("回合剩余时间"), WS_CHILD | WS_VISIBLE | ES_CENTER, CB.Base + CB.ChessDiameter * 50 / 130, CB.ChessDiameter * 1610 / 200, 120, 17, hwnd, NULL, CB.hInst, NULL);
		CB.Player1Hwnd = CreateWindow(_T("Static"), _T("Player1"), WS_CHILD | WS_VISIBLE | ES_CENTER, CB.Base + CB.ChessDiameter * 50 / 130, CB.ChessDiameter * 250 / 200, 200, 20, hwnd, NULL, CB.hInst, NULL);
		CB.Player2Hwnd = CreateWindow(_T("Static"), _T("Player2"), WS_CHILD | WS_VISIBLE | ES_CENTER, CB.Base + CB.ChessDiameter * 50 / 130, CB.ChessDiameter * 1195 / 200, 200, 20, hwnd, NULL, CB.hInst, NULL);
		CB.Player1UseAI = CreateWindow(_T("Button"), _T("使用AI"), WS_CHILD | WS_VISIBLE | ES_CENTER, CB.Base + CB.ChessDiameter * 130 / 120, CB.ChessDiameter * 365 / 200, 130, 30, hwnd, (HMENU)MID_ONE, CB.hInst, NULL);
		CB.Player2UseAI = CreateWindow(_T("Button"), _T("使用AI"), WS_CHILD | WS_VISIBLE | ES_CENTER, CB.Base + CB.ChessDiameter * 130 / 120, CB.ChessDiameter * 1335 / 200, 130, 30, hwnd, (HMENU)MID_TWO, CB.hInst, NULL);
		CB.ChessBoardHwnd = CreateWindow(_T("Static"), NULL, WS_CHILD | WS_VISIBLE | ES_CENTER, 0, 0, CB.Base, CB.Base, hwnd, NULL, CB.hInst, NULL);
		CB.StartGame = CreateWindow(_T("Button"), _T("开始游戏"), WS_CHILD | WS_VISIBLE | ES_CENTER, CB.Base + CB.ChessDiameter * 125 / 120, CB.ChessDiameter * 895 / 200, 130, 30, hwnd, (HMENU)MID_START, CB.hInst, NULL);
		CB.Timer1A = CreateWindow(_T("Button"), NULL, WS_CHILD | WS_VISIBLE | ES_CENTER, CB.Base + CB.ChessDiameter * 50 / 125 + 130, CB.ChessDiameter * 520 / 200, 60, 20, hwnd, NULL, CB.hInst, NULL);
		CB.Timer2A = CreateWindow(_T("Button"), NULL, WS_CHILD | WS_VISIBLE | ES_CENTER, CB.Base + CB.ChessDiameter * 50 / 125 + 130, CB.ChessDiameter * 1490 / 200, 60, 20, hwnd, NULL, CB.hInst, NULL);
		CB.Timer1R = CreateWindow(_T("Button"), NULL, WS_CHILD | WS_VISIBLE | ES_CENTER, CB.Base + CB.ChessDiameter * 50 / 125 + 130, CB.ChessDiameter * 650 / 200, 60, 20, hwnd, NULL, CB.hInst, NULL);
		CB.Timer2R = CreateWindow(_T("Button"), NULL, WS_CHILD | WS_VISIBLE | ES_CENTER, CB.Base + CB.ChessDiameter * 50 / 125 + 130, CB.ChessDiameter * 1610 / 200, 60, 20, hwnd, NULL, CB.hInst, NULL);
		CB.TurnToBlack = CreateWindow(_T("Static"), _T("轮到玩家1下黑子"), WS_CHILD | WS_VISIBLE | ES_CENTER, CB.Base + CB.ChessDiameter * 150 / 130, CB.ChessDiameter * 780 / 200, 120, 17, hwnd, NULL, CB.hInst, NULL);
		CB.TurnToWhite = CreateWindow(_T("Static"), _T("轮到玩家2下白子"), WS_CHILD | ES_CENTER, CB.Base + CB.ChessDiameter * 150 / 130, CB.ChessDiameter * 1060 / 200, 120, 17, hwnd, NULL, CB.hInst, NULL);
		CB.CombatRecordButton = CreateWindow(_T("Button"), _T("战绩查询"), WS_CHILD | ES_CENTER | WS_VISIBLE, CB.Base + CB.ChessDiameter * 150 / 130, CB.ChessDiameter * 1750 / 200, 120, 30, hwnd, (HMENU)CHECK_RECORD, CB.hInst, NULL);
		CB.PaintTimer(CB.Timer1A, CB.Timer1R, 1, true);
		CB.PaintTimer(CB.Timer2A, CB.Timer2R, 2, true);
		CB.Board = (HBITMAP)LoadImage(NULL, _T("Image/Board.bmp"), IMAGE_BITMAP, 500, 500, LR_LOADFROMFILE);
		CB.BlackChess = (HBITMAP)LoadImage(NULL, _T("Image/BlackChess.bmp"), IMAGE_BITMAP, 200, 200, LR_LOADFROMFILE);
		CB.WhiteChess = (HBITMAP)LoadImage(NULL, _T("Image/WhiteChess.bmp"), IMAGE_BITMAP, 200, 200, LR_LOADFROMFILE);//LR_LOADMAP3DCOLORS
		CB.Tips = (HBITMAP)LoadImage(NULL, _T("Image/Tips.bmp"), IMAGE_BITMAP, 126, 126, LR_LOADFROMFILE);
		CB.BackHwnd = CreateWindow(_T("Button"), _T("回退一步"), WS_CHILD | WS_VISIBLE | ES_CENTER, CB.Base + CB.ChessDiameter * 50 / 125 + 50, CB.ChessDiameter * 60 / 200, 100, 40, hwnd, (HMENU)TURNBACK, CB.hInst, NULL);
		PostMessage(hwnd, WM_PAINT, (WPARAM)NULL, (LPARAM)NULL);
		break;
	case WM_MOUSEMOVE://鼠标移动消息
		CB.point.x = LOWORD(lParam);//获取鼠标位置
		CB.point.y = HIWORD(lParam);
		if (CB.point.x < CB.Base && CB.point.y < CB.Base)
			SetCursor(UPARROWcursor);
		else
			SetCursor(ARROWcursor);
		break;
	case WM_LBUTTONDOWN://鼠标左键事件
		if (CB.Start == true || CB.Player1isAI == false || CB.Player2isAI == false)
		{
			if (CB.onTurn == isPlay1onTurn || CB.onTurn == isPlay2onTurn) ResumeThread(CB.MainProcedureThead);
			CB.GetPointPosition(lParam);
		}
		break;
	case WM_DESTROY:
		DeleteObject(CB.Board);
		DeleteObject(CB.BlackChess);
		DeleteObject(CB.WhiteChess);
		DeleteObject(CB.Tips);
		DeleteObject(CB.CombatRecord);
		PostQuitMessage(0);
		int i, data[36];
		std::fstream tempfile("CombatRecord.txt", std::ios::out | std::ios::in);
		if (!tempfile) exit(0);
		tempfile.seekg(0);
		for (i = 0; i < 36; i++)
		{
			tempfile >> data[i];
			if (i < 18) data[i] = 0;
		}
		tempfile.close();
		tempfile.open("CombatRecord.txt", std::ios::out);
		if (!tempfile) exit(0);
		tempfile.seekg(0);
		for (i = 0; i < 36; i++)
		{
			if (i % 6 == 0) tempfile << std::endl;
			tempfile << data[i] << " ";
		}
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}
///计时器函数
DWORD WINAPI TimerProc(PVOID pParam)
{
	ULONGLONG Count = GetTickCount64();//现在的计数
	ULONGLONG Interval = 1000;//时间间隔（单位：ms）
	ULONGLONG Now = Count;//现在已记录的时间
	while (CB.Start)
	{
		Now = GetTickCount64();//获取当前计时
		if (Now - Count > Interval)
		{
			Count = Now;
			if (CB.onTurn == isPlay1onTurn || CB.onTurn == isAI1onTurn)
			{
				CB.AllTime1--;
				CB.Round1--;
				if (CB.AllTime1 == 0)
				{
					CB.Winner = 2;
					CB.ReStart();
					break;
				}
				CB.PaintTimer(CB.Timer1A, CB.Timer1R, 1);
			}
			else if (CB.onTurn == isPlay2onTurn || CB.onTurn == isAI2onTurn)
			{
				CB.AllTime2--;
				CB.Round2--;
				if (CB.AllTime2 == 0)
				{
					CB.Winner = 1;
					CB.ReStart();
					break;
				}
				CB.PaintTimer(CB.Timer2A, CB.Timer2R, 2);
			}
		}
	}
	CloseHandle(CB.AnoHandle);
	return 0;
}
DWORD WINAPI PlayProc(PVOID pParam)
{
	while (CB.Start)
	{
		if(CB.ControlLoop) CB.PaintChess();
		if (!CB.Start && CB.CombatNumber < MAX_COMBAT && CB.Player1isAI && CB.Player2isAI)
		{
			CB.CombatNumber++;
			CB.Start = true;
			CB.RePaint();
			Sleep(200);
		}
	}
	CloseHandle(CB.MainProcedureThead);
	return 0;
}

///响应选择AI
void SelectFun()
{
	CB.SelectAI = CreateWindow(_T("Select"), _T("请选择一个AI"), WS_MINIMIZEBOX | WS_SYSMENU, CB.RootWidth / 2 - 125, CB.RootHeight / 2 - 150, 250, 300, CB.RootHwnd, NULL, CB.hInst, NULL);
	GetWindowRect(CB.RootHwnd, &CB.MainWinRect);//获取主窗口在屏幕中的位置
	MoveWindow(CB.SelectAI, CB.MainWinRect.left + (CB.RootWidth - 250) / 2, CB.MainWinRect.top + (CB.RootHeight - 300) / 2, 250, 300, false);//改变窗口位置和大小
	ShowWindow(CB.SelectAI, SW_SHOW);//设置窗口可视
	if (CB.CanSelectAI1)
	{
		ShowWindow(CB.SelectAI1, SW_SHOW);
		ShowSelect1 = true;
	}
	if (CB.CanSelectAI2)
	{
		ShowWindow(CB.SelectAI2, SW_SHOW);
		ShowSelect2 = true;
	}
	if (CB.CanSelectAI3)
	{
		ShowWindow(CB.SelectAI3, SW_SHOW);
		ShowSelect3 = true;
	}
	UpdateWindow(CB.SelectAI);
}
///选择AI之后的处理
void Select()
{
	ShowWindow(CB.SelectAI1, SW_HIDE);
	ShowWindow(CB.SelectAI2, SW_HIDE);
	ShowWindow(CB.SelectAI3, SW_HIDE);
	ShowWindow(CB.SelectAI, SW_HIDE);
	DestroyWindow(CB.SelectAI);//删除窗口
	switch (Type)
	{
	case 1:
		switch (Num)
		{
			case 1:CB.Player1AI = &CB.ai4; SendMessageA((HWND)Param, WM_SETTEXT, (WPARAM)NULL, LPARAM("已使用了AI4"));break;
			case 2:CB.Player1AI = &CB.ai5; SendMessageA((HWND)Param, WM_SETTEXT, (WPARAM)NULL, LPARAM("已使用了AI5"));break;
			case 3:CB.Player1AI = &CB.ai3; SendMessageA((HWND)Param, WM_SETTEXT, (WPARAM)NULL, LPARAM("已使用了AI3"));break;
		}
		CB.Player1isAI = true;
		
		break;
	case 2:
		switch (Num)
		{
			case 1:CB.Player2AI = &CB.ai44; SendMessageA((HWND)Param, WM_SETTEXT, (WPARAM)NULL, LPARAM("已使用了AI4"));break;
			case 2:CB.Player2AI = &CB.ai55; SendMessageA((HWND)Param, WM_SETTEXT, (WPARAM)NULL, LPARAM("已使用了AI5"));break;
			case 3:CB.Player2AI = &CB.ai33; SendMessageA((HWND)Param, WM_SETTEXT, (WPARAM)NULL, LPARAM("已使用了AI3"));break;
		}
		CB.Player2isAI = true;
		break;
	}
	if (Type == CB.onTurn) CB.onTurn = isAI1onTurn;
	///初始化AI数据
	if (CB.Player1AI != NULL) CB.Player1AI->GetPosition(Num, Type, 0);
	if (CB.Player2AI != NULL) CB.Player2AI->GetPosition(Num, Type, 0);
	ShowSelect1 = false;//是否显示AI
	ShowSelect2 = false;//是否显示AI
	ShowSelect3 = false;//是否显示AI
}
LRESULT CALLBACK WndProcB(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
		if (ShowSelect1) ShowWindow(CB.SelectAI1, SW_SHOW);
		if (ShowSelect2) ShowWindow(CB.SelectAI2, SW_SHOW);
		if (ShowSelect3) ShowWindow(CB.SelectAI3, SW_SHOW);
		break;
	case WM_CREATE:
		CB.SelectAI1 = CreateWindow(_T("Button"), _T("AI4"), WS_CHILD | ES_CENTER, 45, 20, 160, 50, hwnd, (HMENU)AI_1, CB.hInst, NULL);
		CB.SelectAI2 = CreateWindow(_T("Button"), _T("AI5"), WS_CHILD | ES_CENTER, 45, 100, 160, 50, hwnd, (HMENU)AI_2, CB.hInst, NULL);
		CB.SelectAI3 = CreateWindow(_T("Button"), _T("AI3"), WS_CHILD | ES_CENTER, 45, 180, 160, 50, hwnd, (HMENU)AI_3, CB.hInst, NULL);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
			//当选择了AI1
		case AI_1:Num = 1;Select();break;
			//当选择了AI2
		case AI_2:Num = 2;Select();break;
			//当选择了AI3
		case AI_3:Num = 3;Select();break;
		}
		break;
	case WM_DESTROY:
		DestroyWindow(CB.SelectAI);//删除窗口
		break;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}
LRESULT CALLBACK WndCombatRecord(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:; break;
	case WM_CREATE:
		CB.CurrentCR = CreateWindow(_T("Static"), _T("当前战绩"), WS_CHILD | ES_CENTER | WS_VISIBLE, 110, 10, 200, 20, hwnd, NULL, CB.hInst, NULL);
		CB.AllCR = CreateWindow(_T("Static"), _T("历史战绩"), WS_CHILD | ES_CENTER | WS_VISIBLE, 320, 10, 200, 20, hwnd, NULL, CB.hInst, NULL);
		CB.CurrentWin = CreateWindow(_T("Static"), _T("胜"), WS_CHILD | ES_CENTER | WS_VISIBLE, 155, 40, 20, 20, hwnd, NULL, CB.hInst, NULL);
		CB.CurrentLose = CreateWindow(_T("Static"), _T("负"), WS_CHILD | ES_CENTER | WS_VISIBLE, 233, 40, 20, 20, hwnd, NULL, CB.hInst, NULL);
		CB.AllWin = CreateWindow(_T("Static"), _T("胜"), WS_CHILD | ES_CENTER | WS_VISIBLE, 365, 40, 20, 20, hwnd, NULL, CB.hInst, NULL);
		CB.AllLose = CreateWindow(_T("Static"), _T("负"), WS_CHILD | ES_CENTER | WS_VISIBLE, 443, 40, 20, 20, hwnd, NULL, CB.hInst, NULL);
		CB.AI1_VS_AI1 = CreateWindow(_T("Static"), _T("AI4 VS AI4"), WS_CHILD | ES_CENTER | WS_VISIBLE, 0, 70, 100, 20, hwnd, NULL, CB.hInst, NULL);
		CB.AI1_VS_AI2 = CreateWindow(_T("Static"), _T("AI4 VS AI5"), WS_CHILD | ES_CENTER | WS_VISIBLE, 0, 100, 100, 20, hwnd, NULL, CB.hInst, NULL);
		CB.AI1_VS_AI3 = CreateWindow(_T("Static"), _T("AI4 VS AI3"), WS_CHILD | ES_CENTER | WS_VISIBLE, 0, 130, 100, 20, hwnd, NULL, CB.hInst, NULL);
		CB.AI2_VS_AI1 = CreateWindow(_T("Static"), _T("AI5 VS AI4"), WS_CHILD | ES_CENTER | WS_VISIBLE, 0, 160, 100, 20, hwnd, NULL, CB.hInst, NULL);
		CB.AI2_VS_AI2 = CreateWindow(_T("Static"), _T("AI5 VS AI5"), WS_CHILD | ES_CENTER | WS_VISIBLE, 0, 190, 100, 20, hwnd, NULL, CB.hInst, NULL);
		CB.AI2_VS_AI3 = CreateWindow(_T("Static"), _T("AI5 VS AI3"), WS_CHILD | ES_CENTER | WS_VISIBLE, 0, 220, 100, 20, hwnd, NULL, CB.hInst, NULL);
		CB.AI3_VS_AI1 = CreateWindow(_T("Static"), _T("AI3 VS AI4"), WS_CHILD | ES_CENTER | WS_VISIBLE, 0, 250, 100, 20, hwnd, NULL, CB.hInst, NULL);
		CB.AI3_VS_AI2 = CreateWindow(_T("Static"), _T("AI3 VS AI5"), WS_CHILD | ES_CENTER | WS_VISIBLE, 0, 280, 100, 20, hwnd, NULL, CB.hInst, NULL);
		CB.AI3_VS_AI3 = CreateWindow(_T("Static"), _T("AI3 VS AI3"), WS_CHILD | ES_CENTER | WS_VISIBLE, 0, 310, 100, 20, hwnd, NULL, CB.hInst, NULL);
		int i;
		for(i = 0; i < 9; i++) CB.DataHwnd[i] = CreateWindow(_T("Static"), _T("0"), WS_CHILD | ES_CENTER | WS_VISIBLE, 140, 70 + i * 30, 50, 20, hwnd, NULL, CB.hInst, NULL);
		for (i = 0; i < 9; i++) CB.DataHwnd[i + 9] = CreateWindow(_T("Static"), _T("0"), WS_CHILD | ES_CENTER | WS_VISIBLE, 218, 70 + i * 30, 50, 20, hwnd, NULL, CB.hInst, NULL);
		for (i = 0; i < 9; i++) CB.DataHwnd[i + 18] = CreateWindow(_T("Static"), _T("0"), WS_CHILD | ES_CENTER | WS_VISIBLE, 350, 70 + i * 30, 50, 20, hwnd, NULL, CB.hInst, NULL);
		for (i = 0; i < 9; i++) CB.DataHwnd[i + 27] = CreateWindow(_T("Static"), _T("0"), WS_CHILD | ES_CENTER | WS_VISIBLE, 428, 70 + i * 30, 50, 20, hwnd, NULL, CB.hInst, NULL);
		break;
	case WM_DESTROY:
		DestroyWindow(CB.CombatRecord);//删除窗口
		break;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}
void SeeCombatRecord()
{
	CB.CombatRecord = CreateWindow(_T("CombatRecord"), _T("战绩记录"), WS_MINIMIZEBOX | WS_SYSMENU, CB.RootWidth / 2 - 125, CB.RootHeight / 2 - 150, 250, 300, CB.RootHwnd, NULL, CB.hInst, NULL);
	GetWindowRect(CB.RootHwnd, &CB.MainWinRect);//获取主窗口在屏幕中的位置
	MoveWindow(CB.CombatRecord, CB.MainWinRect.left + (CB.RootWidth - 550) / 2, CB.MainWinRect.top + (CB.RootHeight - 370) / 2, 550, 370, false);//改变窗口位置和大小
	ShowWindow(CB.CombatRecord, SW_SHOW);//设置窗口可视
	CB.UpdateCRdata();
}