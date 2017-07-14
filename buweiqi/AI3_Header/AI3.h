#pragma once
#ifndef AI3_H_INCLUDED
#define AI3_H_INCLUDED

#include "../ChessBoard_Header/WinCheck.h"
#include "../AI2_Header/AI2.h"
#include "../ChessBoard_Header/showUnicodeInfo.h"
#include <tuple>
#include <set>
#include <iostream>
#include <cstdlib>
#include "MCTS.h"

class SimulatorGo : public AI2 {
private:
	WinCheck::ChessInfo chessInfo;
	mutable int chessScoreW[ChessEnd][ChessEnd];
protected:
	virtual int* getPatternType() override {
		return Type[player_to_move - 1];
	}
public:
	bool ifShowInfo;

	mutable int Winner;
	int player_to_move;
	mutable int depth;
	SimulatorGo() :
		depth(0),
		Winner(NoChess),
		ifShowInfo(false)
	{
		initAllArray();
		for (int i = ChessInit; i < ChessEnd; ++i) {
			for (int j = ChessInit; j < ChessEnd; ++j) {
				chessScoreW[i][j] = this->getDefaultChessScore(i, j);
				chessScore[i][j] = chessScoreW[i][j];
			}
		}
	}

	SimulatorGo(
		int b[ChessEnd][ChessEnd],
		int Id) :
		player_to_move(Id),
		Winner(NoChess),
		depth(0),
		ifShowInfo(false)
	{
		initAllArray();
		for (int i = ChessInit; i < ChessEnd; ++i) {
			for (int j = ChessInit; j < ChessEnd; ++j) {
				cross[i][j] = b[i][j];
				chessScoreW[i][j] = this->getDefaultChessScore(i, j);
				chessScore[i][j] = chessScoreW[i][j];
			}
		}
	}

	void initSimulation() const {
		for (int i = ChessInit; i < ChessEnd; ++i) {
			for (int j = ChessInit; j < ChessEnd; ++j) {
				if (cross[i][j] == NoChess && CS[i][j] == minLimit) continue;
				if (cross[i][j] == NoChess && CS[i][j] == 0) continue;
				CS[i][j] = const_cast<SimulatorGo*>(this)->getDefaultChessScore(i, j);
			}
		}
	}

	// 随机走步
	template<typename RandomEngine>
	void do_random_move(RandomEngine* engine, std::vector<int> moves)
	{
		if (moves.empty()) {// 如果着子点集合为空的话，就直接返回
			Winner = getRival(player_to_move);
			return;
		}
		std::uniform_int_distribution<std::size_t> move_ind(0, moves.size() - 1);
		auto move = moves[move_ind(*engine)];
		// 开始走步
		do_move(move);
	}

	// 模拟着子
	virtual void do_move(int move)
	{
		// 结点的深度加1
		depth++;
		// 对方的结点
		int rival = getRival(player_to_move);
		// 这里表示模拟没有结束，这时我们需要判断是否已经输赢论定了
		int line, column;
		line = getLine(move);
		column = getColumn(move);
		// 如果没有输赢未论定的话，就继续模拟
		cross[line][column] = player_to_move;


		if (ifShowInfo) {
			_cprintf("\n**************This is chess score*******(%d, %d)***%s********\n",
				line, column, player_to_move == Black ? "Black" : "White");
			for (int i = 1; i < 10; ++i)
			{
				for (int j = 1; j < 10; ++j)
					if (CS[i][j] >= 20) {
						_cprintf("%d\t", CS[i][j]);
					}
					else if (CS[i][j] == minLimit) {
						_cprintf("M\t");
					}
					else if (CS[i][j] == 0) {
						_cprintf("0\t");
					}
					else {
						_cprintf(" \t");
					}
					_cprintf("\n");
			}
			_cprintf("**************This is chess cross*********(%d, %d)***%s******\n",
				line, column, player_to_move == Black ? "Black" : "White");
			showChessBoard(cross);
			// system("pause");
		}

		// 轮到下一个玩家着子
		player_to_move = rival;
	}

	// 是否还有可着子的着子点
	template<typename RandomEngine>
	bool do_random_move(RandomEngine* engine)
	{
		auto moves = get_moves();
		if (!moves.empty()) {
			do_random_move(engine, moves);
			return true;
		}
		return false;
	}

	virtual void initCSPoint() override {
		if (player_to_move == White) {
			CS = chessScoreW;
		}
		else {
			CS = chessScore;// 这里目测不会用到的
		}
	}

	// 从棋盘中搜集所有可行的着子点
	virtual std::vector<int> get_moves() const
	{
		// 下面是搜集所有可能的着子点。
		std::vector<int> moves;

		// 如果深度大于888层的话就，直接诶返回moves了。
		if (depth > 888) {
			return moves;
		}

		if (Winner == Black && Winner == White) {
			if (ifShowInfo) {
				system("pause");
				_cprintf("Winner is %s", Winner == Black ? "Black" : "White");
			}
			return moves;
		}
		if (ifShowInfo) {
			// 调用Pattern对当前局面进行处理，将所有可能的着子点加入到moves数组
			_cprintf("Player is -> %s \n", player_to_move == Black ? "Black" : "White");
		}

		const_cast<SimulatorGo*>(this)->initCSPoint();
		initSimulation();
		const_cast<SimulatorGo*>(this)->Revalute();

		// 从当前棋盘中选择出由Pattern匹配出来的比较好的着子点集合
		for (int i = ChessStart; i < ChessEnd; ++i) {
			for (int j = ChessStart; j < ChessEnd; ++j) {
				if (cross[i][j] == NoChess && CS[i][j] >= 10) {
					moves.emplace_back(getMove(i, j));
				}
			}
		}
		// 如果当前Pattern匹配完之后，没有一个是好的着子点
		if (moves.size() < 5) {
			for (int i = ChessStart; i < ChessEnd; ++i) {
				for (int j = ChessStart; j < ChessEnd; ++j) {
					if (cross[i][j] == NoChess && CS[i][j] != minLimit) {
						if (const_cast<SimulatorGo*>(this)->isGo2Dead(i, j, player_to_move)) {
							CS[i][j] = minLimit;
							continue;
						}
						moves.emplace_back(getMove(i, j));
					}
				}
			}
		}

		// 如果执行完上述的之后没有一个着子点可以下的话，游戏结束。
		if (ifShowInfo && moves.empty()) {
			_cprintf("moves is null.\n");
		}
		return moves;
	}

	// 用于判断输赢结果
	double get_result(int current_player_to_move) const
	{
		double Score = Winner == current_player_to_move ? 0.0 : 1.0;
		return Score;
	}

	virtual bool checkEmptyPos(const int& x, const int& y, const int& start, const int& mainColor, const Pos *emptyPos) override {
		/******************************************
		判断当前匹配到的空位是否是敌方的自杀点，
		如果是的话，就把该点的分数设置为0，跳过匹配模式
		*******************************************/
		int rival = getRival(player_to_move);
		for (int i = 0; i < start; ++i) {
			if (mainColor == rival) {
				// 临时设置当前获得的位置为敌方着子点，判断是否是敌方的自杀点
				if (isGo2Dead(emptyPos[i].line, emptyPos[i].column, rival)) {
					// 如果是敌方的自杀点的话，这里就置零   -.-！！！
					CS[emptyPos[i].line][emptyPos[i].column] = 0;
					return false;
				}
			}
			else if (mainColor == player_to_move) {
				// 临时设置当前获得的位置为我方着子点，判断是否是我方的自杀点
				if (isGo2Dead(x, y, player_to_move)) {
					CS[x][y] = minLimit;
					// 如果是我方的自杀点的话，就直接跳转，不用判断是否是敌方的自杀点了。
					return false;
				}
			}
			// 这里既不是我方自杀点，也不是敌方自杀点
			cross[emptyPos[i].line][emptyPos[i].column] = NoChess;
		}
		return true;
	}
	// 检查棋子是否有效，并对分析的结果进行相应的加分
	virtual bool checkStone(const int& x, const int& y) override {
		// 对于当前匹配到的着子点的环境进行分析
		// 临时设置当前获得的位置为我方着子点，判断是否是我方的自杀点
		int rival = getRival(player_to_move);
		if (isGo2Dead(x, y, player_to_move)) {
			CS[x][y] = minLimit;
			// 如果是我方的自杀点的话，就直接跳转，不用判断是否是敌方的自杀点了。
			return false;
		}
		// 临时设置当前获得的位置为敌方着子点，判断是否是敌方的自杀点
		if (cross[x][y] == NoChess && CS[x][y] == 0) return false;
		if (isGo2Dead(x, y, rival)) {
			// 如果是敌方的自杀点的话，这里就置零   -.-！！！
			CS[x][y] = 0;
			return false;
		}
		// 这里既不是我方自杀点，也不是敌方自杀点
		return true;
	}
};

class AI3 : public AI2
{
private:

public:

	virtual int maxandmin(const int& depth) override {
		return predict();
	}
	int predict() {
		MCTS::ComputeOptions options;
		options.number_of_threads = 8;
		// options.verbose = true;
		// options.max_time = 1;
		auto state_copy = new SimulatorGo(cross, PlayerId);
		auto best_move = MCTS::compute_move(state_copy, options);
		return best_move;
	}
};

#endif // AI3_H_INCLUDED
