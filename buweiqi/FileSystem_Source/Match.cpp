#include "../FileSystem_Header/FileSystem.h"
///匹配当前盘面状况的记忆
NEXTPACE* FileSystem::Match(SITUATION &StatusQuo, int player, int round)
{
	NEXTPACE *np = NULL;//记录“下一步”应对方法的链表的头结点
	NEXTPACE *temp = np;
	int i;
 	SITUATION situa;
	OpenFile(FN.ForeName + std::to_string(round) + FN.TXT, TempFile);//打开当前盘面状况的记录文件
	situa.Line[0] = StatusQuo.Line[0];
	for (i = 1; i < 10; i++) situa.Line[i] = StatusQuo.Line[i];
	TempFile.seekg(0);
	while (!TempFile.eof())//循环直到文件末尾
	{
		for (i = 1; i < 10; i++)
		{
			TempFile >> value[i];
			if (situa.Line[i] != value[i])//只要有一个数据不相同，则匹配下一条记录
			{
				for (; i < 10; i++) 
					TempFile >> value[0];
				break;
			}
			else if (i == 9)
			{
				if (np == NULL)
				{
					temp = new NEXTPACE;
					np = temp;
					temp->next = NULL;
				}
				else
				{
					temp->next = new NEXTPACE;
					temp = temp->next;
					temp->next = NULL;
				}
				TempFile >> temp->site;
			}
		}
	}
	TempFile.close();
	return np;
}