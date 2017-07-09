#include "../FileSystem_Header/FileSystem.h"

///将本局游戏记录到总记忆库
void FileSystem::AddMemory(std::shared_ptr<SITUATION> header, int Winner)
{
	mem = nullptr;
	if (Winner == 1) DistributeThread(header, false);
	else DistributeThread(header, true);
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
void FileSystem::DistributeThread(std::shared_ptr<SITUATION> header, bool change)
{
	std::shared_ptr<SITUATION> s[4];
	s[0] = header;
	s[1] = s[0]->next;
	s[2] = s[1]->next;
	s[3] = s[2]->next;
	bool wait[3] = { true };//标记各线程是否已经执行完毕
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
	std::string name;//文件名
	bool rep = false;//标记是否已经查出是重复的
	bool comp = true;//标记是否需要比较是否重复
	bool need = true;//标记是否需要写进文件
	int i;
	name = FN.ForeName + std::to_string(j) + FN.TXT;
	OpenFile(name, FinalFile[ThreadNumber]);
	FinalFile[ThreadNumber].seekg(0);
	///将文件中的内容读到内存中
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
		//如果已经检查出是重复的，那么就不用检查了
		if (comp)
		{
			rep = Repeat(s, tempMEM, change);//比较是否重复
			if(rep) comp = false;
		}
		//如果当前盘面已重复，则把次数加1
		if (need && rep)
		{
			tempMEM->count++;
			need = false;
		}
	}
	FinalFile[ThreadNumber].close();
	//↓↓↓↓↓↓由于C++文件流读取，最后一组数据会读两遍，所以要删除一组↓↓↓↓↓↓//
	std::shared_ptr<MEMO> ttt;//辅助变量
	ttt = tempMEM;
	tempMEM = tempMEM->prior;
	ttt = nullptr;
	tempMEM->next = nullptr;
	//↑↑↑↑↑↑↑由于C++文件流读取，最后一组数据会读两遍，所以要删除一组↑↑↑↑↑↑//
	//如果当前盘面是记录中没有出现过的
	if (!rep)
	{
		tempMEM->next = std::shared_ptr<MEMO>(new MEMO);
		tempMEM->next->prior = tempMEM;
		tempMEM = tempMEM->next;
		tempMEM->next = nullptr;
		for (i = 0; i < 10; i++) tempMEM->value[i] = s->Line[i];
		tempMEM->count = 1;
	}
	///---------------把修改后的内容重新写进文件-------------------//
	FinalFile[ThreadNumber].open(name, std::ios::out);//清空文件内容
	//↓↓↓↓↓↓把修改后的内容写进文件↓↓↓↓↓↓//
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
	//↑↑↑↑↑↑↑把修改后的内容写进文件↑↑↑↑↑↑//
}
