#pragma once

#include "../ChessBoard_Header/WinCheck.h"
#include "../AI2_Header/AI2.h"
#include "../ChessBoard_Header/showUnicodeInfo.h"
#include "../ChessBoard_Header/parameter.h"

class SimulatorGo : public AI2 {
private:
	mutable int chessScoreW[ChessEnd][ChessEnd];
protected:
	virtual int* getPatternType() override {
		return Type[player_to_move - 1];
	}
public:
	bool ifShowInfo;

	mutable int Winner;
	int player_to_move;
	SimulatorGo() :
		Winner(NoChess),
		ifShowInfo(false)
	{
		initAllArray();
		for (int i = ChessInit; i < ChessEnd; ++i) {
			for (int j = ChessInit; j < ChessEnd; ++j) {
				chessScoreW[i][j] = getDefaultChessScore(i, j);
				chessScore[i][j] = chessScoreW[i][j];
			}
		}
	}

	SimulatorGo(
		int b[ChessEnd][ChessEnd],
		int Id) :
		player_to_move(Id),
		Winner(NoChess),
		ifShowInfo(false)
	{
		setRole(Id);
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

	void initSimulation() /*const*/;

	std::vector<int> getAllMoves();

	// �Ƿ��п����ӵ����ӵ�
	template<typename RandomEngine>
	bool doRandomMove(RandomEngine* engine)
	{
		initCSPoint();
		auto moves = getMoves();
		if (moves.empty()) {// ������ӵ㼯��Ϊ�յĻ�����ֱ�ӷ���
			Winner = getRival(player_to_move);
			return false;
		}
		else {
			std::uniform_int_distribution<std::size_t> move_ind(0, moves.size() - 1);
			// ��ʼģ���߲�
			SimulateMove(moves[move_ind(*engine)]);
			return true;
		}
		return false;
	}

	// ģ�����ӣ���Ҫ������������ģ������
	virtual void SimulateMove(const int& move)
	{
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
		initCSPoint();
	}

	// ���������Ѽ����п��е����ӵ�
	virtual std::vector<int> getMoves() /*const*/;

	// �����ж���Ӯ���
	virtual double getResult(const int& current_player_to_move)
	{
		// û��ģ�⵽��ͷ
		if (Winner != White && Black != Black) {
			return 0.5;
		}
		return Winner == current_player_to_move ? 1.0 : 0.0;
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
	virtual void ScanChessBroad() override;

	void showSimaluteInfo(const int& line, const int& column);
};

class AI3 : public AI2
{
private:

public:

	virtual int maxandmin(const int& depth) override {
		initChessScore(true);
		return predict();
	}
	int predict();
};

