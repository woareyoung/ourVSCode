// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <conio.h>
#include <string>
/// 在PaintChess中使用
#define isPlay1onTurn 1
#define isPlay2onTurn 2
#define isAI1onTurn -1
#define isAI2onTurn -2

/// 定义黑子和白子类型
#define isBlack 1
#define isWhite 2
#define noChess 0

#define MAX_COMBAT 100 //连续对局次数
#define DISPLAY 1 //1表示在界面显示，2表示在DOS显示，0表示全显示

// TODO: reference additional headers your program requires here
