#include "../FileSystem_Header/FileSystem.h"
///ƥ�䵱ǰ����״���ļ���
NEXTPACE* FileSystem::Match(SITUATION &StatusQuo, int player, int round)
{
	NEXTPACE *np = NULL;//��¼����һ����Ӧ�Է����������ͷ���
	NEXTPACE *temp = np;
	int i;
 	SITUATION situa;
	OpenFile(FN.ForeName + std::to_string(round) + FN.TXT, TempFile);//�򿪵�ǰ����״���ļ�¼�ļ�
	situa.Line[0] = StatusQuo.Line[0];
	for (i = 1; i < 10; i++) situa.Line[i] = StatusQuo.Line[i];
	TempFile.seekg(0);
	while (!TempFile.eof())//ѭ��ֱ���ļ�ĩβ
	{
		for (i = 1; i < 10; i++)
		{
			TempFile >> value[i];
			if (situa.Line[i] != value[i])//ֻҪ��һ�����ݲ���ͬ����ƥ����һ����¼
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