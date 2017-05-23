#include "../FileSystem_Header/FileSystem.h"
///��������Ϸ��¼���ܼ����
void FileSystem::AddMemory(SITUATION *header, int Winner)
{
	OpenFile(AIMemoryFileName, AIMemory);
	SITUATION *s = header;
	AIMemory.seekg(0, std::ios::end);
	int i;
	if (Winner == 1)
	{
		while (s->next != NULL)
		{
			for (i = 1; i < 10; i++)
			{
				if (i == 6) AIMemory << std::endl;
				AIMemory << s->Line[i] << " ";
			}
			AIMemory << s->next->Line[0] << std::endl;
			s = s->next;
		}
	}
	else
	{
		while (s->next != NULL)
		{
			for (i = 1; i < 10; i++)
			{
				///��С�����������ֵ�ת
				value[i] = DigitalChange(s->Line[i]);
				AIMemory << value[i] << " ";
				if (i == 5) AIMemory << std::endl;
			}
			AIMemory << s->next->Line[0] << std::endl;
			s = s->next;
		}
	}
	AIMemory.close();
}
///����ֵ����С�����������ֵĽ���
double FileSystem::DigitalChange(double num)
{
	double help1;
	int help2;
	help2 = num;
	help1 = num - help2;
	return help1 * 10000 + (double)help2 / 10000;
}