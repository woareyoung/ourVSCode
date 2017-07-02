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

		int opponent = 3 - player_to_move;

		if (move == pass) {
			player_to_move = opponent;
			return;
		}

		int i, j;
		std::tie(i, j) = ind_to_ij(move);// Ԫ��
		attest(is_move_possible(i, j));

		board[i][j] = player_to_move;

		// ���������в���֮ǰ�������ӵĹ�ϣֵ���Է����顣
		previous_board_hash_value = compute_hash_value();
		all_hash_values.insert(previous_board_hash_value);

		// ���ڵ����ӵ�һ���ǿ��еġ�
		attest(board[i][j] == player_to_move);

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
				value = 65537 * value + board[i][j];
			}
		}
		return value;
	}

	// ����߲�
	template<typename RandomEngine>
	void do_random_move(RandomEngine* engine)
	{
		// ����Pattern�Ե�ǰ������д���
		const_cast<AI3*>(this)->Revalute();
		auto moves = get_moves();
		attest(!moves.empty());
		std::uniform_int_distribution<std::size_t> move_ind(0, moves.size() - 1);
		auto move = moves[move_ind(*engine)];
		do_move(move);
	}

	// �Ƿ��п����ӵ����ӵ�
	virtual bool has_moves() const
	{
		return !get_moves().empty();
	}

	// �ӿ����ӵ����ӵ㼯���л�ȡһ�����ӵ�
	virtual std::vector<Move> get_moves() const
	{
		// �������Ѽ����п��ܵ����ӵ㡣
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
				// �������û�����ӵ�����£������ҷ������ӵ�
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

	// ���ƾ�������
	// ��Ҫ��ͨ�������۵����������ӵ�����
	virtual double get_result(int current_player_to_move) const
	{
		// ��ȡ��ҵĵ�ǰ����
		// ��������������⣬���Ǹ������ӵ��������۵��������и��ֵ�
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
		int score1 = get_player_score(1);// ��ȡ�۵�����
		int score2 = get_player_score(2);// ��ȡ�۵�����

		if (score1 == score2) {
			return 0.5;
		}

		// �����۵������ƶ���˭��Ӯ
		// ������1���۵����������2�Ķ࣬��ô���1��Ӯ
		int winner = score1 > score2 ? 1 : 2;

		// ���Ӯ���ǵ�ǰ�ƶ���
		return winner == current_player_to_move ? 0.0 : 1.0;
	}


	virtual bool checkEmptyPos(int& x, int& y, int& start, int& mainColor, Pos emptyPos[])  override {
		/******************************************
		�жϵ�ǰƥ�䵽�Ŀ�λ�Ƿ��ǵз�����ɱ�㣬
		����ǵĻ����ͰѸõ�ķ�������Ϊ0������ƥ��ģʽ
		*******************************************/
		for (int i = 0; i < start; ++i) {
			if (mainColor == Rival) {
				// ��ʱ���õ�ǰ��õ�λ��Ϊ�з����ӵ㣬�ж��Ƿ��ǵз�����ɱ��
				board[emptyPos[i].line][emptyPos[i].column] = Rival;
				if (isGo2Dead(emptyPos[i].line, emptyPos[i].column, Rival)) {
					board[emptyPos[i].line][emptyPos[i].column] = NoChess;
					// ����ǵз�����ɱ��Ļ������������   -.-������
					simulatorScore[emptyPos[i].line][emptyPos[i].column] = 0;
					return false;
				}
			}
			else if (mainColor == turn2Who) {
				// ��ʱ���õ�ǰ��õ�λ��Ϊ�ҷ����ӵ㣬�ж��Ƿ����ҷ�����ɱ��
				board[x][y] = turn2Who;
				if (isGo2Dead(x, y, turn2Who)) {
					simulatorScore[x][y] = minLimit;
					board[x][y] = NoChess;
					// ������ҷ�����ɱ��Ļ�����ֱ����ת�������ж��Ƿ��ǵз�����ɱ���ˡ�
					return false;
				}
			}
			// ����Ȳ����ҷ���ɱ�㣬Ҳ���ǵз���ɱ��
			board[emptyPos[i].line][emptyPos[i].column] = NoChess;
		}
		return true;
	}
	// ��������Ƿ���Ч�����Է����Ľ��������Ӧ�ļӷ�
	virtual bool checkStone(int& x, int& y) override {
		// ���ڵ�ǰƥ�䵽�����ӵ�Ļ������з���
		// ��ʱ���õ�ǰ��õ�λ��Ϊ�ҷ����ӵ㣬�ж��Ƿ����ҷ�����ɱ��
		board[x][y] = turn2Who;
		if (isGo2Dead(x, y, turn2Who)) {
			simulatorScore[x][y] = minLimit;
			board[x][y] = NoChess;
			// ��Ϊ������Ҫͳ���۵�������������ǰ�ľ��ƣ�����������Ҫ��¼��
			++this->RivalEyes;
		}
		// ��ʱ���õ�ǰ��õ�λ��Ϊ�з����ӵ㣬�ж��Ƿ��ǵз�����ɱ��
		if (board[x][y] == NoChess && simulatorScore[x][y] == 0) return false;
		board[x][y] = Rival;
		if (isGo2Dead(x, y, Rival)) {
			board[x][y] = NoChess;
			// ����ǵз�����ɱ��Ļ������������   -.-������
			simulatorScore[x][y] = 0;
			++this->ourEyes;
			return false;
		}
		// ����Ȳ����ҷ���ɱ�㣬Ҳ���ǵз���ɱ��
		board[x][y] = NoChess;
		return true;
	}

	virtual bool is_move_possible(int i, int j) const
	{
		return simulatorScore[i][j] != minLimit && is_move_possible(i, j, player_to_move);
	}

	virtual bool is_move_possible(const int i, const int j, const int player) const
	{
		const int opponent = getRival(player);// ����

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
			// û�п��е����ӵ�
			return false;
		}
	}

	void predict() {
		
	}
};

#endif // AI3_H_INCLUDED
