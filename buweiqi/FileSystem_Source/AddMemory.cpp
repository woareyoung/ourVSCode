#include "../FileSystem_Header/FileSystem.h"

///将本局游戏记录到总记忆库
void FileSystem::AddMemory(std::shared_ptr<SITUATION> header, int Winner)
{
	mem = nullptr;
	if (Winner == 1) ReadFileToMemory(header, false);
	else ReadFileToMemory(header, true);
}
///对数值进行高位与低位部分的交换
int FileSystem::DigitalChange(int num)
{
	int help1;
	int help2;
	help2 = num / 10000; //获取高位
	help1 = num % 10000; //获取低位
	return help1 * 10000 + help2;
}
///查看是否重复
bool FileSystem::Repeat(std::shared_ptr<SITUATION> sit, std::shared_ptr<MEMO> tempnode, bool change)
{
	int i;
	//存到临时变量中
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
void FileSystem::ReadFileToMemory(std::shared_ptr<SITUATION> header, bool change)
{
	std::shared_ptr<SITUATION> s = header;
	std::shared_ptr<MEMO> ttt;
	std::string name;
	int i, j;
	for (j = 1; s != NULL; ++j, s = s->next)
	{
		name = FN.ForeName + std::to_string(j) + FN.TXT;
		OpenFile(name, TempFile);
		TempFile.seekg(0);
		while (!TempFile.eof())
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
				ttt = std::shared_ptr<MEMO>(new MEMO);
				tempMEM->next = ttt;
				ttt->prior = tempMEM;
				ttt->next = nullptr;
				tempMEM = ttt;
			}
			for (i = 1; i < 10; i++) 
				TempFile >> tempMEM->value[i];
			TempFile >> tempMEM->value[0] >> tempMEM->count;
			//如果当前盘面已重复，则加1
			if (Repeat(s, tempMEM, change))
			{
				tempMEM->count++;
				continue;
			}
			else
			{
				tempMEM->next = std::shared_ptr<MEMO>(new MEMO);
				tempMEM->next->prior = tempMEM;
				tempMEM = tempMEM->next;
				tempMEM->next = nullptr;
				for (i = 0; i < 10; i++)
					tempMEM->value[i] = s->Line[i];
				tempMEM->count = 1;
			}
		}
		TempFile.close();
		TempFile.open(name, std::ios::out);
		bool repeat = true;
		ttt = tempMEM->prior;
		for (i = 0; i < 10; i++)
		{
			if (tempMEM->value[i] != ttt->value[i]) 
			{
				repeat = false;
				break;
			}
		}
		if (!repeat)
		{
			ttt = ttt->prior;
			repeat = true;
			for (i = 0; i < 10; i++)
			{
				if (tempMEM->value[i] != ttt->value[i])
				{
					repeat = false;
					break;
				}
			}
		}
		if (repeat)
		{
			ttt = tempMEM->prior;
			tempMEM = nullptr;
			ttt->next = nullptr;
		}
		for (tempMEM = mem; tempMEM != nullptr; )
		{
			for (i = 1; i < 10; i++)
			{
				TempFile << tempMEM->value[i] << " ";
				if (i == 5) TempFile << std::endl;
			}
			TempFile << tempMEM->value[0] << " " << tempMEM->count << std::endl;
			mem = tempMEM;
			tempMEM = tempMEM->next;
			mem = nullptr;
		}
		TempFile.close();
	}
}