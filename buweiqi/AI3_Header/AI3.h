#pragma once
#ifndef AI3_H_INCLUDED
#define AI3_H_INCLUDED

#include "../AI2_Header/AI2.h"
#include <tuple>
#include <set>

#define pass (-2)
#define no_move (-1)

class AI3 : public AI2
{
private:
	unsigned int previous_board_hash_value;
	std::set<unsigned int> all_hash_values;

	// ��ʵ�ϣ������Turn2who
	typedef int Move;
	Move player_to_move;
public:
	mutable int depth;
	AI3() : depth(0), previous_board_hash_value(0)
	{
		all_hash_values.insert(compute_hash_value());
	}

	// �ֵ���һ����������
	virtual void do_move(Move move)
	{
		depth++;

		int i, j;
		std::tie(i, j) = ind_to_ij(move);// Ԫ��
		cross[i][j] = player_to_move;

		// ���������в���֮ǰ�������ӵĹ�ϣֵ���Է����顣
		previous_board_hash_value = compute_hash_value();
		all_hash_values.insert(previous_board_hash_value);

		std::set<std::pair<int, int>> pieces;
		// ���ڵ����ӵ�һ���ǿ��еġ�
		attest(cross[i][j] == player_to_move);

		// Next player
		// �ֵ���һ���������
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

	// ����hashֵ
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

	// ����߲�
	template<typename RandomEngine>
	void do_random_move(RandomEngine* engine)
	{
		auto moves = get_moves();
		attest(!moves.empty());
		std::uniform_int_distribution<std::size_t> move_ind(0, moves.size() - 1);
		auto move = moves[move_ind(*engine)];
		do_move(move);
	}

	// �Ƿ��п����ӵ����ӵ�
	virtual bool has_moves() const
	{
		// TODO: make faster.
		return !get_moves().empty();
	}

	// �ӿ����ӵ����ӵ㼯���л�ȡһ�����ӵ�
	virtual std::vector<Move> get_moves() const
	{
		std::vector<Move> moves;
		if (depth > 1000) {
			attest(false);
			return moves;
		}

		bool opponent_has_move = false;
		for (int i = ChessStart; i < ChessEnd; ++i) {
			for (int j = ChessStart; j < ChessEnd; ++j) {
				if (chessScore[i][j] != minLimit) {
					moves.push_back(ij_to_ind(i, j));
				}

				if (!opponent_has_move && chessScore[i][j] != minLimit) {
					opponent_has_move = true;
				}
			}
		}

		if (moves.empty() && opponent_has_move) {
			moves.push_back(pass);
		}

		return moves;
	}

	// ��ȡ��ҵĵ�ǰ����
	// ��������������⣬���Ǹ������ӵ��������۵��������и��ֵ�
	virtual int get_player_score(int player) const
	{
		int score = 0;
		//for (int i = 0; i < M; ++i) {
		//	for (int j = 0; j < N; ++j) {
		//		if (board[i][j] == empty) {
		//			if (is_eye(i, j, player)) {
		//				score++;
		//			}
		//		}
		//		if (board[i][j] == player) {
		//			score++;
		//		}
		//	}
		//}
		return score;
	}
};

#endif // AI3_H_INCLUDED
