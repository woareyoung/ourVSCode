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

	//    hdc = BeginPaint(ChessBoardHwnd, &ps);//��ȡ�豸�����Ļ�������ʾ�豸��
	hdc = GetDC(ChessBoardHwnd);
	hdcBuffer = CreateCompatibleDC(hdc);//�������ݻ����豸
										//    hBitMap = CreateCompatibleBitmap(hdcBuffer, Base, Base);//��������λͼ
										//    SelectObject(hdcBuffer, hBitMap);
										//    FillRect(hdcBuffer, &rect, (HBRUSH)GetStockObject(LTGRAY_BRUSH));//��䱳��
}
///�ͷŻ�����
void ChessBoard::DelHandle()
{
	//    DeleteObject(hBitMap);//ɾ��λͼ���
	DeleteDC(hdcBuffer);//ɾ���豸���
	ReleaseDC(ChessBoardHwnd, hdc);//�ͷ���ʾ�豸
								   //    EndPaint(ChessBoardHwnd,&ps);//������ͼ����
								   //    UpdateWindow(ChessBoardHwnd);
}
