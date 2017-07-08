#pragma once
#ifndef AI3_H_INCLUDED
#define AI3_H_INCLUDED

#include <cstdlib>
#include "../ChessBoard_Header/WinCheck.h"
#include "../AI2_Header/AI2.h"
#include <tuple>
#include <set>
#include <iostream>
#include <cstdlib>
#include "MCTS.h"

class SimulatorGo : public AI2 {
private:
	WinCheck::ChessInfo chessInfo;
	virtual int* getPatternType() override {
		return Type[player_to_move - 1];
	}
public:
	mutable int Winner;
	int player_to_move;
	mutable int depth;
	SimulatorGo() :
		depth(0),
		Winner(NoChess)
	{
		initAllArray();
	}

	SimulatorGo(
		int b[ChessEnd][ChessEnd],
		int C[ChessEnd][ChessEnd],
		int Id) :
		player_to_move(Id),
		Winner(NoChess),
		depth(0)
	{
		initAllArray();
		for (int i = ChessInit; i < ChessEnd; ++i) {
			for (int j = ChessInit; j < ChessEnd; ++j) {
				cross[i][j] = b[i][j];
				chessScore[i][j] = C[i][j];
			}
		}
	}

	void initSimulation() const {
		for (int i = ChessInit; i < ChessEnd; ++i) {
			for (int j = ChessInit; j < ChessEnd; ++j) {
				if (cross[i][j] == NoChess && chessScore[i][j] == minLimit) continue;
				if (cross[i][j] == NoChess && chessScore[i][j] == 0) continue;
				chessScore[i][j] = const_cast<SimulatorGo*>(this)->getDefaultChessScore(i, j);
			}
		}
	}

	// 随机走步
	template<typename RandomEngine>
	void do_random_move(RandomEngine* engine)
	{
		auto moves = get_moves();// 获取可以着子的着子点集合
		if (moves.empty()) {// 如果着子点集合为空的话，就直接返回
			Winner = getRival(player_to_move);
			return;
		}
		std::uniform_int_distribution<std::size_t> move_ind(0, moves.size() - 1);
		auto move = moves[move_ind(*engine)];
		// 开始走步
		do_move(move);
		//_cprintf("\n**************This is chess score******************\n");
		//for (int i = 1; i < 10; ++i)
		//{
		//	for (int j = 1; j < 10; ++j)
		//		_cprintf("%d\t", chessScore[i][j]);
		//	_cprintf("\n");
		//}
		//_cprintf("**************This is chess cross******************\n");
		//for (int i = 1; i < 10; ++i)
		//{
		//	for (int j = 1; j < 10; ++j)
		//		_cprintf("%d\t", cross[i][j]);
		//	_cprintf("\n");
		//}
	}

	// 模拟着子
	virtual void do_move(int move)
	{
		// 结点的深度加1
		depth++;
		// 对方的结点
		int rival = getRival(player_to_move);
		if (move == no_move) {
			Winner = rival;
			return;
		}
		// 这里表示模拟没有结束，这时我们需要判断是否已经输赢论定了
		int line, column;
		line = getLine(move);
		column = getColumn(move);

		chessInfo.WinOrLose(line, column, player_to_move, Winner, cross);
		if (Winner == Black || Winner == White) {
			return;
		}

		// 如果没有输赢未论定的话，就继续模拟
		cross[line][column] = player_to_move;

		// 轮到下一个玩家着子
		player_to_move = rival;
	}

	// 是否还有可着子的着子点
	virtual bool has_moves() const
	{
		return !get_moves().empty();
	}

	// 从棋盘中搜集所有可行的着子点
	virtual std::vector<int> get_moves() const
	{
		initSimulation();
		// 调用Pattern对当前局面进行处理，将所有可能的着子点加入到moves数组
		const_cast<SimulatorGo*>(this)->Revalute();
		// 下面是搜集所有可能的着子点。
		std::vector<int> moves;
		// 如果深度大于1000层的话就，直接诶返回moves了。
		if (depth > 1000) {
			return moves;
		}
		// 从当前棋盘中选择出由Pattern匹配出来的比较好的着子点集合
		for (int i = ChessStart; i < ChessEnd; ++i) {
			for (int j = ChessStart; j < ChessEnd; ++j) {
				if (cross[i][j] == NoChess && chessScore[i][j] >= 20) {
					moves.emplace_back(getMove(i, j));
				}
			}
		}
		// 如果当前Pattern匹配完之后，没有一个是好的着子点
		if (moves.empty()) {
			for (int i = ChessStart; i < ChessEnd; ++i) {
				for (int j = ChessStart; j < ChessEnd; ++j) {
					if (cross[i][j] == NoChess && chessScore[i][j] != minLimit) {
						moves.emplace_back(getMove(i, j));
					}
				}
			}
		}
		// 如果执行完上述的之后没有一个着子点可以下的话，游戏结束。
		/*_cprintf("\n moves` vector is : \n");
		for (auto move : moves) {
		_cprintf("%d.", move);
		}*/
		return moves;
	}

	// 用于判断输赢结果
	double get_result(int current_player_to_move) const
	{
		double Score = Winner == current_player_to_move ? 0.0 : 1.0;
		Winner = NoChess;
		return Score;
	}
};

class AI3 : public AI2
{
private:

public:

	virtual int maxandmin(int depth) override {
		Revalute();
		return predict();
	}
	int predict() {
		MCTS::ComputeOptions options;
		options.number_of_threads = 1;
		options.verbose = true;
		// options.max_time = 1;
		auto state_copy = new SimulatorGo(cross, chessScore, PlayerId);
		auto best_move = MCTS::compute_move(state_copy, options);
		return best_move;
	}
};



#endif // AI3_H_INCLUDED
