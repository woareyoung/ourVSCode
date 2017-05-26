#include "../FileSystem_Header/FileSystem.h"
///ƥ�䵱ǰ����״���ļ���
void FileSystem::Match(SITUATION &StatusQuo, int player)
{
	int i;
	SITUATION situa;
	TempFile.close();
	remove(TempFileName.c_str());
	OpenFile(TempFileName, TempFile);
	if (NowTempHouse == &TempHouse1)
	{
		TempHouse2.close();
		remove(TempHouse2Name.c_str());
		OpenFile(TempHouse2Name, TempHouse2);
	}
	else
	{
		TempHouse1.close();
		remove(TempHouse1Name.c_str());
		OpenFile(TempHouse1Name, TempHouse1);
	}
	situa.Line[0] = StatusQuo.Line[0];
	if (player == 2)
	{
		for (i = 1; i < 10; i++) situa.Line[i] = DigitalChange(StatusQuo.Line[i]);
	}
	else for (i = 1; i < 10; i++) situa.Line[i] = StatusQuo.Line[i];
	while (!(*NowTempHouse).eof())//ѭ��ֱ���ļ�ĩβ
	{
		for (i = 0; i < 9; i++)
		{
			(*NowTempHouse) >> value[i];
			if (situa.Line[i + 1] != value[i])
			{
				for (; i < 10; i++) (*NowTempHouse) >> value[i];
				if (NowTempHouse == &TempHouse1) AddFile(TempHouse2, TempHouse2Name);
				else AddFile(TempHouse1, TempHouse1Name);
				break;
			}
			else if (i == 9) AddFile(TempFile, TempFileName);
		}
	}
	if (NowTempHouse == &TempHouse1) NowTempHouse = &TempHouse2;
	else NowTempHouse = &TempHouse1;
}
///�����ϵ�ǰ����״���ļ���ת�Ƶ���ʱ����������ļ�
///��δ���ֹ�������ת�Ƶ���ʱԴ���ļ�
void FileSystem::AddFile(std::fstream &fst, std::string filename)
{
	fst.seekg(0, std::ios::end);//���ļ�ָ���ƶ����ļ�ĩβ
	for (int i = 0; i < 10; i++)
	{
		if (i == 5) fst << std::endl;
		fst << value[i] << " ";
	}
	fst << std::endl;
}