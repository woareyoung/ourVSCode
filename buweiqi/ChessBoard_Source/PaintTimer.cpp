#include "../stdafx.h"
#include "../buweiqi.h"
#include "../ChessBoard_Header/ChessBoard.h"
///�������ʱ��Ϣ
//������All  ��Ϸ��ʣ��ʱ��Ĵ��ھ��
//������Round  ��ʱ���ھ��
//������Player  ��ұ��
//������FirstPrint  �Ƿ��ǵ�һ�����ʱ�䣨Ĭ��Ϊfalse��
void ChessBoard::PaintTimer(HWND All, HWND Round, int Player, bool FirstPrint)
{
	if (FirstPrint)
	{
		SendMessageA(All, WM_SETTEXT, (WPARAM)NULL, (LPARAM)"15��00");
		SendMessageA(Round, WM_SETTEXT, (WPARAM)NULL, (LPARAM)"1��00");
		return;
	}
	if (Start)
	{
		std::string str;
		if (Player == 1)
		{
			str = Int2String(AllTime1 / 60);
			str += "��";
			str += Int2String(AllTime1 % 60);
		}
		else
		{
			str = Int2String(AllTime2 / 60);
			str += "��";
			str += Int2String(AllTime2 % 60);
		}
		SendMessageA(All, WM_SETTEXT, (WPARAM)NULL, (LPARAM)str.substr(0, 7).c_str());
		SendMessageA(All, WM_PAINT, (WPARAM)NULL, (LPARAM)NULL);
		if (Player == 1)
		{
			str = Int2String(Round1 / 60);
			str += "��";
			str += Int2String(Round1 % 60);
		}
		else
		{
			str = Int2String(Round2 / 60);
			str += "��";
			str += Int2String(Round2 % 60);
		}
		SendMessageA(Round, WM_SETTEXT, (WPARAM)NULL, (LPARAM)str.substr(0, 7).c_str());
		SendMessageA(Round, WM_PAINT, (WPARAM)NULL, (LPARAM)NULL);
	}
}
