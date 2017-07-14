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

	// ����߲�
	template<typename RandomEngine>
	void do_random_move(RandomEngine* engine, std::vector<int> moves)
	{
		if (moves.empty()) {// ������ӵ㼯��Ϊ�յĻ�����ֱ�ӷ���
			Winner = getRival(player_to_move);
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
		int rival = getRival(player_to_move);
		// �����ʾģ��û�н�������ʱ������Ҫ�ж��Ƿ��Ѿ���Ӯ�۶���
		int line, column;
		line = getLine(move);
		column = getColumn(move);
		// ���û����Ӯδ�۶��Ļ����ͼ���ģ��
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

		// �ֵ���һ���������
		player_to_move = rival;
	}

	// �Ƿ��п����ӵ����ӵ�
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
			CS = chessScore;// ����Ŀ�ⲻ���õ���
		}
	}

	// ���������Ѽ����п��е����ӵ�
	virtual std::vector<int> get_moves() const
	{
		// �������Ѽ����п��ܵ����ӵ㡣
		std::vector<int> moves;

		// �����ȴ���888��Ļ��ͣ�ֱ��������moves�ˡ�
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
			// ����Pattern�Ե�ǰ������д��������п��ܵ����ӵ���뵽moves����
			_cprintf("Player is -> %s \n", player_to_move == Black ? "Black" : "White");
		}

		const_cast<SimulatorGo*>(this)->initCSPoint();
		initSimulation();
		const_cast<SimulatorGo*>(this)->Revalute();

		// �ӵ�ǰ������ѡ�����Patternƥ������ıȽϺõ����ӵ㼯��
		for (int i = ChessStart; i < ChessEnd; ++i) {
			for (int j = ChessStart; j < ChessEnd; ++j) {
				if (cross[i][j] == NoChess && CS[i][j] >= 10) {
					moves.emplace_back(getMove(i, j));
				}
			}
		}
		// �����ǰPatternƥ����֮��û��һ���Ǻõ����ӵ�
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

		// ���ִ����������֮��û��һ�����ӵ�����µĻ�����Ϸ������
		if (ifShowInfo && moves.empty()) {
			_cprintf("moves is null.\n");
		}
		return moves;
	}

	// �����ж���Ӯ���
	double get_result(int current_player_to_move) const
	{
		double Score = Winner == current_player_to_move ? 0.0 : 1.0;
		return Score;
	}

	virtual bool checkEmptyPos(const int& x, const int& y, const int& start, const int& mainColor, const Pos *emptyPos) override {
		/******************************************
		�жϵ�ǰƥ�䵽�Ŀ�λ�Ƿ��ǵз�����ɱ�㣬
		����ǵĻ����ͰѸõ�ķ�������Ϊ0������ƥ��ģʽ
		*******************************************/
		int rival = getRival(player_to_move);
		for (int i = 0; i < start; ++i) {
			if (mainColor == rival) {
				// ��ʱ���õ�ǰ��õ�λ��Ϊ�з����ӵ㣬�ж��Ƿ��ǵз�����ɱ��
				if (isGo2Dead(emptyPos[i].line, emptyPos[i].column, rival)) {
					// ����ǵз�����ɱ��Ļ������������   -.-������
					CS[emptyPos[i].line][emptyPos[i].column] = 0;
					return false;
				}
			}
			else if (mainColor == player_to_move) {
				// ��ʱ���õ�ǰ��õ�λ��Ϊ�ҷ����ӵ㣬�ж��Ƿ����ҷ�����ɱ��
				if (isGo2Dead(x, y, player_to_move)) {
					CS[x][y] = minLimit;
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
	virtual bool checkStone(const int& x, const int& y) override {
		// ���ڵ�ǰƥ�䵽�����ӵ�Ļ������з���
		// ��ʱ���õ�ǰ��õ�λ��Ϊ�ҷ����ӵ㣬�ж��Ƿ����ҷ�����ɱ��
		int rival = getRival(player_to_move);
		if (isGo2Dead(x, y, player_to_move)) {
			CS[x][y] = minLimit;
			// ������ҷ�����ɱ��Ļ�����ֱ����ת�������ж��Ƿ��ǵз�����ɱ���ˡ�
			return false;
		}
		// ��ʱ���õ�ǰ��õ�λ��Ϊ�з����ӵ㣬�ж��Ƿ��ǵз�����ɱ��
		if (cross[x][y] == NoChess && CS[x][y] == 0) return false;
		if (isGo2Dead(x, y, rival)) {
			// ����ǵз�����ɱ��Ļ������������   -.-������
			CS[x][y] = 0;
			return false;
		}
		// ����Ȳ����ҷ���ɱ�㣬Ҳ���ǵз���ɱ��
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
