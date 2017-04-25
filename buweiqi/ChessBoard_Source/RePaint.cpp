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

	//    hdc = BeginPaint(ChessBoardHwnd, &ps);//获取设备上下文环境（显示设备）
	hdc = GetDC(ChessBoardHwnd);
	hdcBuffer = CreateCompatibleDC(hdc);//创建兼容缓冲设备
										//    hBitMap = CreateCompatibleBitmap(hdcBuffer, Base, Base);//创建兼容位图
										//    SelectObject(hdcBuffer, hBitMap);
										//    FillRect(hdcBuffer, &rect, (HBRUSH)GetStockObject(LTGRAY_BRUSH));//填充背景
}
///释放缓冲区
void ChessBoard::DelHandle()
{
	//    DeleteObject(hBitMap);//删除位图句柄
	DeleteDC(hdcBuffer);//删除设备句柄
	ReleaseDC(ChessBoardHwnd, hdc);//释放显示设备
								   //    EndPaint(ChessBoardHwnd,&ps);//结束绘图功能
								   //    UpdateWindow(ChessBoardHwnd);
}
