#include "../FileSystem_Header/FileSystem.h"

FileSystem::FileSystem()
{
	AIMemoryFileName = "AIMemory.txt";//�ܼ�����ļ���������ֻ���Ӳ����ٵ��ļ���
	TempFileName = "TempFile.txt";//��ʱ����������ļ�����¼�뵱ǰ����״��һģһ���ļ��䣩
	TempHouse1Name = "TempHouse1.txt";//��ʱ������ļ�
	TempHouse2Name = "TempHouse2.txt";//��ʱ������ļ�
}
///���ļ�
void FileSystem::OpenFile(std::string FileName, std::fstream &f)
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
	remove(TempHouse1Name.data());
	remove(TempHouse2Name.data());
	remove(TempFileName.data());
}
FileSystem::~FileSystem()
{
}