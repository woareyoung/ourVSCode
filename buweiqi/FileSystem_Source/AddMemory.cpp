#include "../FileSystem_Header/FileSystem.h"
///将本局游戏记录到总记忆库
void FileSystem::AddMemory(SITUATION *header, int Winner)
{
	int i, j;
	SITUATION *s = header;
	if (Winner == 1)
	{
		for (j = 1; s->next != NULL; j++, s = s->next)
		{
			OpenFile(FN.ForeName + std::to_string(j) + FN.TXT, TempFile);
			//如果当前盘面已重复，则跳过
			if (Repeat(s))
			{
				TempFile.close();
				continue;
			}
			//如果没有重复，则记录下来
			TempFile.seekg(0);
			for (i = 1; i < 10; i++)
			{
				if (i == 6) TempFile << std::endl;
				TempFile << s->Line[i] << " ";
			}
			TempFile << s->next->Line[0] << std::endl;
			TempFile.close();
		}
	}
	else
	{
		for (j = 1; s->next != NULL; j++, s = s->next)
		{
			OpenFile(FN.ForeName + std::to_string(j) + FN.TXT, TempFile);
			//如果已经重复，则忽略
			if (Repeat(s, true))
			{
				TempFile.close();
				continue;
			}
			//如果没有重复，则记录下来
			for (i = 1; i < 10; i++)
			{
				//将小数与整数部分调转
				value[i] = DigitalChange(s->Line[i]);
				TempFile << value[i] << " ";
				if (i == 5) TempFile << std::endl;
			}
			TempFile << s->next->Line[0] << std::endl;
			TempFile.close();
		}
	}
}
///对数值进行小数与整数部分的交换
double FileSystem::DigitalChange(double num)
{
	double help1;
	int help2;
	help2 = num; //获取整数部分
	help1 = num - help2; //获取小数部分
	return help1 * 10000 + (double)help2 / 10000;
}
///查看是否重复
bool FileSystem::Repeat(SITUATION* sit, bool change)
{
	int i;
	double temp;
	bool repeat = false;
	TempFile.seekg(0);
	//存到临时变量中
	for (i = 0; i < 10; i++)
	{
		if(change) value[i] = DigitalChange(sit->Line[i]);
		else value[i] = sit->Line[i];
	}
	//一直循环，直到文件结尾
	while (!TempFile.eof())
	{
		repeat = true;
		for (i = 1; i < 10; i++)
		{
			TempFile >> temp;
			//如果有不相等的，就读取到下一记录
			if (temp != value[i])
			{
				for (; i < 10; i++) TempFile >> temp;
				repeat = false;
				break;
			}
		}
		if (repeat) return true;
	}
	return false;
}