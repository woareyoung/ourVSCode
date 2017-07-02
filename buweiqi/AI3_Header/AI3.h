#pragma once
#ifndef AI3_H_INCLUDED
#define AI3_H_INCLUDED

#include "../AI2_Header/AI2.h"
#include "MCTS.h"
#include <tuple>
#include <set>

#define pass (-2)
#define no_move (-1)

class AI3 : public AI2
{
private:
	mutable unsigned int board[ChessEnd][ChessEnd];
	mutable unsigned int simulatorScore[ChessEnd][ChessEnd];
	unsigned int previous_board_hash_value;
	std::set<unsigned int> all_hash_values;

	// 事实上，这个是Turn2who
	typedef int Move;
	Move player_to_move;
public:
	mutable int depth;
	AI3() : depth(0), previous_board_hash_value(0)
	{
		all_hash_values.insert(compute_hash_value());
	}

	// 轮到下一方进行着子
	virtual void do_move(Move move)
	{
		depth++;

		int opponent = 3 - player_to_move;

		if (move == pass) {
			player_to_move = opponent;
			return;
		}

		int i, j;
		std::tie(i, j) = ind_to_ij(move);// 元组
		attest(is_move_possible(i, j));

		board[i][j] = player_to_move;

		// 我们在所有捕获之前保存棋子的哈希值，以方便检查。
		previous_board_hash_value = compute_hash_value();
		all_hash_values.insert(previous_board_hash_value);

		// 现在的着子点一定是可行的。
		attest(board[i][j] == player_to_move);

		// Next player
		// 轮到下一个玩家着子
		player_to_move = getRival(player_to_move);
	}

	static int ij_to_ind(int i, int j)
	{
		attest(i >= ChessStart && j >= ChessStart && i < ChessEnd && j < ChessEnd);
		return 100 * i + j;
	}


	static std::pair<int, int> ind_to_ij(int ind)
	{
		attest(ind >= 0 && ind < 910);
		return std::make_pair(ind / 100, ind % 100);
	}

	// 计算hash值
	virtual unsigned int compute_hash_value() const
	{
		unsigned int value = 0;
		for (int i = ChessStart; i < ChessEnd; ++i) {
			for (int j = ChessStart; j < ChessEnd; ++j) {
				value = 65537 * value + board[i][j];
			}
		}
		return value;
	}

	// 随机走步
	template<typename RandomEngine>
	void do_random_move(RandomEngine* engine)
	{
		// 调用Pattern对当前局面进行处理
		const_cast<AI3*>(this)->Revalute();
		auto moves = get_moves();
		attest(!moves.empty());
		std::uniform_int_distribution<std::size_t> move_ind(0, moves.size() - 1);
		auto move = moves[move_ind(*engine)];
		do_move(move);
	}

	// 是否还有可着子的着子点
	virtual bool has_moves() const
	{
		return !get_moves().empty();
	}

	// 从可着子的着子点集合中获取一个着子点
	virtual std::vector<Move> get_moves() const
	{
		// 下面是搜集所有可能的着子点。
		std::vector<Move> moves;
		if (depth > 1000) {
			attest(false);
			return moves;
		}
		bool opponent_has_move = false;
		for (int i = ChessStart; i < ChessEnd; ++i) {
			for (int j = ChessStart; j < ChessEnd; ++j) {
				if (is_move_possible(i, j, player_to_move)) {
					moves.push_back(ij_to_ind(i, j));
				}
				// 假如对手没有着子点可以下，并且我方的着子点
				if (!opponent_has_move && is_move_possible(i, j, 3 - player_to_move)) {
					opponent_has_move = true;
				}
			}
		}
		if (moves.empty() && opponent_has_move) {
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
					if (board[i][j] == player) {
						score++;
					}
				}
			}
			return score += player == player_to_move ? ourEyes : RivalEyes;
		};
		int score1 = get_player_score(1);// 获取眼的数量
		int score2 = get_player_score(2);// 获取眼的数量

		if (score1 == score2) {
			return 0.5;
		}

		// 按照眼的数量推断是谁会赢
		// 如果玩家1的眼的数量比玩家2的多，那么玩家1会赢
		int winner = score1 > score2 ? 1 : 2;

		// 如果赢者是当前移动的
		return winner == current_player_to_move ? 0.0 : 1.0;
	}


	virtual bool checkEmptyPos(int& x, int& y, int& start, int& mainColor, Pos emptyPos[])  override {
		/******************************************
		判断当前匹配到的空位是否是敌方的自杀点，
		如果是的话，就把该点的分数设置为0，跳过匹配模式
		*******************************************/
		for (int i = 0; i < start; ++i) {
			if (mainColor == Rival) {
				// 临时设置当前获得的位置为敌方着子点，判断是否是敌方的自杀点
				board[emptyPos[i].line][emptyPos[i].column] = Rival;
				if (isGo2Dead(emptyPos[i].line, emptyPos[i].column, Rival)) {
					board[emptyPos[i].line][emptyPos[i].column] = NoChess;
					// 如果是敌方的自杀点的话，这里就置零   -.-！！！
					simulatorScore[emptyPos[i].line][emptyPos[i].column] = 0;
					return false;
				}
			}
			else if (mainColor == turn2Who) {
				// 临时设置当前获得的位置为我方着子点，判断是否是我方的自杀点
				board[x][y] = turn2Who;
				if (isGo2Dead(x, y, turn2Who)) {
					simulatorScore[x][y] = minLimit;
					board[x][y] = NoChess;
					// 如果是我方的自杀点的话，就直接跳转，不用判断是否是敌方的自杀点了。
					return false;
				}
			}
			// 这里既不是我方自杀点，也不是敌方自杀点
			board[emptyPos[i].line][emptyPos[i].column] = NoChess;
		}
		return true;
	}
	// 检查棋子是否有效，并对分析的结果进行相应的加分
	virtual bool checkStone(int& x, int& y) override {
		// 对于当前匹配到的着子点的环境进行分析
		// 临时设置当前获得的位置为我方着子点，判断是否是我方的自杀点
		board[x][y] = turn2Who;
		if (isGo2Dead(x, y, turn2Who)) {
			simulatorScore[x][y] = minLimit;
			board[x][y] = NoChess;
			// 因为我们需要统计眼的数量来分析当前的局势，所以我们需要记录眼
			++this->RivalEyes;
		}
		// 临时设置当前获得的位置为敌方着子点，判断是否是敌方的自杀点
		if (board[x][y] == NoChess && simulatorScore[x][y] == 0) return false;
		board[x][y] = Rival;
		if (isGo2Dead(x, y, Rival)) {
			board[x][y] = NoChess;
			// 如果是敌方的自杀点的话，这里就置零   -.-！！！
			simulatorScore[x][y] = 0;
			++this->ourEyes;
			return false;
		}
		// 这里既不是我方自杀点，也不是敌方自杀点
		board[x][y] = NoChess;
		return true;
	}

	virtual bool is_move_possible(int i, int j) const
	{
		return simulatorScore[i][j] != minLimit && is_move_possible(i, j, player_to_move);
	}

	virtual bool is_move_possible(const int i, const int j, const int player) const
	{
		const int opponent = getRival(player);// 对手

		if (!OnChessBoard(i, j)) {
			if (board[i][j] != NoChess) {
				return false;
			}
			board[i][j] = player;

			bool possible = false;
			if (board[i][j] != minLimit) {
				possible = true;
			}

			if (possible) {
				if (compute_hash_value() == previous_board_hash_value) {
					possible = false;
				}
				else if (all_hash_values.find(compute_hash_value()) != all_hash_values.end()) {
					possible = false;
				}
			}

			board[i][j] = NoChess;
			return possible;
		}
		else {
			// 没有可行的着子点
			return false;
		}
	}

	void predict() {
		
	}
};

#endif // AI3_H_INCLUDED
