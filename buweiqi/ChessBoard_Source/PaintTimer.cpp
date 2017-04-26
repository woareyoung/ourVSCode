#include "../stdafx.h"
#include "../ChessBoard_Header/ChessBoard.h"
///�������ʱ��Ϣ
//������All  ��Ϸ��ʣ��ʱ��Ĵ��ھ��
//������Round  ��ʱ���ھ��
//������Player  ��ұ��
//������FirstPrint  �Ƿ��ǵ�һ�����ʱ�䣨Ĭ��Ϊfalse��
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
			lstrcat(LPWSTR(str), LPWSTR("��"));
			lstrcat(LPWSTR(str), LPWSTR(Int2String(AllTime1 % 60)));
		}
		else
		{
			lstrcat(LPWSTR(str), LPWSTR(Int2String(AllTime2 / 60)));
			lstrcat(LPWSTR(str), LPWSTR("��"));
			lstrcat(LPWSTR(str), LPWSTR(Int2String(AllTime2 % 60)));
		}
		SendMessageA(All, WM_SETTEXT, (WPARAM)NULL, LPARAM(str));
		for (i = 0; i < 20; i++) str[i] = '\0';
		if (Player == 1)
		{
			lstrcat(LPWSTR(str), LPWSTR(Int2String(Round1 / 60)));
			lstrcat(LPWSTR(str), LPWSTR("��"));
			lstrcat(LPWSTR(str), LPWSTR(Int2String(Round1 % 60)));
		}
		else
		{
			lstrcat(LPWSTR(str), LPWSTR(Int2String(Round2 / 60)));
			lstrcat(LPWSTR(str), LPWSTR("��"));
			lstrcat(LPWSTR(str), LPWSTR(Int2String(Round2 % 60)));
		}
		SendMessageA(Round, WM_SETTEXT, (WPARAM)NULL, LPARAM(str));
	}
}
