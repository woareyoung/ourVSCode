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
		initCSPoint();
		for (int i = ChessInit; i < ChessEnd; ++i) {
			for (int j = ChessInit; j < ChessEnd; ++j) {
				cross[i][j] = b[i][j];
				chessScoreW[i][j] = this->getDefaultChessScore(i, j);
				chessScore[i][j] = chessScoreW[i][j];
			}
		}
	}

	void initSimulation() const;

	std::vector<int> getAllMoves();

	// �Ƿ��п����ӵ����ӵ�
	template<typename RandomEngine>
	bool doRandomMove(RandomEngine* engine);
	// ����߲�
	template<typename RandomEngine>
	void doRandomMove(RandomEngine* engine, std::vector<int>& moves);

	// ģ�����ӣ���Ҫ������������ģ������
	virtual void SimulateMove(const int& move)
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
		// ��ʾģ������ݣ�ʹ��ifShowInfo���п���
		showSimaluteInfo(line, column);

		// �ֵ���һ���������
		player_to_move = rival;
	}

	// ���������Ѽ����п��е����ӵ�
	virtual std::vector<int> getMoves() const;

	// �����ж���Ӯ���
	double getResult(const int& current_player_to_move) const
	{
		return Winner == current_player_to_move ? 0.0 : 1.0;
	}

	virtual bool checkEmptyPos(
		const int& x,
		const int& y,
		const int& start,
		const int& mainColor,
		const Pos* emptyPos) override;
	// ��������Ƿ���Ч�����Է����Ľ��������Ӧ�ļӷ�
	virtual bool checkStone(
		const int& x, 
		const int& y, 
		const bool& below4) override;
	virtual void initCSPoint() override;

	void showSimaluteInfo(const int& line, const int& column);
};

class AI3 : public AI2
{
private:

public:

	virtual int maxandmin(const int& depth) override {
		initChessScore(true);
		int maxCount = 6;
		int bestMove;
		/*do {*/
			bestMove = predict();
		/*	--maxCount;
		} while (this->isGo2Dead(getLine(bestMove), getColumn(bestMove), turn2Who) && maxCount != 0);
		if (maxCount == 0) {
			bestMove = 0;
		}*/
		return bestMove;
	}
	int predict() {
		MCTS::ComputeOptions options;
		options.number_of_threads = 1;
		options.verbose = true;
		options.max_iterations = 1;
		// options.max_time = 1;
		auto state_copy = new SimulatorGo(cross, PlayerId);
		auto best_move = MCTS::computeNextMove(state_copy, options);
		return best_move;
	}
};

#endif // AI3_H_INCLUDED
