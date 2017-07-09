#include "../FileSystem_Header/FileSystem.h"

///��������Ϸ��¼���ܼ����
void FileSystem::AddMemory(std::shared_ptr<SITUATION> header, int Winner)
{
	mem = nullptr;
	if (Winner == 1) DistributeThread(header, false);
	else DistributeThread(header, true);
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
bool FileSystem::Repeat(std::shared_ptr<SITUATION> sit, std::shared_ptr<MEMO> tempnode, bool change)
{
	int i;
	//�浽��ʱ������
	for (i = 1; i < 10; ++i)
	{
		if(change) value[i] = DigitalChange(sit->Line[i]);
 		else value[i] = sit->Line[i];
	}
	value[0] = sit->Line[0];
	for (i = 0; i < 10; i++)
	{
		if (value[i] == tempnode->value[i]) continue;
		else return false;
	}
	return true;
}
void FileSystem::DistributeThread(std::shared_ptr<SITUATION> header, bool change)
{
	std::shared_ptr<SITUATION> s[4];
	s[0] = header;
	s[1] = s[0]->next;
	s[2] = s[1]->next;
	s[3] = s[2]->next;
	bool wait[3] = { true };//��Ǹ��߳��Ƿ��Ѿ�ִ�����
	std::async(std::launch::async, [&]()
	{
		for (int j = 1; s[0] != nullptr; j = j + 4)
		{
			ReadMemoryToFile(s[0], j, change, 0);
			s[0] = s[0]->fourPtr;
		}
		wait[0] = false;
	});
	std::async(std::launch::async, [&]()
	{
		for (int j = 2; s[1] != nullptr; j = j + 4)
		{
			ReadMemoryToFile(s[1], j, change, 1);
			s[1] = s[1]->fourPtr;
		}
		wait[1] = false;
	});
	std::async(std::launch::async, [&]()
	{
		for (int j = 3; s[2] != nullptr; j = j + 4)
		{
			ReadMemoryToFile(s[2], j, change, 2);
			s[2] = s[2]->fourPtr;
		}
		wait[2] = false;
	});
	for (int j = 4; s[3] != nullptr; j = j + 4)
	{
		ReadMemoryToFile(s[3], j, change, 3);
		s[3] = s[3]->fourPtr;
	}
	while (wait[0] && wait[1] && wait[2]) {}
}
void FileSystem::ReadMemoryToFile(std::shared_ptr<SITUATION> s, int j, bool change, int ThreadNumber)
{
	std::string name;//�ļ���
	bool rep = false;//����Ƿ��Ѿ�������ظ���
	bool comp = true;//����Ƿ���Ҫ�Ƚ��Ƿ��ظ�
	bool need = true;//����Ƿ���Ҫд���ļ�
	int i;
	name = FN.ForeName + std::to_string(j) + FN.TXT;
	OpenFile(name, FinalFile[ThreadNumber]);
	FinalFile[ThreadNumber].seekg(0);
	///���ļ��е����ݶ����ڴ���
	while (!FinalFile[ThreadNumber].eof())
	{
		if (mem == nullptr)
		{
			mem = std::shared_ptr<MEMO>(new MEMO);
			mem->prior = nullptr;
			mem->next = nullptr;
			tempMEM = mem;
		}
		else
		{
			tempMEM->next = std::shared_ptr<MEMO>(new MEMO);
			tempMEM->next->prior = tempMEM;
			tempMEM = tempMEM->next;
			tempMEM->next = nullptr;
		}
		for (i = 1; i < 10; i++) FinalFile[ThreadNumber] >> tempMEM->value[i];
		FinalFile[ThreadNumber] >> tempMEM->value[0] >> tempMEM->count;
		//����Ѿ��������ظ��ģ���ô�Ͳ��ü����
		if (comp)
		{
			rep = Repeat(s, tempMEM, change);//�Ƚ��Ƿ��ظ�
			if(rep) comp = false;
		}
		//�����ǰ�������ظ�����Ѵ�����1
		if (need && rep)
		{
			tempMEM->count++;
			need = false;
		}
	}
	FinalFile[ThreadNumber].close();
	//����������������C++�ļ�����ȡ�����һ�����ݻ�����飬����Ҫɾ��һ�������������//
	std::shared_ptr<MEMO> ttt;//��������
	ttt = tempMEM;
	tempMEM = tempMEM->prior;
	ttt = nullptr;
	tempMEM->next = nullptr;
	//������������������C++�ļ�����ȡ�����һ�����ݻ�����飬����Ҫɾ��һ�������������//
	//�����ǰ�����Ǽ�¼��û�г��ֹ���
	if (!rep)
	{
		tempMEM->next = std::shared_ptr<MEMO>(new MEMO);
		tempMEM->next->prior = tempMEM;
		tempMEM = tempMEM->next;
		tempMEM->next = nullptr;
		for (i = 0; i < 10; i++) tempMEM->value[i] = s->Line[i];
		tempMEM->count = 1;
	}
	///---------------���޸ĺ����������д���ļ�-------------------//
	FinalFile[ThreadNumber].open(name, std::ios::out);//����ļ�����
	//���������������޸ĺ������д���ļ�������������//
	for (tempMEM = mem; tempMEM != nullptr; mem = tempMEM, tempMEM = tempMEM->next, mem = nullptr)
	{
		if (tempMEM->value[0] == 0) continue;
		for (i = 1; i < 10; i++)
		{
			FinalFile[ThreadNumber] << tempMEM->value[i] << " ";
			if (i == 5) FinalFile[ThreadNumber] << std::endl;
		}
		FinalFile[ThreadNumber] << tempMEM->value[0] << " " << tempMEM->count << std::endl;
	}
	FinalFile[ThreadNumber].close();
	//�����������������޸ĺ������д���ļ�������������//
}
