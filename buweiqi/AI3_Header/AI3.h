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
			auto moves = get_moves();// ��ȡ�������ӵ����ӵ㼯��
			if (moves.empty()) {// ������ӵ㼯��Ϊ�յĻ�����ֱ�ӷ���
				return;
			}
			std::uniform_int_distribution<std::size_t> move_ind(0, moves.size() - 1);
			auto move = moves[move_ind(*engine)];
			// ��ʼ�߲�
			do_move(move);
		}

		// ģ������
		virtual void do_move(int move)
		{
			// ������ȼ�1
			depth++;
			// �Է��Ľ��
			int opponent = getRival(player_to_move);
			// �����ʾģ���Ѿ�������
			if (move == pass) {
				player_to_move = opponent;
				return;
			}

			// �����ʾģ��û�н�������ʱ������Ҫ�ж��Ƿ��Ѿ���Ӯ�۶���
			int i, j/*, Win*/;
			std::tie(i, j) = ind_to_ij(move);// Ԫ��
			// ���û����Ӯδ�۶��Ļ����ͼ���ģ��
			cross[i][j] = player_to_move;

			// �ֵ���һ���������
			player_to_move = opponent;
		}

		// �Ƿ��п����ӵ����ӵ�
		virtual bool has_moves() const
		{
			return !get_moves().empty();
		}

		// ���������Ѽ����п��е����ӵ�
		virtual std::vector<int> get_moves() const
		{
			initSimulation();
			// ����Pattern�Ե�ǰ������д��������п��ܵ����ӵ���뵽moves����
			const_cast<SimulatorGo*>(this)->Revalute();
			// �������Ѽ����п��ܵ����ӵ㡣
			std::vector<int> moves;
			// �����ȴ���1000��Ļ��ͣ�ֱ��������moves�ˡ�
			if (depth > 1000) {
				return moves;
			}
			// ���������ж϶Է��Ƿ����߲������ߣ�����еĻ�����˵��δ�ﵽ��Ϸ��ֹ�������
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
						if (this->cross[i][j] == player) {
							score++;
						}
					}
				}
				return score += player == player_to_move ? ourEyes : RivalEyes;
			};
			int black = get_player_score(1);// ��ȡ�۵�����
			int white = get_player_score(2);// ��ȡ�۵�����

			if (black == white) {
				return 0.5;
			}

			// �����۵������ƶ���˭��Ӯ
			// ������1���۵����������2�Ķ࣬��ô���1��Ӯ
			int winner = black > white ? Black : White;

			// ���Ӯ���ǵ�ǰ�ƶ���
			return winner == current_player_to_move ? 0.0 : 1.0;
		}

		virtual bool checkEmptyPos(int& x, int& y, int& start, int& mainColor, Pos emptyPos[]) override {
			/******************************************
			�жϵ�ǰƥ�䵽�Ŀ�λ�Ƿ��ǵз�����ɱ�㣬
			����ǵĻ����ͰѸõ�ķ�������Ϊ0������ƥ��ģʽ
			*******************************************/
			for (int i = 0; i < start; ++i) {
				if (mainColor == Rival) {
					// ��ʱ���õ�ǰ��õ�λ��Ϊ�з����ӵ㣬�ж��Ƿ��ǵз�����ɱ��
					cross[emptyPos[i].line][emptyPos[i].column] = Rival;
					if (isGo2Dead(emptyPos[i].line, emptyPos[i].column, Rival)) {
						cross[emptyPos[i].line][emptyPos[i].column] = NoChess;
						// ����ǵз�����ɱ��Ļ������������   -.-������
						chessScore[emptyPos[i].line][emptyPos[i].column] = 0;
						return false;
					}
				}
				else if (mainColor == turn2Who) {
					// ��ʱ���õ�ǰ��õ�λ��Ϊ�ҷ����ӵ㣬�ж��Ƿ����ҷ�����ɱ��
					cross[x][y] = turn2Who;
					if (isGo2Dead(x, y, turn2Who)) {
						chessScore[x][y] = minLimit;
						cross[x][y] = NoChess;
						// ������ҷ�����ɱ��Ļ�����ֱ����ת�������ж��Ƿ��ǵз�����ɱ���ˡ�
						return false;
					}
				}
				// ����Ȳ����ҷ���ɱ�㣬Ҳ���ǵз���ɱ��
				cross[emptyPos[i].line][emptyPos[i].column] = NoChess;
			}
			return true;
		}
		// ��������Ƿ���Ч�����Է����Ľ��������Ӧ�ļӷ�
		virtual bool checkStone(int& x, int& y) override {
			// ���ڵ�ǰƥ�䵽�����ӵ�Ļ������з���
			// ��ʱ���õ�ǰ��õ�λ��Ϊ�ҷ����ӵ㣬�ж��Ƿ����ҷ�����ɱ��
			cross[x][y] = turn2Who;
			if (isGo2Dead(x, y, turn2Who)) {
				chessScore[x][y] = minLimit;
				cross[x][y] = NoChess;
				// ��Ϊ������Ҫͳ���۵�������������ǰ�ľ��ƣ�����������Ҫ��¼��
				++this->RivalEyes;
			}
			// ��ʱ���õ�ǰ��õ�λ��Ϊ�з����ӵ㣬�ж��Ƿ��ǵз�����ɱ��
			if (cross[x][y] == NoChess && chessScore[x][y] == 0) return false;
			cross[x][y] = Rival;
			if (isGo2Dead(x, y, Rival)) {
				cross[x][y] = NoChess;
				// ����ǵз�����ɱ��Ļ������������   -.-������
				if (chessScore[x][y] != minLimit) {
					chessScore[x][y] = 0;
				}

				++this->ourEyes;
				return false;
			}
			// ����Ȳ����ҷ���ɱ�㣬Ҳ���ǵз���ɱ��
			cross[x][y] = NoChess;
			return true;
		}
	};

};

#endif // AI3_H_INCLUDED
