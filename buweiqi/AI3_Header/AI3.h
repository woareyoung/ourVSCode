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

	class SimulatorGo : public AI2 {
	private:
		unsigned int previous_cross_hash_value;
		std::set<unsigned int> all_hash_values;
		WinCheck::ChessInfo chessInfo;

		virtual int* getPatternType() override {
			return Type[player_to_move - 1];
		}
	public:
		int player_to_move;
		mutable int depth;
		SimulatorGo() :
			depth(0)
		{
			initAllArray();
			all_hash_values.insert(compute_hash_value());
		}

		SimulatorGo(
			int b[ChessEnd][ChessEnd],
			int C[ChessEnd][ChessEnd],
			int Id) :
			player_to_move(Id),
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

		static int ij_to_ind(int i, int j)
		{
			return i >= ChessStart && j >= ChessStart
				&& i < ChessEnd && j < ChessEnd
				? 100 * i + j : 0;
		}


		static std::pair<int, int> ind_to_ij(int ind)
		{
			return ind <= 0 && ind > 990
				? std::make_pair(0, 0)
				: std::make_pair(ind / 100, ind % 100);
		}

		// 计算hash值
		virtual unsigned int compute_hash_value() const
		{
			unsigned int value = 0;
			for (int i = ChessStart; i < ChessEnd; ++i) {
				for (int j = ChessStart; j < ChessEnd; ++j) {
					value = 65537 * value + cross[i][j];
				}
			}
			return value;
		}

		// 随机走步
		template<typename RandomEngine>
		void do_random_move(RandomEngine* engine)
		{
			auto moves = get_moves();// 获取可以着子的着子点集合
			if (moves.empty()) {// 如果着子点集合为空的话，就直接返回
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
			int opponent = getRival(player_to_move);
			// 这里表示模拟已经结束了
			if (move == pass) {
				player_to_move = opponent;
				return;
			}

			// 这里表示模拟没有结束，这时我们需要判断是否已经输赢论定了
			int i, j/*, Win*/;
			std::tie(i, j) = ind_to_ij(move);// 元组
			// 如果没有输赢未论定的话，就继续模拟
			cross[i][j] = player_to_move;

			// 轮到下一个玩家着子
			player_to_move = opponent;
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
			// 这里用于判断对方是否还有走步可以走，如果有的话就是说尚未达到游戏终止的情况。
			for (int i = ChessStart; i < ChessEnd; ++i) {
				for (int j = ChessStart; j < ChessEnd; ++j) {
					if (cross[i][j] == NoChess && chessScore[i][j] >= 20) {
						moves.push_back(ij_to_ind(i, j));
					}
				}
			}
			if (moves.empty()) {
				for (int i = ChessStart; i < ChessEnd; ++i) {
					for (int j = ChessStart; j < ChessEnd; ++j) {
						if (cross[i][j] == NoChess && chessScore[i][j] != minLimit) {
							moves.push_back(ij_to_ind(i, j));
						}
					}
				}
			}
			if (moves.empty()) {
				moves.push_back(pass);
			}
			return moves;
		}

		// 估计局势优劣
		// 主要是通过计算眼的数量和着子的数量
		virtual double get_result(int current_player_to_move) const
		{
			// 获取玩家的当前估分
			// 这个函数存在问题，这是根据着子的数量和眼的数量进行给分的
			auto get_player_score = [&](int player)
			{
				int score = 0;
				for (int i = ChessStart; i < ChessEnd; ++i) {
					for (int j = ChessStart; j < ChessEnd; ++j) {
						if (this->cross[i][j] == player) {
							score++;
						}
					}
				}
				return score += player == player_to_move ? ourEyes : RivalEyes;
			};
			int black = get_player_score(1);// 获取眼的数量
			int white = get_player_score(2);// 获取眼的数量

			if (black == white) {
				return 0.5;
			}

			// 按照眼的数量推断是谁会赢
			// 如果玩家1的眼的数量比玩家2的多，那么玩家1会赢
			int winner = black > white ? Black : White;

			// 如果赢者是当前移动的
			return winner == current_player_to_move ? 0.0 : 1.0;
		}

		virtual bool checkEmptyPos(int& x, int& y, int& start, int& mainColor, Pos emptyPos[]) override {
			/******************************************
			判断当前匹配到的空位是否是敌方的自杀点，
			如果是的话，就把该点的分数设置为0，跳过匹配模式
			*******************************************/
			for (int i = 0; i < start; ++i) {
				if (mainColor == Rival) {
					// 临时设置当前获得的位置为敌方着子点，判断是否是敌方的自杀点
					cross[emptyPos[i].line][emptyPos[i].column] = Rival;
					if (isGo2Dead(emptyPos[i].line, emptyPos[i].column, Rival)) {
						cross[emptyPos[i].line][emptyPos[i].column] = NoChess;
						// 如果是敌方的自杀点的话，这里就置零   -.-！！！
						chessScore[emptyPos[i].line][emptyPos[i].column] = 0;
						return false;
					}
				}
				else if (mainColor == turn2Who) {
					// 临时设置当前获得的位置为我方着子点，判断是否是我方的自杀点
					cross[x][y] = turn2Who;
					if (isGo2Dead(x, y, turn2Who)) {
						chessScore[x][y] = minLimit;
						cross[x][y] = NoChess;
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
		virtual bool checkStone(int& x, int& y) override {
			// 对于当前匹配到的着子点的环境进行分析
			// 临时设置当前获得的位置为我方着子点，判断是否是我方的自杀点
			cross[x][y] = turn2Who;
			if (isGo2Dead(x, y, turn2Who)) {
				chessScore[x][y] = minLimit;
				cross[x][y] = NoChess;
				// 因为我们需要统计眼的数量来分析当前的局势，所以我们需要记录眼
				++this->RivalEyes;
			}
			// 临时设置当前获得的位置为敌方着子点，判断是否是敌方的自杀点
			if (cross[x][y] == NoChess && chessScore[x][y] == 0) return false;
			cross[x][y] = Rival;
			if (isGo2Dead(x, y, Rival)) {
				cross[x][y] = NoChess;
				// 如果是敌方的自杀点的话，这里就置零   -.-！！！
				if (chessScore[x][y] != minLimit) {
					chessScore[x][y] = 0;
				}

				++this->ourEyes;
				return false;
			}
			// 这里既不是我方自杀点，也不是敌方自杀点
			cross[x][y] = NoChess;
			return true;
		}
	};

};

#endif // AI3_H_INCLUDED
