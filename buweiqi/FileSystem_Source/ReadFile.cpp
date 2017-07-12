#include "../FileSystem_Header/FileSystem.h"

//���ļ�������
void FileSystem::ReadFile(int round)
{
	bool ThreadFinish[OpenFileThreadNumber] = { false };//����߳��Ƿ�ִ�����
	bool MainFinish = false;//��Ǹ��߳��Ƿ�����뿪��
	auto ReadFileToSTL = [&](std::string fileName, int ThreadNum, std::unordered_multimap<std::string, int> &stl) {
		int temp;
		std::string data;
		CountFile[ThreadNum].open(fileName, std::ios::in | std::ios::out);
		if (!CountFile) return;//���ļ�ʧ��
		//���ļ���ȡ��������
		while (!CountFile[ThreadNum].eof())
		{
			CountFile[ThreadNum].seekg(0);
			CountFile[ThreadNum] >> data >> temp;
			stl.insert(std::pair<std::string, int>(data, temp));
		}
		CountFile[ThreadNum].close();
		ThreadFinish[ThreadNum] = true;
	};
	int i;
	if (CountMapUseLack)
	{
		for (i = 0; i < OpenFileThreadNumber; i++)
		{
			std::thread t(ReadFileToSTL, FN.ForeName + std::to_string(round + i) + FN.TXT, i, CCountMap[i]);
			if (i == OpenFileThreadNumber - 1) t.join();//�������һ���̵߳�ʱ�򣬵ȴ���ִ��������뿪
			else t.detach();//ʹ���߳��븸�̷߳���ִ��
		}
	}
	else
	{
		for (i = 0; i < OpenFileThreadNumber; i++)
		{
			std::thread t(ReadFileToSTL, FN.ForeName + std::to_string(round + i) + FN.TXT, i, CountMap[i]);
			if (i == OpenFileThreadNumber - 1) t.join();//�������һ���̵߳�ʱ�򣬵ȴ���ִ��������뿪
			else t.detach();//ʹ���߳��븸�̷߳���ִ��
		}
		CountMapUseLack = true;
	}
	//�ȴ����߳�
	while (!MainFinish)
	{
		for (i = 0; i < OpenFileThreadNumber; i++)
		{
			//ֻҪ�����߳�δִ����ϣ�������ȴ�
			if (!ThreadFinish[i])
			{
				MainFinish = false;
				break;
			}
			else if (i == OpenFileThreadNumber - 1) MainFinish = true;
		}
	}
}