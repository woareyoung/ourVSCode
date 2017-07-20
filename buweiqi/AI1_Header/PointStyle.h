#pragma once
///记录“当前点”的特殊点类型与级差分
class PointStyle
{
public:
	//加分
	int CurrentFormatPointStyle;
	double CurrentFormatScorePoor;
	int CurrentPointStyle;
	double CurrentScorePoor;
	//减分
	int LastFormatPointStyle;
	double LastFormatScorePoor;
	int LastSpecialPointStyle;
	double LastSpecialScorePoor;
	//层差分
	double RivalSinglePointPoor;//对方单点
	double RivalChipPoor;//对方缺口
	double RivalTigerMouthPoor;//对方虎口

	double RivalFormatChipPoor;//对方形成缺口
	double RivalFormatTigerMouthPoor;//对方形成虎口
	double RivalFormatEyePoor;//对方形成眼

	double MySinglePointPoor;//己方单点
	double MyChipPoor;//己方缺口
	double MyTigerMouthPoor;//己方虎口

	double MyFormatChipPoor;//己方形成缺口
	double MyFormatTigerMouthPoor;//己方形成虎口
	double MyFormatEyePoor;//己方形成眼

};

