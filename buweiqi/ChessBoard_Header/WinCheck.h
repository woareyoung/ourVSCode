#ifndef WinCheck_H
#define WinCheck_H

namespace WinCheck {
	struct ChessBoardOption// 计算选择
	{
		int emptyChess;// 空位
		int white;// 白子
		int black;// 黑子
		int edge;// 边界
		bool isInterface;

		ChessBoardOption() :
			emptyChess(noChess),// 默认的线程数量是8条
			white(isWhite),// 默认的最大的迭代数量是10000
			black(isBlack), // 默认是没有时间限制的
			edge(8),
			isInterface(false)
		{ }
	};
}


#include <conio.h>
#include <iostream>
#include <tuple>
#include <stack>

namespace WinCheck {
	class ChessInfo {
	private:
		mutable bool WinCheckPosition[4];
		mutable bool WinCheckCross[10][10];
		mutable int WinCheckcross[10][10];

		ChessBoardOption option;
	protected:
		//判断对方的棋子是否被围死，两个参数表示对方棋子的位置
		//该函数用于一堆棋子被围死的情况，用递归的方法检查一堆棋子的情况
		//若棋子被围死，则返回true，只要棋子周围有一个空位就返回false
		bool Besieged(int RivalLine, int RivalColumn, int player, int rival) const
		{
			bool tie[4] = { false, false, false, false };
			auto setStatus = [&](int RivalLine, int RivalColumn) -> void {
				WinCheckCross[RivalLine][RivalColumn] = true;
				WinCheckCross[RivalLine][0] = true;//下标为0的那一行如果为true则初始化，否则不用初始化
			};
			setStatus(RivalLine, RivalColumn);//设置位置访问状态
											  //检查对方棋子的上方
			auto CheBesie = [&](int RivalLine, int RivalColumn, int player, int rival, bool &tie)
			{
				//若对方棋子的某方向有对方的棋子且未到边缘
				if (WinCheckcross[RivalLine][RivalColumn] == rival && RivalLine > 0 && RivalColumn > 0
					&& WinCheckCross[RivalLine][RivalColumn] == false && RivalLine < 10 && RivalColumn < 10)
					tie = Besieged(RivalLine, RivalColumn, player, rival);
				//若对方棋子的某方向没有棋子，直接返回false
				else if (WinCheckcross[RivalLine][RivalColumn] == 0 && RivalLine > 0 && RivalColumn > 0 && RivalLine < 10 && RivalColumn < 10)
					return false;
				//若对方棋子的某方向有己方的棋子或已到边缘
				else tie = true;
				return true;
			};
			if (!CheBesie(RivalLine - 1, RivalColumn, player, rival, tie[0])) return false;
			//检查对方棋子的下方
			if (!CheBesie(RivalLine + 1, RivalColumn, player, rival, tie[1])) return false;
			//检查对方棋子的右方
			if (!CheBesie(RivalLine, RivalColumn + 1, player, rival, tie[2])) return false;
			//检查对方棋子的左方
			if (!CheBesie(RivalLine, RivalColumn - 1, player, rival, tie[3])) return false;
			if (tie[0] && tie[1] && tie[2] && tie[3]) return true;
			return false;
		}
		///参数：OriLine  下棋位置的行
		///参数：OriColumn  下棋位置的列
		///参数：CLine  需要检查的位置的行
		///参数：CColumn   需要检查的位置的列
		///参数：Player  哪个玩家下的棋子
		///参数：i  下标（WinCheckPosition）
		bool Check(int OriLine, int OriColumn, int CLine, int CColumn, int player, int i, int& Winner) const
		{
			int rival = player == 1 ? 2 : 1;
			auto reduceRecursionTimes = [&]() -> void {
				for (int i = 0; i < 10; ++i)
				{
					if (WinCheckCross[i][0] == true)
						for (int j = 0; j < 10; ++j)
							WinCheckCross[i][j] = false;
				}
			};

			auto showChessBroadInfoOnDOS = [&]()
			{
				int countB = 0;
				int countW = 0;
				_cprintf("------------------Chess Broad---------------------\n");
				for (int i = 1; i < 10; ++i)
				{
					for (int j = 1; j < 10; ++j)
					{
						_cprintf("%d\t", WinCheckcross[i][j]);
						if (WinCheckcross[i][j] == option.white)
						{
							countW++;
						}
						else if (WinCheckcross[i][j] == option.black)
						{
							countB++;
						}
					}
					_cprintf("\n");
				}
				_cprintf("----Black:%d,White:%d----\n", countB, countW);
			};
			//若己方棋子已到边缘
			if (CColumn == 0 || CColumn == 10 || CLine == 0 || CLine == 10) WinCheckPosition[i] = true;
			//若有对方的一个棋子
			else if (WinCheckcross[CLine][CColumn] == rival)
			{
				reduceRecursionTimes();
				if (Besieged(CLine, CColumn, player, rival))
				{
					Winner = rival;
					// showChessBroadInfoOnDOS();
					return true;
				}
				WinCheckPosition[i] = true;
			}
			//若有己方的棋子
			else if (WinCheckcross[CLine][CColumn] == player)
			{
				reduceRecursionTimes();
				WinCheckPosition[i] = Besieged(OriLine, OriColumn, rival, player);
			}
			return false;
		}
	public:
		ChessInfo() :
			option(ChessBoardOption())
		{}
		ChessInfo(const ChessBoardOption chessOption) :
			option(chessOption)
		{}
		/*胜负条件
		* 1、超时。该条件判断由窗口过程函数实现
		* 2、空手。先忽略
		* 3、围死。基本思想：假设黑子下一个棋子，则判断该黑子周围的四个位置上有没有白子被围死，还有该黑子是否被围死
		*/
		//注：调用该函数（即下棋的玩家）的那个人或AI是“己方”
		bool WinOrLose(int& line, int& column, int& onTurn, int& Winner, int c[10][10]) const
		{
			auto initArray = [&](int c[10][10]) {
				for (int i = 0; i < 10; ++i) {
					for (int j = 0; j < 10; ++j) {
						WinCheckcross[i][j] = c[i][j];
						WinCheckCross[i][j] = false;
					}
				}
			};
			initArray(c);

			int i;
			for (i = 0; i < 4; ++i) WinCheckPosition[i] = false;
			int player;//记录己方的编号（是1还是2）
			int rival;//记录对方的编号（是1还是2）
					  //onTurn的值是先变化了再调用该函数（WinOrLose）的
			if (onTurn == isPlay1onTurn || onTurn == isAI1onTurn)
			{
				player = isWhite;
				rival = isBlack;
			}
			else if (onTurn == isPlay2onTurn || onTurn == isAI2onTurn)
			{
				player = isBlack;
				rival = isWhite;
			}
			else {
				return false;
			}

			//判断棋子四周有没有棋子
			//检查左边
			if (Check(line, column, line, column - 1, player, 0, Winner))
			{
				Winner = rival;
				return true;
			}
			//检查右边
			if (Check(line, column, line, column + 1, player, 1, Winner))
			{
				Winner = rival;
				return true;
			}
			//检查上边
			if (Check(line, column, line - 1, column, player, 2, Winner))
			{
				Winner = rival;
				return true;
			}
			//检查下边
			if (Check(line, column, line + 1, column, player, 3, Winner))
			{
				Winner = rival;
				return true;
			}
			if (WinCheckPosition[0] && WinCheckPosition[1] && WinCheckPosition[2] && WinCheckPosition[3])
			{
				Winner = rival;
				return true;
			}
			return false;
		}

		bool WinCheck(int& line, int& column, int& type, int CROSS[][10]) {
			int Winner;
			return WinCheck(line, column, type, Winner, CROSS);
		}

		bool WinCheck(const int& line, const int& column, const int& type, int& Winner, int CROSS[][10])
		{
			auto initArray = [&](int c[10][10]) {
				for (int i = 0; i < 10; ++i) {
					for (int j = 0; j < 10; ++j) {
						WinCheckcross[i][j] = CROSS[i][j];
						WinCheckCross[i][j] = false;
					}
				}
			};
			initArray(CROSS);

			int player;//记录己方的编号（是1还是2）
			int rival;//记录对方的编号（是1还是2）
					  //onTurn的值是先变化了再调用该函数（WinOrLose）的
			if (option.isInterface) {

				if (type == isPlay1onTurn || type == isAI1onTurn)
				{
					player = option.white;
					rival = option.black;
				}
				else if (type == isPlay2onTurn || type == isAI2onTurn)
				{
					player = option.black;
					rival = option.white;
				}
				else {
					return false;
				}
			}
			else {
				player = type;
				rival = getRival(type);//对方
			}

			bool simulate = false;//标记是否是模拟下棋

			int tempLine = line, tempColumn = column;

			std::stack<std::pair<int, int>> position;
			//弹出数据
			auto POPdata = [&]() {
				if (position.empty()) return false;
				std::tie(tempLine, tempColumn) = position.top();
				position.pop();
				return true;
			};
			//压栈，并设置遍历
			auto PUSHdata = [&](int l, int c) {
				position.push(std::make_pair(tempLine, tempColumn));
				tempLine = l;
				tempColumn = c;
			};
			//还原数据
			auto BackData = [&]() {
				tempLine = line;
				tempColumn = column;
				while (!position.empty()) position.pop();
			};
			//判断是否是模拟下棋
			if (WinCheckcross[tempLine][tempColumn] == option.emptyChess)
			{
				WinCheckcross[tempLine][tempColumn] = player;
				simulate = true;
			}

			auto resetWinCheckCross = [&]() -> void {
				for (register int i = ChessInit; i < ChessEnd; ++i) {
					for (register int j = ChessInit; j < ChessEnd; ++j) {
						WinCheckCross[i][j] = false;
					}
				}
			};
			//跟踪遍历（判断有没有被围死）
			//param[Player]:检查 谁 的棋子被围死
			auto VisitAll = [&](const int& Player)
			{
				resetWinCheckCross();
				while (true)
				{
					WinCheckCross[tempLine][tempColumn] = true;
					//遇到空位，活下去了
					if (WinCheckcross[tempLine][tempColumn] == option.emptyChess && OnChessBoard(tempLine, tempColumn))
					{
						BackData();
						return false;
					}
					//如果上面有自己的棋子且未遍历过
					if (WinCheckcross[tempLine - 1][tempColumn] == Player && !WinCheckCross[tempLine - 1][tempColumn] && tempLine > 1)
					{
						PUSHdata(tempLine - 1, tempColumn);
						continue;
					}
					else if (WinCheckcross[tempLine - 1][tempColumn] == option.emptyChess && tempLine > 1)
					{
						--tempLine;
						continue;
					}
					//如果下面有自己的棋子且未遍历过
					if (WinCheckcross[tempLine + 1][tempColumn] == Player && !WinCheckCross[tempLine + 1][tempColumn] && tempLine < 9)
					{
						PUSHdata(tempLine + 1, tempColumn);
						continue;
					}
					else if (WinCheckcross[tempLine + 1][tempColumn] == option.emptyChess && tempLine < 9)
					{
						++tempLine;
						continue;
					}
					//如果左边有自己的棋子且未遍历过
					if (WinCheckcross[tempLine][tempColumn - 1] == Player && !WinCheckCross[tempLine][tempColumn - 1] && tempColumn > 1)
					{
						PUSHdata(tempLine, tempColumn - 1);
						continue;
					}
					else if (WinCheckcross[tempLine][tempColumn - 1] == option.emptyChess && tempColumn > 1)
					{
						--tempColumn;
						continue;
					}
					//如果右边有自己的棋子且未遍历过
					if (WinCheckcross[tempLine][tempColumn + 1] == Player && !WinCheckCross[tempLine][tempColumn + 1] && tempColumn < 9)
					{
						PUSHdata(tempLine, tempColumn + 1);
						continue;
					}
					else if (WinCheckcross[tempLine][tempColumn + 1] == option.emptyChess && tempColumn < 9)
					{
						++tempColumn;
						continue;
					}
					if (POPdata()) continue;
					BackData();
					return true;//能到这里证明挂了
				}
			};
			if (WinCheckcross[tempLine - 1][tempColumn] == rival && tempLine > 1)
			{
				--tempLine;
				if (VisitAll(rival)) {
					if (simulate) WinCheckcross[line][column] = option.emptyChess;
					Winner = rival;
					return true;
				}
			}
			if (WinCheckcross[tempLine + 1][tempColumn] == rival && tempLine < 9)
			{
				++tempLine;
				if (VisitAll(rival)) {
					if (simulate) WinCheckcross[line][column] = option.emptyChess;
					Winner = rival;
					return true;
				}
			}
			if (WinCheckcross[tempLine][tempColumn - 1] == rival && tempColumn > 1)
			{
				--tempColumn;
				if (VisitAll(rival)) {
					if (simulate) WinCheckcross[line][column] = option.emptyChess;
					Winner = rival;
					return true;
				}
			}
			if (WinCheckcross[tempLine][tempColumn + 1] == rival && tempColumn < 9)
			{
				++tempColumn;
				if (VisitAll(rival)) {
					if (simulate) WinCheckcross[line][column] = option.emptyChess;
					Winner = rival;
					return true;
				}
			}
			if (VisitAll(player)) {
				if (simulate) WinCheckcross[line][column] = option.emptyChess;
				Winner = rival;
				return true;
			}
			if (simulate) WinCheckcross[line][column] = option.emptyChess;
			return false;
		}
	};
};
#endif // WinCheck_H
