#include "../stdafx.h"
#include "../ChessBoard_Header/ChessBoard.h"
///响应重绘消息
void ChessBoard::RePaint()
{
	SetHandle();
	PaintBoard();//画棋盘
	DelHandle();
}
///设置缓冲区
void ChessBoard::SetHandle()
{
	hdc = GetDC(ChessBoardHwnd);
	hdcBuffer = CreateCompatibleDC(hdc);//创建兼容缓冲设备
}
///释放缓冲区
void ChessBoard::DelHandle()
{
	DeleteDC(hdcBuffer);//删除设备句柄
	ReleaseDC(ChessBoardHwnd, hdc);//释放显示设备
}
