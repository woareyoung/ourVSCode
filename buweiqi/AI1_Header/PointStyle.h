#pragma once
#ifndef POINTSTYLE_H_INCLUDED
#define POINTSTYLE_H_INCLUDED
///��¼����ǰ�㡱������������뼶���
class PointStyle
{
public:
	//�ӷ�
	int CurrentFormatPointStyle;
	int CurrentFormatScorePoor;
	int CurrentPointStyle;
	int CurrentScorePoor;
	//����
	int LastFormatPointStyle;
	int LastFormatScorePoor;
	int LastSpecialPointStyle;
	int LastSpecialScorePoor;
	//����
	int RivalSinglePointPoor;//�Է�����
	int RivalChipPoor;//�Է�ȱ��
	int RivalTigerMouthPoor;//�Է�����

	int RivalFormatChipPoor;//�Է��γ�ȱ��
	int RivalFormatTigerMouthPoor;//�Է��γɻ���
	int RivalFormatEyePoor;//�Է��γ���

	int MySinglePointPoor;//��������
	int MyChipPoor;//����ȱ��
	int MyTigerMouthPoor;//��������

	int MyFormatChipPoor;//�����γ�ȱ��
	int MyFormatTigerMouthPoor;//�����γɻ���
	int MyFormatEyePoor;//�����γ���

};

#endif // POINTSTYLE_H_INCLUDED
