#include "../FileSystem_Header/FileSystem.h"
///匹配当前盘面状况的记忆
void FileSystem::Match(SITUATION &StatusQuo, int player, int round)
{
	int i;
	SITUATION situa;
	OpenFile(FN.ForeName + std::to_string(round) + FN.TXT, TempFile);
	situa.Line[0] = StatusQuo.Line[0];
	if (player == 2)
	{
		for (i = 1; i < 10; i++) situa.Line[i] = DigitalChange(StatusQuo.Line[i]);
	}
	else for (i = 1; i < 10; i++) situa.Line[i] = StatusQuo.Line[i];
	while (!TempFile.eof())//循环直到文件末尾
	{
		for (i = 0; i < 9; i++)
		{
			TempFile >> value[i];
			if (situa.Line[i + 1] != value[i])
			{
				for (; i < 10; i++) TempFile >> value[i];
				break;
			}
		}
	}
}