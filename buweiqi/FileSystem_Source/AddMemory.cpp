#include "../FileSystem_Header/FileSystem.h"

///��������Ϸ��¼���ܼ����
void FileSystem::AddMemory(std::list<SITUATION> header, int winner)
{
	std::list<SITUATION>::iterator it = header.begin();
	std::string name;//�ļ���
	int temp;
	int Win;
	std::string tempstr;
	bool repeat = false;
	for (int j = 1; it != header.end(); j++, it++)
	{
		repeat = false;
		name = FN.ForeName + std::to_string(j) + FN.TXT;
		OpenFile(name, TempFile);
		TempFile.seekg(0);
		//���ļ��е����ݶ����ݴ�����
		while (!TempFile.eof())
		{
			TempFile >> tempstr >> temp >> Win;
			if (it->BoardStatus == tempstr && it->ChessPosition == temp)
			{
				repeat = true;
				break;
			}
		}
		if (!repeat)
		{
			TempFile.close();
			OpenFile(name, TempFile);
			TempFile.seekg(0, std::ios::end);
			TempFile << it->BoardStatus << " " << it->ChessPosition << " " << winner << std::endl;
		}
		TempFile.close();
	}
}
