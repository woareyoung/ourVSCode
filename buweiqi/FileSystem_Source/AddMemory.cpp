#include "../FileSystem_Header/FileSystem.h"
///��������Ϸ��¼���ܼ����
void FileSystem::AddMemory(SITUATION *header, int Winner)
{
	int i, j;
	SITUATION *s = header;
	if (Winner == 1)
	{
		for (j = 1; s->next != NULL; j++, s = s->next)
		{
			OpenFile(FN.ForeName + std::to_string(j) + FN.TXT, TempFile);
			//�����ǰ�������ظ���������
			if (Repeat(s))
			{
				TempFile.close();
				continue;
			}
			//���û���ظ������¼����
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
			//����Ѿ��ظ��������
			if (Repeat(s, true))
			{
				TempFile.close();
				continue;
			}
			//���û���ظ������¼����
			for (i = 1; i < 10; i++)
			{
				//��С�����������ֵ�ת
				value[i] = DigitalChange(s->Line[i]);
				TempFile << value[i] << " ";
				if (i == 5) TempFile << std::endl;
			}
			TempFile << s->next->Line[0] << std::endl;
			TempFile.close();
		}
	}
}
///����ֵ����С�����������ֵĽ���
double FileSystem::DigitalChange(double num)
{
	double help1;
	int help2;
	help2 = num; //��ȡ��������
	help1 = num - help2; //��ȡС������
	return help1 * 10000 + (double)help2 / 10000;
}
///�鿴�Ƿ��ظ�
bool FileSystem::Repeat(SITUATION* sit, bool change)
{
	int i;
	double temp;
	bool repeat = false;
	TempFile.seekg(0);
	//�浽��ʱ������
	for (i = 0; i < 10; i++)
	{
		if(change) value[i] = DigitalChange(sit->Line[i]);
		else value[i] = sit->Line[i];
	}
	//һֱѭ����ֱ���ļ���β
	while (!TempFile.eof())
	{
		repeat = true;
		for (i = 1; i < 10; i++)
		{
			TempFile >> temp;
			//����в���ȵģ��Ͷ�ȡ����һ��¼
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