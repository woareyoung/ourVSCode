#include "../AI1_Header/AI1.h"

int AI1::MatchMemory()
{
	double NextPace;
	FileSystem FileIO;
	GetChessBoardStatus(FileIO);
	FileIO.MatchFile.seekg(0, std::ios::end);
	NextPace = FileIO.MatchFile.tellg();
	if (NextPace == 0) return 0;
	FileIO.MatchFile.seekg(0);
	for (int i = 0; i < 10; i++)
	{
		FileIO.MatchFile >> NextPace;
	}
	return NextPace;
}

void AI1::GetChessBoardStatus(FileSystem &FileIO)
{
	SITUATION NowStatus;
	int i, j;
	//获取当前棋盘状况
	for (i = 1; i < 10; i++)
	{
		for (j = 1; j < 10; j++)
		{
			if (cross[i][j] == PlayerNumber) NowStatus.Line[i] += pow(2, j);
			else if (cross[i][j] == 3 - PlayerNumber) NowStatus.Line[i] += pow(2, j) / 10000;
		}
	}
	FileIO.Match(NowStatus, PlayerNumber);
	FileIO.MatchFile.close();
	FileIO.OpenFile("TempFile.txt", FileIO.MatchFile);
}