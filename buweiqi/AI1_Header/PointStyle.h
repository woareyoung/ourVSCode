#pragma once
#ifndef POINTSTYLE_H_INCLUDED
#define POINTSTYLE_H_INCLUDED
///记录“当前点”的特殊点类型与级差分
class PointStyle
{
public:
	//加分
	int CurrentFormatPointStyle;
	int CurrentFormatScorePoor;
	int CurrentPointStyle;
	int CurrentScorePoor;
	//减分
	int LastFormatPointStyle;
	int LastFormatScorePoor;
	int LastSpecialPointStyle;
	int LastSpecialScorePoor;
	//层差分
	int RivalSinglePointPoor;//对方单点
	int RivalChipPoor;//对方缺口
	int RivalTigerMouthPoor;//对方虎口

	int RivalFormatChipPoor;//对方形成缺口
	int RivalFormatTigerMouthPoor;//对方形成虎口
	int RivalFormatEyePoor;//对方形成眼

	int MySinglePointPoor;//己方单点
	int MyChipPoor;//己方缺口
	int MyTigerMouthPoor;//己方虎口

	int MyFormatChipPoor;//己方形成缺口
	int MyFormatTigerMouthPoor;//己方形成虎口
	int MyFormatEyePoor;//己方形成眼

};

#endif // POINTSTYLE_H_INCLUDED
