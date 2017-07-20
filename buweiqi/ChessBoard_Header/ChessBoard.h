#pragma once

#ifdef __ChessBoard

#endif // __ChessBoard

#include "../stdafx.h"
#include "WindowSize.h"
#include "WindowHwnd.h"
#include "PlayerInfo.h"
#include "Chess.h"
#include "../AI1_Header/AI1.h"
#include "../AI2_Header/AI2.h"
#include "../AI3_Header/AI3.h"
#include "../AI4_Header/AI4.h"

#ifdef __cplusplus
extern "C" {
#endif
	class ChessBoard;
#ifdef __cplusplus
}
#endif
class ChessBoard : public WindowSize, public WindowHwnd, public PlayerInfo, public Chess
{
public:


	///��InitData.cpp�ļ���
	ChessBoard();//���캯��
	void Init_cross();//��ʼ������״̬����
	void Init_Pace();//��ʼ�������¼
	///��PaintBoard.cpp�ļ���
	void PaintBoard();//������
	void BackPace();//����һ�����ܺ���
	///��PaintTimer.cpp�ļ���
	void PaintTimer(HWND All, HWND Round, int Player, bool FirstPrint = false);//���ʱ����Ϣ
	///��PaintChess.cpp�ļ���
	void PaintChess();//�����Ӻ���
	void PaintAChess(int type);//ʵ�л�����
	void GetPointPosition(DWORD lParam);//��ȡ����λ��
	void DisplayOnTurn(int type);
	///��AddList.cpp�ļ���
	void AddList(int type);
	void AddSituationList();
	///��ReStart.cpp�ļ���
	void ReStart();
	///��RePaint.cpp�ļ���
	void RePaint();//��Ӧ�����ػ���Ϣ
	void SetHandle();//���û�����
	void DelHandle();//�ͷŻ�����

	POINT point;//���ڱ������λ��
	int onTurn;//�ֵ�˭
	int CombatNumber;
	int CurrentRoundNumber;

	AI *Player1AI;
	AI *Player2AI;
	
	AI1 ai11;
	AI2 ai22;
	AI3 ai33;
	AI4 ai44;
	AI1 ai1;
	AI2 ai2;
	AI3 ai3;
	AI4 ai4;

	int line;//����λ�õ�����
	int column;//����λ�õ�����
private:
	int CrossCross[10];//�������õ�����ֵ

	///����ת�� ��int2String.cpp�ļ���
	std::string String;//��������ת��
	std::string Int2String(int num);//���ͻ���Ϊ�ַ���
	void addChar(int mid);//���ڸ���ת��
};
