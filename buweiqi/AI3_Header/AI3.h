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

	// ����߲�
	template<typename RandomEngine>
	void do_random_move(RandomEngine* engine)
	{
		auto moves = get_moves();// ��ȡ�������ӵ����ӵ㼯��
		if (moves.empty()) {// ������ӵ㼯��Ϊ�յĻ�����ֱ�ӷ���
			Winner = getRival(player_to_move);
			return;
		}
		std::uniform_int_distribution<std::size_t> move_ind(0, moves.size() - 1);
		auto move = moves[move_ind(*engine)];
		// ��ʼ�߲�
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

	// ģ������
	virtual void do_move(int move)
	{
		// ������ȼ�1
		depth++;
		// �Է��Ľ��
		int rival = getRival(player_to_move);
		if (move == no_move) {
			Winner = rival;
			return;
		}
		// �����ʾģ��û�н�������ʱ������Ҫ�ж��Ƿ��Ѿ���Ӯ�۶���
		int line, column;
		line = getLine(move);
		column = getColumn(move);

		chessInfo.WinOrLose(line, column, player_to_move, Winner, cross);
		if (Winner == Black || Winner == White) {
			return;
		}

		// ���û����Ӯδ�۶��Ļ����ͼ���ģ��
		cross[line][column] = player_to_move;

		// �ֵ���һ���������
		player_to_move = rival;
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
		// �ӵ�ǰ������ѡ�����Patternƥ������ıȽϺõ����ӵ㼯��
		for (int i = ChessStart; i < ChessEnd; ++i) {
			for (int j = ChessStart; j < ChessEnd; ++j) {
				if (cross[i][j] == NoChess && chessScore[i][j] >= 20) {
					moves.emplace_back(getMove(i, j));
				}
			}
		}
		// �����ǰPatternƥ����֮��û��һ���Ǻõ����ӵ�
		if (moves.empty()) {
			for (int i = ChessStart; i < ChessEnd; ++i) {
				for (int j = ChessStart; j < ChessEnd; ++j) {
					if (cross[i][j] == NoChess && chessScore[i][j] != minLimit) {
						moves.emplace_back(getMove(i, j));
					}
				}
			}
		}
		// ���ִ����������֮��û��һ�����ӵ�����µĻ�����Ϸ������
		/*_cprintf("\n moves` vector is : \n");
		for (auto move : moves) {
		_cprintf("%d.", move);
		}*/
		return moves;
	}

	// �����ж���Ӯ���
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
