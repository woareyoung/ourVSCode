#include "../stdafx.h"
#include "../ChessBoard_Header/ChessBoard.h"
///输出倒计时信息
//参数：All  游戏总剩余时间的窗口句柄
//参数：Round  步时窗口句柄
//参数：Player  玩家编号
//参数：FirstPrint  是否是第一次输出时间（默认为false）
void ChessBoard::PaintTimer(HWND All, HWND Round, int Player, bool FirstPrint)
{
	TheFirstNumber = false;
	char str[20];
	int i;
	if (FirstPrint)
	{
		SendMessageA(All, WM_SETTEXT, (WPARAM)NULL, LPARAM("15:00"));
		SendMessageA(Round, WM_SETTEXT, (WPARAM)NULL, LPARAM("1:00"));
		return;
	}
	if (Start)
	{
		for (i = 0; i < 20; i++) str[i] = '\0';
		if (Player == 1)
		{
			lstrcat(LPWSTR(str), LPWSTR(Int2String(AllTime1 / 60)));
			lstrcat(LPWSTR(str), LPWSTR("："));
			lstrcat(LPWSTR(str), LPWSTR(Int2String(AllTime1 % 60)));
		}
		else
		{
			lstrcat(LPWSTR(str), LPWSTR(Int2String(AllTime2 / 60)));
			lstrcat(LPWSTR(str), LPWSTR("："));
			lstrcat(LPWSTR(str), LPWSTR(Int2String(AllTime2 % 60)));
		}
		SendMessageA(All, WM_SETTEXT, (WPARAM)NULL, LPARAM(str));
		for (i = 0; i < 20; i++) str[i] = '\0';
		if (Player == 1)
		{
			lstrcat(LPWSTR(str), LPWSTR(Int2String(Round1 / 60)));
			lstrcat(LPWSTR(str), LPWSTR("："));
			lstrcat(LPWSTR(str), LPWSTR(Int2String(Round1 % 60)));
		}
		else
		{
			lstrcat(LPWSTR(str), LPWSTR(Int2String(Round2 / 60)));
			lstrcat(LPWSTR(str), LPWSTR("："));
			lstrcat(LPWSTR(str), LPWSTR(Int2String(Round2 % 60)));
		}
		SendMessageA(Round, WM_SETTEXT, (WPARAM)NULL, LPARAM(str));
	}
}
