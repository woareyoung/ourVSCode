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
	///�������2���򽫶����������ݽ�λ�ߵ�λת��
	if (player == 2)
	{
		for (i = 1; i < 10; i++) situa.Line[i] = DigitalChange(StatusQuo.Line[i]);
	}
	else for (i = 1; i < 10; i++) situa.Line[i] = StatusQuo.Line[i];
	while (!TempFile.eof())//ѭ��ֱ���ļ�ĩβ
	{
		for (i = 0; i < 9; i++)
		{
			TempFile >> value[i];
			if (situa.Line[i + 1] != value[i])//ֻҪ��һ�����ݲ���ͬ����ƥ����һ����¼
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