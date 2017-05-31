#include "../FileSystem_Header/FileSystem.h"

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