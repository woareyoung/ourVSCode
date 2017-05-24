#include "../stdafx.h"
#include "../ChessBoard_Header/ChessBoard.h"

ChessBoard::ChessBoard()
{
	RootWidth = 1366 * 800 / GetSystemMetrics(SM_CXSCREEN);//获取屏幕宽度并设置窗体宽度
	RootHeight = 768 * 550 / GetSystemMetrics(SM_CYSCREEN);//获取屏幕高度并设置窗体高度
	Base = RootWidth > RootHeight ? RootHeight : RootWidth;//设置窗体内容规模以适应不同平台
	ChessDiameter = Base / 10;//设置棋子直径，也是棋盘中每个格子的边长
	///默认没有使用AI
	Player1isAI = false;
	Player2isAI = false;
	line = 0;
	column = 0;
	Start = false;//表示未开始游戏
	AllTime1 = 900;//总时长（单位：S）
	AllTime2 = 900;
	Round1 = 60;//步时（单位：s）
	Round2 = 60;
	onTurn = 1;//轮到谁
	Winner = 0;//胜利者
	PrintTime = true;//是否需要输出时间
					 ///结构体，保存区域
	rect.left = 0;
	rect.top = 0;
	rect.right = Base;
	rect.bottom = Base;
	///创建灰色画刷并保存
	int i;
	//设置每个交叉点的坐标值
	for (i = 1; i < 10; i++)
	{
		CrossCross[i] = i * ChessDiameter;
	}
	Tail == NULL;
	SituaHead = NULL;
	TempTail = NULL;
	Init_cross();
	Init_Pace();
}
///初始化棋盘状态数组
void ChessBoard::Init_cross()
{
	int i, j;
	for (i = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {
			cross[i][j] = 0;
		}
	}
}
///初始化下棋记录
void ChessBoard::Init_Pace()
{
	PACE *p = Tail;
	while (p != NULL)
	{
		Tail = Tail->perior;
		delete p;
		p = NULL;
		p = Tail;
	}
	SITUATION *s = SituaHead;
	while (p != NULL)
	{
		SituaHead = SituaHead->next;
		delete s;
		s = NULL;
		s = SituaHead;
	}
}