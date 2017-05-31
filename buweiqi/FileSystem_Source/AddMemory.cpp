#include "../FileSystem_Header/FileSystem.h"
///将本局游戏记录到总记忆库
void FileSystem::AddMemory(SITUATION *header, int Winner)
{
	int i, j;
	SITUATION *s = header;
	if (Winner == 1)
	{
		for (j = 1; s->next != NULL; j++)
		{
			OpenFile(FN.ForeName + std::to_string(j) + FN.TXT, TempFile);
			for (i = 1; i < 10; i++)
			{
				if (i == 6) TempFile << std::endl;
				TempFile << s->Line[i] << " ";
			}
			TempFile << s->next->Line[0] << std::endl;
			s = s->next;
			TempFile.close();
		}
	}
	else
	{
		for (j = 1; s->next != NULL; j++)
		{
			OpenFile(FN.ForeName + std::to_string(j) + FN.TXT, TempFile);
			for (i = 1; i < 10; i++)
			{
				///将小数与整数部分调转
				value[i] = DigitalChange(s->Line[i]);
				TempFile << value[i] << " ";
				if (i == 5) TempFile << std::endl;
			}
			TempFile << s->next->Line[0] << std::endl;
			s = s->next;
			TempFile.close();
		}
	}
}
///对数值进行小数与整数部分的交换
double FileSystem::DigitalChange(double num)
{
	double help1;
	int help2;
	help2 = num;
	help1 = num - help2;
	return help1 * 10000 + (double)help2 / 10000;
}