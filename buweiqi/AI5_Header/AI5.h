#pragma once
#include "../ChessBoard_Header/parameter.h"
#include "../AI3_Header/MCTS.h"
#include "../AI4_Header/AI4.h"
#include <vector>
#include <iostream>


class SimulatorGo2 : public AIPlayer {
private:
	int cross[ChessEnd][ChessEnd];
public:
	mutable int Winner;
	int player_to_move;

	SimulatorGo2() :
		Winner(NoChess)
	{
		for (int i = ChessInit; i < ChessEnd; ++i) {
			for (int j = ChessInit; j < ChessEnd; ++j) {
				cross[i][j] = NoChess;
			}
		}
	}

	SimulatorGo2(
		int b[ChessEnd][ChessEnd],
		int Id) :
		player_to_move(Id),
		Winner(NoChess)
	{
		this->turn2Who = Id;
		this->PlayerId = turn2Who;
		for (int i = ChessInit; i < ChessEnd; ++i) {
			for (int j = ChessInit; j < ChessEnd; ++j) {
				cross[i][j] = b[i][j];
			}
		}
	}

	// 是否还有可着子的着子点
	template<typename RandomEngine>
	bool doRandomMove(RandomEngine* engine)
	{
		AI4 state;
		std::vector<int> moves;
		bool Win;
		if (state.getMoves(moves, cross, player_to_move, Win)) {
			if (moves.empty()) {
				Winner = getRival(player_to_move);
				return false;
			}
			std::uniform_int_distribution<std::size_t> move_ind(0, moves.size() - 1);
			// 开始模拟走步
			SimulateMove(moves[move_ind(*engine)]);
			return true;
		}
		else {
			if (Win) {
				Winner = getRival(player_to_move);
				return false;
			}
			else {
				SimulateMove(*moves.begin());
				return true;
			}
		}
		return false;
	}

	// 从棋盘中搜集所有可行的着子点
	virtual std::vector<int> getMoves() {
		// 下面是搜集所有可能的着子点。
		std::vector<int> moves;

		if (Winner == Black || Winner == White) {
			return moves;
		}

		AI4 state;
		bool Win;
		// 如果是true，那就直接返回
		if (state.getMoves(moves, cross, player_to_move, Win)) {
			return moves;
		}
		else {
			if (Win) {
				// 如果Win为true的话，就是表示输了，就直接清空数据
				moves.clear();
				moves.swap(std::vector<int>());
				return moves;
			}
			else {
				// 如果Win为false的话，就是表示moves有且仅有一个子，而且这一步是确定的，也直接清空数据。
				return moves;
			}
		}
	}

	// 用于判断输赢结果
	virtual double getResult(const int& current_player_to_move)
	{
		// 没有模拟到尽头
		if (Winner != White && Black != Black) {
			return 0.5;
		}
		return Winner == current_player_to_move ? 1.0 : 0.0;
	}

	// 模拟着子，主要的作用是用于模拟下棋
	virtual void SimulateMove(const int& move)
	{
		// 这里表示模拟没有结束，这时我们需要判断是否已经输赢论定了
		// 如果没有输赢未论定的话，就继续模拟
		cross[getLine(move)][getColumn(move)] = player_to_move;
		// showSimaluteInfo(getLine(move), getColumn(move));
		// 轮到下一个玩家着子
		player_to_move = getRival(player_to_move);
	}

	void showSimaluteInfo(const int& line, const int& column) {
		_cprintf("**************This is chess cross*********(%d, %d)***%s******\n",
			line, column, player_to_move == Black ? "Black" : "White");
		showChessBoard(cross);
		system("pause");

	}
};


class AI5 : public AI, public AIPlayer {
private:
public:
	AI5() :
		chessCount(0)
	{
		for (int i = ChessStart; i < ChessEnd; ++i) {
			for (int j = ChessStart; j < ChessEnd; ++j) {
				cross[i][j] = NoChess;
			}
		}
	}
	void GetPosition(int &line, int &column, int onTurn) {
		//接收到界面的远程命令
		if (onTurn != 1 && onTurn != -1 && onTurn != 2 && onTurn != -2)
		{
			//用于响应主窗口对AI的检查
			if (onTurn == 0)
			{
				// 这里是重新开始游戏的数据重置过程
				line++;
				column++;
				for (int i = ChessStart; i < ChessEnd; ++i) {
					for (int j = ChessStart; j < ChessEnd; ++j) {
						cross[i][j] = NoChess;
					}
				}
			}
			//回退一步的命令，对数据进行回退。（100为玩家1，200为玩家2，line与column为回退的位置）
			else if (onTurn == 100 || onTurn == 200)
			{
				rollback(line, column, onTurn / 100);
			}
			return;
		}
		/******************************************
		添加敌方着子点到我方棋盘上
		*******************************************/
		this->chessCount++;
		// 注意传递进来的onTurn参数是对方的，而不是己方的。
		initPlayerRole(onTurn);
		cross[line][column] = Rival;

		/******************************************
		我方着子
		*******************************************/
		int temp = predict();
		line = getLine(temp);
		column = getColumn(temp);
		if (!OnChessBoard(line, column)) {
			line = 0;
			column = 0;
		}
		cross[line][column] = turn2Who;

		_cprintf("**************This is chess cross*******(%d, %d)***********\n", line, column);
		showChessBoard(cross);
	}
protected:
	int cross[10][10];
	int chessCount;

	void rollback(int line, int column, int onTurn) {
		if (onTurn == turn2Who) {
			--chessCount;
		}
		if (cross[line][column] != NoChess) {
			cross[line][column] = NoChess;
		}
	}

	void initPlayerRole(int onTurn) {
		Rival = (onTurn == Black || onTurn == isAI1onTurn) ? Black : White;
		this->turn2Who = getRival(Rival);
		this->PlayerId = turn2Who;
	}

	int predict() {
		AI4 state;
		std::vector<int> moves;
		bool Win;
		if (!state.getMoves(moves, cross, PlayerId, Win)) {
			return *moves.begin();
		}
		//else {
		//	std::mt19937_64 random_engine(1008611);// 随机函数种子，用于随机走步
		//	std::uniform_int_distribution<std::size_t> move_ind(0, moves.size() - 1);
		//	// 开始模拟走步
		//	return moves[move_ind(random_engine)];
		//}
		MCTS::ComputeOptions options;
		options.number_of_threads = 2;
		options.verbose = true;
		options.max_iterations = -1;
		options.max_time = 1;
		auto state_copy = new SimulatorGo2(cross, PlayerId);
		auto best_move = MCTS::computeNextMove(state_copy, options);
		return best_move;
	}
};

