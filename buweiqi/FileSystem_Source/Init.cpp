#include "../FileSystem_Header/FileSystem.h"

FileSystem::FileSystem()
{
	OpenFile(AIMemoryFileName, AIMemory);
	OpenFile(TempFileName, TempFile);
	OpenFile(TempHouse1Name, TempHouse1);
	OpenFile(TempHouse2Name, TempHouse2);
	NowTempHouse = &AIMemory;
}
FileSystem::FileSystem(char *FileName)
{
	OpenFile(FileName, MatchFile);
}
///���ļ�
void FileSystem::OpenFile(char *FileName, std::fstream &f)
{
	f.open(FileName, std::ios::out | std::ios::in);//�����ִ򿪷�ʽʱ�����ļ�������ʱ���ļ�ʧ��
	if (!f)
	{
		f.close();
		f.open(FileName, std::ios::out);//�����ִ򿪷�ʽʱ�����ļ�������ʱ�Զ������ļ�
		if (!f) exit(0);//�������ļ�ʧ��
		else
		{
			f.close();
			f.open(FileName, std::ios::out | std::ios::in);
			if (!f) exit(0);//�����ļ�ʧ��
		}
	}
}
///�ر��ļ�
void FileSystem::CloseFile()
{
	AIMemory.close();
	TempFile.close();
	TempHouse1.close();
	TempHouse2.close();
	remove(TempHouse1Name);
	remove(TempHouse2Name);
	remove(TempFileName);
}
FileSystem::~FileSystem()
{
	MatchFile.close();
}