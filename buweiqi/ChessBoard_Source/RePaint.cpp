#include "../stdafx.h"
#include "../ChessBoard_Header/ChessBoard.h"
///��Ӧ�ػ���Ϣ
void ChessBoard::RePaint()
{
	SetHandle();
	PaintBoard();//������
	DelHandle();
}
///���û�����
void ChessBoard::SetHandle()
{
	hdc = GetDC(ChessBoardHwnd);
	hdcBuffer = CreateCompatibleDC(hdc);//�������ݻ����豸
}
///�ͷŻ�����
void ChessBoard::DelHandle()
{
	DeleteDC(hdcBuffer);//ɾ���豸���
	ReleaseDC(ChessBoardHwnd, hdc);//�ͷ���ʾ�豸
}
