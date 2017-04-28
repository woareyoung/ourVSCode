#include "../../stdafx.h"
#include"../../ChessBoard_Header/ChessBoard.h"
/**
* [ChessBoard::Int2String ����ת��Ϊ�ַ���]
* @param  num []
* @return     []
*/
std::string ChessBoard::Int2String(int num)
{
	String.clear();
	int mid;
	if (num > 100 || num == 100)
	{
		mid = num / 100;
		addChar(mid);
		mid = (num % 100) / 10;
		addChar(mid);
	}
	else if (num < 100)
	{
		mid = num / 10;
		addChar(mid);
	}
	mid = num % 10;
	addChar(mid);
	return String;
}
/**
* [ChessBoard::addChar ����ת��Ϊ�ַ���ʱ�ĸ�������]
* @param mid [description]
*/
void ChessBoard::addChar(int mid)
{
	switch (mid)
	{
	case 9:String += "9"; break;
	case 8:String += "8"; break;
	case 7:String += "7"; break;
	case 6:String += "6"; break;
	case 5:String += "5"; break;
	case 4:String += "4"; break;
	case 3:String += "3"; break;
	case 2:String += "2"; break;
	case 1:String += "1"; break;
	case 0:String += "0"; break;
	}
}