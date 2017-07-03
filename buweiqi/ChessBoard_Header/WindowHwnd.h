#pragma once
#ifndef WINDOWHWND_H_INCLUDED
#define WINDOWHWND_H_INCLUDED
#include "../stdafx.h"
#include <fstream>
#include <string>

class WindowHwnd
{
public:
	HWND BackHwnd;//����һ����ť
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

	HWND CombatRecordButton;//����ս����ť
	HWND CombatRecord;//����ս������
	HWND AI1_VS_AI1;
	HWND AI1_VS_AI2;
	HWND AI1_VS_AI3;
	HWND AI2_VS_AI1;
	HWND AI2_VS_AI2;
	HWND AI2_VS_AI3;
	HWND AI3_VS_AI1;
	HWND AI3_VS_AI2;
	HWND AI3_VS_AI3;
	HWND CurrentCR;//����ǰս��������
	HWND AllCR;//����ʷս��������
	HWND CurrentWin;
	HWND CurrentLose;
	HWND AllWin;
	HWND AllLose;
	HWND DataHwnd[36];//ս������

	HINSTANCE hInst;//Ӧ�ó���ʵ�����
	HANDLE AnoHandle;//��ʱ���߳�
	HANDLE MainProcedureThead;//�������߳�
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
	//�Ƿ����ѡAI
	bool CanSelectAI1;
	bool CanSelectAI2;
	bool CanSelectAI3;
	//ˢ�¶�ս��¼
	void UpdateCRdata()
	{
		using namespace std;
		string data[36];
		fstream tempfile;
		tempfile.open("CombatRecord.txt", ios::out | ios::in);
		if (!tempfile) 
		{
			tempfile.close();
			tempfile.open("CombatRecord.txt", ios::out);//�����ļ�
			tempfile.close();
			return; 
		}
		for (int i = 0; i < 36; i++)
		{
			tempfile >> data[i];
			SendMessageA(DataHwnd[i], WM_SETTEXT, (WPARAM)NULL, LPARAM(data[i].c_str()));
		}
		tempfile.close();
	}
	//ˢ�¶�ս��¼
	void UpdateCRdata(int Player1, int Player2, int winner)
	{
		using namespace std;
		int data[36] = { 0 };
		int i;
		fstream tempfile;
		tempfile.open("CombatRecord.txt", ios::out | ios::in);
		if (!tempfile)
		{
			tempfile.close();
			tempfile.open("CombatRecord.txt", ios::out);//�����ļ�
		}
		else
		{
			tempfile.seekg(0);
			for (i = 0; i < 36; i++) tempfile >> data[i];
		}
		if (winner == 1)
		{
			if (Player1 == 1)
			{
				if (Player2 == 1) { data[0]++; data[18]++; }
				else if (Player2 == 2) { data[1]++; data[19]++; }
				else { data[2]++; data[20]++; }
			}
			else if (Player1 == 2)
			{
				if (Player2 == 1) { data[3]++; data[21]++; }
				if (Player2 == 2) { data[4]++; data[22]++; }
				else { data[5]++; data[23]++; }
			}
			else if (Player1 == 3)
			{
				if (Player2 == 1) { data[6]++; data[24]++; }
				else if (Player2 == 2) { data[7]++; data[25]++; }
				else { data[8]++; data[26]++; }
			}
		}
		else 
		{
			if (Player1 == 1)
			{
				if (Player2 == 1) { data[9]++; data[27]++; }
				else if (Player2 == 2) { data[10]++; data[28]++; }
				else { data[11]++; data[29]++; }
			}
			else if (Player1 == 2)
			{
				if (Player2 == 1) { data[12]++; data[30]++; }
				if (Player2 == 2) { data[13]++; data[31]++; }
				else { data[14]++; data[32]++; }
			}
			else if (Player1 == 3)
			{
				if (Player2 == 1) { data[15]++; data[33]++; }
				else if (Player2 == 2) { data[16]++; data[34]++; }
				else { data[17]++; data[35]++; }
			}
		}
		tempfile.close();
		tempfile.open("CombatRecord.txt", ios::out);
		for (i = 0; i < 36; i++)
		{
			if (i % 6 == 0) tempfile << endl;
			tempfile << data[i] << " ";
		}
		tempfile.close();
		UpdateCRdata();
	}
};


#endif // WINDOWHWND_H_INCLUDED
