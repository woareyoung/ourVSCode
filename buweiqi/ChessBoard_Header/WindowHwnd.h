#pragma once
#ifndef WINDOWHWND_H_INCLUDED
#define WINDOWHWND_H_INCLUDED
#include "../stdafx.h"

class WindowHwnd
{
public:
	HWND RootHwnd;//Ӧ�ó���������
	HWND TimerAll1Hwnd;//���1��ʱ����Ϣ
	HWND TimerAll2Hwnd;//���2��ʱ����Ϣ
	HWND TimerRound1Hwnd;//���1��ʱ��Ϣ
	HWND TimerRound2Hwnd;//���2��ʱ��Ϣ
	HWND Timer1A;//���1ʣ��ʱ������
	HWND Timer2A;//���2ʣ��ʱ������
	HWND Timer1R;//���1ʣ��ʱ������
	HWND Timer2R;//���2ʣ��ʱ������
	HWND Player1Hwnd;//���1��player1������
	HWND Player2Hwnd;//���2��player2������
	HWND Player1UseAI;//���1ʹ��AI��ť
	HWND Player2UseAI;//���2ʹ��AI��ť
	HWND ChessBoardHwnd;//��������
	HWND StartGame;//��ʼ��Ϸ��ť
	HWND TurnToBlack;//�ֵ����1������
	HWND TurnToWhite;//�ֵ����2������
	HWND SelectAI;//ѡ��AI����
	HWND SelectAI1;//ѡ��AI1��ť����ѡ��AI������
	HWND SelectAI2;//ѡ��AI2��ť����ѡ��AI������
	HWND SelectAI3;//ѡ��AI3��ť����ѡ��AI������
	HINSTANCE hInst;//Ӧ�ó���ʵ�����
	HANDLE TimeHandle;//��ʱ���߳�
	bool PrintTime;

	RECT MainWinRect;//��¼������λ��

	HDC hdcBuffer;//����
	HBITMAP hBitMap;//�ڴ�λͼ
	RECT rect;//��������
	PAINTSTRUCT ps;
	HDC hdc;//��ʾ�豸
	HBITMAP BlackChess;//������ӵ�ͼƬ
	HBITMAP WhiteChess;//������ӵ�ͼƬ
	HBITMAP Board;//�������̵�ͼƬ
	HBITMAP Tips;//������ʾλͼ
};


#endif // WINDOWHWND_H_INCLUDED
