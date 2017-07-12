#include "../FileSystem_Header/FileSystem.h"

///将本局游戏记录到总记忆库
void FileSystem::AddMemory(std::shared_ptr<SITUATION> header)
{
	std::shared_ptr<SITUATION> s = header;
	std::string name;//文件名
	int temp;
	std::string tempstr;
	bool repeat = false;
	for (int j = 1; s != nullptr; j++)
	{
		name = FN.ForeName + std::to_string(j) + FN.TXT;
		OpenFile(name, TempFile);
		TempFile.seekg(0);
		//将文件中的内容读到暂存区中
		while (!TempFile.eof())
		{
			TempFile >> tempstr >> temp;
			if (s->BoardStatus == tempstr && s->ChessPosition == temp)
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
			TempFile << s->BoardStatus << " " << s->ChessPosition << std::endl;
		}
		TempFile.close();
		header = s;
		s = s->next;
		header = nullptr;
	}
}
