#pragma once
///��¼����ǰ�㡱������������뼶���
class PointStyle
{
public:
	//�ӷ�
	int CurrentFormatPointStyle;
	double CurrentFormatScorePoor;
	int CurrentPointStyle;
	double CurrentScorePoor;
	//����
	int LastFormatPointStyle;
	double LastFormatScorePoor;
	int LastSpecialPointStyle;
	double LastSpecialScorePoor;
	//����
	double RivalSinglePointPoor;//�Է�����
	double RivalChipPoor;//�Է�ȱ��
	double RivalTigerMouthPoor;//�Է�����

	double RivalFormatChipPoor;//�Է��γ�ȱ��
	double RivalFormatTigerMouthPoor;//�Է��γɻ���
	double RivalFormatEyePoor;//�Է��γ���

	double MySinglePointPoor;//��������
	double MyChipPoor;//����ȱ��
	double MyTigerMouthPoor;//��������

	double MyFormatChipPoor;//�����γ�ȱ��
	double MyFormatTigerMouthPoor;//�����γɻ���
	double MyFormatEyePoor;//�����γ���

};

