#pragma once
#ifndef AI3_H_INCLUDED
#define AI3_H_INCLUDED

#include "../AI2_Header/AI2.h"
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
		options.max_time = 3;
		auto state_copy = new MCTS::SimulatorGo(cross, PlayerId);
		std::future<int> best_move = std::async(std::launch::async, [state_copy, options]()
		{
			return MCTS::compute_move(state_copy, options);
		});
		std::future_status status;
		do {
			status = best_move.wait_for(std::chrono::seconds(1));
		} while (status != std::future_status::ready);
		return best_move.get();
	}

};

#endif // AI3_H_INCLUDED
