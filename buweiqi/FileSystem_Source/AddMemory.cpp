#include "../FileSystem_Header/FileSystem.h"
///��������Ϸ��¼���ܼ����
void FileSystem::AddMemory(SITUATION *header, int Winner)
{
	int i, j;
	SITUATION *s = header;
	std::string name;
	if (Winner == 1)
	{
		for (j = 1; s->next != NULL; j++, s = s->next)
		{
			name = FN.ForeName + std::to_string(j) + FN.TXT;
			OpenFile(name, TempFile);
			//�����ǰ�������ظ���������
			if (Repeat(s))
			{
				TempFile.close();
				continue;
			}
			//���ڶ��ļ����ݶ���ĩβʱ���ļ������ʧЧ�ˣ��������´��ļ�
			TempFile.close();
			OpenFile(name, TempFile);
			TempFile.seekg(0, std::ios::end);
			//���û���ظ������¼����
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
			name = FN.ForeName + std::to_string(j) + FN.TXT;
			OpenFile(name, TempFile);
			//����Ѿ��ظ��������
			if (Repeat(s, true))
			{
				TempFile.close();
				continue;
			}
			//���ڶ��ļ����ݶ���ĩβʱ���ļ������ʧЧ�ˣ��������´��ļ�
			TempFile.close();
			OpenFile(name, TempFile);
			TempFile.seekg(0, std::ios::end);
			//���û���ظ������¼����
			for (i = 1; i < 10; i++)
			{
				//����λ���λ���ֵ�ת
				value[i] = DigitalChange(s->Line[i]);
				TempFile << value[i] << " ";
				if (i == 5) TempFile << std::endl;
			}
			TempFile << s->next->Line[0] << std::endl;
			TempFile.close();
		}
	}
}
///����ֵ���и�λ���λ���ֵĽ���
int FileSystem::DigitalChange(int num)
{
	int help1;
	int help2;
	help2 = num / 10000; //��ȡ��λ
	help1 = num % 10000; //��ȡ��λ
	return help1 * 10000 + help2;
}
///�鿴�Ƿ��ظ�
bool FileSystem::Repeat(SITUATION* sit, bool change)
{
	int i;
	int temp;//��ʱ����
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
				for (; i < 11; i++) TempFile >> temp;
				repeat = false;
				break;
			}
		}
		if (repeat)
		{
			TempFile >> temp;
			return true;
		}
	}
	return false;
}