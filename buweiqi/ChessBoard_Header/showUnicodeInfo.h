#pragma once

#include <tchar.h>
#include <windows.h> 
#include <iostream>
#include <conio.h>

namespace {

#define Black 1 // ����
#define White 2// ����

	void showInfo(const char* info) {
		HANDLE hOutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);//��ÿ���̨������
		DWORD nRet = 0;
		TCHAR buf[100] = { 0 };//��������ַ��Ļ�����
		int num = MultiByteToWideChar(0, 0, info, -1, NULL, 0);
		wchar_t *wide = new wchar_t[num];
		MultiByteToWideChar(0, 0, info, -1, wide, num);
		lstrcpy(buf, wide);
		WriteConsole(hOutputHandle, buf, lstrlen(buf), &nRet, NULL);
	}

	void showUnicode(int pos) {
		std::string ch[20] = { "��","��","��","��","��","��","��","��","��","��" };
		showInfo(ch[pos].c_str());
	}

	void showChessBoard(int cross[10][10]) {
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				if (i == 0) {
					showUnicode(j);
					showInfo("  ");
				}
				else if (j == 0) {
					showUnicode(i);
					showInfo("  ");
				}
				else {
					if (cross[i][j] != Black && cross[i][j] != White) {
						showInfo("    ");
					}
					else if (cross[i][j] == Black) {
						showInfo("��  ");
					}
					else if (cross[i][j] == White) {
						showInfo("��  ");
					}
				}
			}
			_cprintf("\n\n");
		}

	}
};