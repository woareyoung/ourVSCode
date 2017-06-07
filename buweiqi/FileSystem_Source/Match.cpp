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
	///若是玩家2，则将读出来的数据进位高低位转换
	if (player == 2)
	{
		for (i = 1; i < 10; i++) situa.Line[i] = DigitalChange(StatusQuo.Line[i]);
	}
	else for (i = 1; i < 10; i++) situa.Line[i] = StatusQuo.Line[i];
	while (!TempFile.eof())//循环直到文件末尾
	{
		for (i = 0; i < 9; i++)
		{
			TempFile >> value[i];
			if (situa.Line[i + 1] != value[i])//只要有一个数据不相同，则匹配下一条记录
			{
				for (; i < 10; i++) TempFile >> value[i];
				break;
			}
			else if (i == 8)
			{
				if (np == NULL)
				{
					temp = new NEXTPACE;
					TempFile >> temp->site;
				}
				else
				{
					temp->next = new NEXTPACE;
					temp = temp->next;
					TempFile >> temp->site;
				}
			}
		}
	}
	TempFile.close();
	return np;
}