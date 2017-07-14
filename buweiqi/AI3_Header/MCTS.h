#pragma once
//
// Petter Strandmark 2013
// petter.strandmark@gmail.com
//
// 为有限游戏而编写的蒙卡洛树
// Monte Carlo Tree Search for finite games.
//
// Originally based on Python code at
// http://mcts.ai/code/python.html
//
// 使用了根的并行化技术
// Uses the "root parallelization" technique [1].

namespace MCTS
{
	struct ComputeOptions// 计算选择
	{
		int number_of_threads;// 线程数量
		int max_iterations;// 最大迭代次数
		double max_time;// 最长的时间
		bool verbose;// 是否显示Dos信息

		ComputeOptions() :
			number_of_threads(8),// 默认的线程数量是8条
			max_iterations(10000),// 默认的最大的迭代数量是10000
			max_time(-1.0), // 默认是没有时间限制的
			verbose(false)
		{ }
	};

	/**
	* 推测出一步的着子点
	* @param root_state 棋盘的属性
	* @param options 创建树的参数设置，包含多线程和深度
	*/
	template<typename State>
	int compute_move(const State root_state,
		const ComputeOptions options = ComputeOptions());
}
//
//
// [1] Chaslot, G. M. B., Winands, M. H., & van Den Herik, H. J. (2008).
//     Parallel monte-carlo tree search. In Computers and Games (pp.
//     60-71). Springer Berlin Heidelberg.
//

#include <algorithm>
#include <cstdlib>
#include <future>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <random>
#include <set>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
#include <conio.h>

#define no_move (-1)

// 蒙特卡洛树搜索——Monte Carlo Tree Search
namespace MCTS
{
	using std::cerr;
	using std::endl;
	using std::vector;
	using std::size_t;

	// 这个用于输出信息
#define showInfoOnDOS(info) if(info != NULL){_cprintf("%s\n", info);}

	// 这个类是用于构建蒙卡洛树的，蒙卡洛树的根结点由用户创建，剩余的其他的结点都是直接添加的。
	template<typename State>
	class Node
	{
	public:

		Node(const State& state);
		~Node();

		// 这里表示的是：
		// 是否还有本结点展开了的但是并没有尝试过的着子点
		bool has_untried_moves() const;
		template<typename RandomEngine>
		// 随机获取未曾遍历过的着子方法
		int get_untried_move(RandomEngine* engine) const;
		Node* best_child() const;// 最优的着子点

		bool has_children() const// 是否还有子节点
		{
			return !children.empty();
		}

		Node* select_child_UCT() const;// 使用UTC算法挑选出最优的子节点
		Node* add_child(const int& move, const State& state);// 添加子节点
		void update(double result);// 更新父节点的信息

		std::string to_string() const;
		std::string tree_to_string(int max_depth = 1000000, int indent = 0) const;

		const int move;// 
		Node* const parent;// 父节点
		const int player_to_move;// 当前执子的玩家

		double wins;// 估分
		int visits;// 访问次数

		std::vector<int> moves;// 这是移动路径
		std::vector<Node*> children; // 这是子节点

	private:
		Node(const State& state, const int& move, Node* parent);

		std::string indent_string(int indent) const;// 缩进字符串

		Node(const Node&);

		double UCT_score;// UCT分数
	};

	template<typename State>
	Node<State>::Node(const State& state) :
		move(no_move),// 初始化为没有走步
		parent(nullptr),// 初始化为空
		player_to_move(state->player_to_move),// 初始化为当前玩家一样的角色
		wins(0),
		visits(0),
		moves(state->get_moves()),// 可供着子的着子点集
		UCT_score(0)
	{ }

	template<typename State>
	Node<State>::Node(const State& state, const int& move_, Node* parent_) :
		move(move_),
		parent(parent_),
		player_to_move(state->player_to_move),
		wins(0),
		visits(0),
		moves(state->get_moves()),
		UCT_score(0)
	{ }

	template<typename State>
	Node<State>::~Node()
	{
		for (auto child : children) {// 将子节点占用的内存进行释放
			delete child;
		}
	}

	template<typename State>
	bool Node<State>::has_untried_moves() const// 是否还有没有遍历过的子节点
	{
		return !moves.empty();
	}

	// 获取未曾尝试过的着子点
	template<typename State>
	template<typename RandomEngine>
	int Node<State>::get_untried_move(RandomEngine* engine) const// 从没有遍历过的子节点中获取一个结点作为子树展开的结点
	{
		// 给定[0, move.size() - 1]范围初始化分布类
		std::uniform_int_distribution<std::size_t> moves_distribution(0, moves.size() - 1);
		return moves[moves_distribution(*engine)];
	}

	// 最优的着子点
	template<typename State>
	Node<State>* Node<State>::best_child() const
	{
		if (options.verbose && moves.empty()) {
			showInfoOnDOS("error, best_child -> moves vector is empty!");
		}
		if (options.verbose && children.empty()) {
			showInfoOnDOS("error, best_child -> node`s children is none!");
		}

		return *std::max_element(children.begin(), children.end(),
			[](Node* a, Node* b) { return a->visits < b->visits; });;
	}

	/**
	* 使用UTC算法选择UTC分数最优的一个子节点并返回
	* upper confidence bounds applied to trees
	* 原文網址：https://read01.com/NyRDaN.html
	*/
	template<typename State>
	Node<State>* Node<State>::select_child_UCT() const
	{
		/*if (moves.empty()) {
			showInfoOnDOS("error, select_child_UCT -> moves vector is empty!");
		}*/

		// 计算结点的UCT分数
		for (auto child : children) {
			child->UCT_score = double(child->wins) / double(child->visits) +
				2.0 * std::sqrt(std::log(double(this->visits)) / child->visits);
		}

		return *std::max_element(children.begin(), children.end(),
			[](Node* a, Node* b) { return a->UCT_score < b->UCT_score; });
	}

	/**
	* 添加子节点
	* @param move 当前着子点
	* @param state 当前棋盘的状态（局势）
	*/
	template<typename State>
	Node<State>* Node<State>::add_child(const int& move, const State& state)
	{
		// 新建新的结点，将新的结点添加到children中，并判断children数组是否为空
		auto node = new Node(state, move, this);
		children.push_back(node);

		if (children.empty()) {
			// shishowInfoOnDOS("error, node`s children is empty!");
		}

		// 从第一个着子点开始
		auto itr = moves.begin();
		// 重新修改着子点数组的大小
		for (; itr != moves.end() && *itr != move; ++itr);
		moves.erase(itr);// 从moves数组中删除move元素
		return node;
	}

	/**
	* 更新结点的访问次数和结点的wins
	* @param result 表示win分数
	*/
	template<typename State>
	void Node<State>::update(double result)
	{
		visits++;
		wins += result;
	}

	template<typename State>
	std::string Node<State>::to_string() const
	{
		std::stringstream sout;
		sout << "["
			<< "P" << 3 - player_to_move << " "
			<< "M:" << move << " "
			<< "W/V: " << wins << "/" << visits << " "
			<< "U: " << moves.size() << "]\n";
		return sout.str();
	}

	template<typename State>
	std::string Node<State>::tree_to_string(int max_depth, int indent) const
	{
		if (indent >= max_depth) {
			return "";
		}

		std::string s = indent_string(indent) + to_string();
		for (auto child : children) {
			s += child->tree_to_string(max_depth, indent + 1);
		}
		return s;
	}

	template<typename State>
	std::string Node<State>::indent_string(int indent) const
	{
		std::string s = "";
		for (int i = 1; i <= indent; ++i) {
			s += "| ";
		}
		return s;
	}

	/**
	* 生成树，随机的选定一个方向进行扩展
	* @param root_state 结点的棋盘数据
	* @param options  结点的并发属性
	* @param initial_seed 随机数初始化种子
	* @return 返回根结点
	*/
	template<typename State>
	std::unique_ptr<Node<State>>  compute_tree(
		const State root_state,
		const ComputeOptions options,
		std::mt19937_64::result_type initial_seed)
	{
		std::mt19937_64 random_engine(initial_seed);// 随机函数种子，用于随机走步

		if (options.verbose) {
			if (options.max_iterations >= 0 || options.max_time >= 0) {
				showInfoOnDOS("option is right~");
			}
			if (!(root_state->player_to_move == 1 || root_state->player_to_move == 2)) {
				showInfoOnDOS("player_to_move is error~");
			}
		}
		// root指针管理一个Node结点对象。
		auto root = std::unique_ptr<Node<State>>(new Node<State>(root_state));

		// 遍历，最大的遍历次数是ComputeOption的最大值
		// 对棋盘进行UTC加分处理
		for (int iter = 1; iter <= options.max_iterations/*最大的迭代数量*/ ||
			options.max_iterations < 0; ++iter) {
			auto node = root.get();
			State state = root_state;

			// 这里表示的是所有可以走的走步全部都创建为孩子结点了，
			// 不存在非孩子结点的走步move
			// node的没有尝试的move为空了，并且node并非叶子结点
			// 这里表示基本上所有的孩子结点都访问过了，这里我们就用UTC的算法对结点进行估分。
			while (!node->has_untried_moves() && node->has_children()) {
				node = node->select_child_UCT();
				state->do_move(node->move);
			}

			// 随机选取一个根结点
			if (node->has_untried_moves()) {
				auto move = node->get_untried_move(&random_engine);
				state->do_move(move);
				node = node->add_child(move, state);
			}

			// Okay，选取完根结点，我们就开始跑，循环直到结束。
			while (state->do_random_move(&random_engine)) {}

			// 我们已经到达了树的根结点了，这个时候我们递归回根结点，
			// 为下一次循环的UTC加分做准备
			while (node != nullptr) {
				node->update(state->get_result(node->player_to_move));// 这里只看输赢就好了
				node = node->parent;
			}
		}
		if (options.verbose) {
			auto node = root.get();
			_cprintf("*******compute tree:*******\n %s\n", node->tree_to_string().c_str());
			// system("pause");
		}

		return root;
	}


	/**
	* 推测出比较优的着子点
	* @param root_state 棋盘的属性
	* @param options 创建树的参数设置，包含多线程和深度
	* @return 返回最优的着子点
	*/
	template<typename State>
	int compute_move(const State root_state,
		const ComputeOptions options)
	{
		using namespace std;

		auto moves = root_state->get_moves();// 获取所有的可行走的着子点
		if (options.verbose) {
			if (moves.size() <= 0) {
				showInfoOnDOS("moves.size() is zero");
			}
			if (moves.size() == 1) {
				return moves[0];
			}
		}
	
		// 分发所有的任务去计算树——这里采用的是异步线程的方式来处理树
		vector<future<unique_ptr<Node<State>>>> root_futures;
		ComputeOptions job_options = options;
		job_options.verbose = false;
		for (int t = 0; t < options.number_of_threads; ++t) {
			auto func = [t, &root_state, &job_options]() -> std::unique_ptr<Node<State>> // 指定类型
			{
				return compute_tree(root_state, job_options, 1012411 * t + 12515);
			};
			// 使用异步线程来处理数据——分发任务
			root_futures.push_back(std::async(std::launch::async, func));
		}

		// 搜集结果
		vector<unique_ptr<Node<State>>> roots;
		for (int t = 0; t < options.number_of_threads; ++t) {
			roots.push_back(std::move(root_futures[t].get()));
		}

		// 合并所有根结点的孩子结点
		map<int, int> visits;
		map<int, double> wins;
		long long games_played = 0;
		// UTC合并加分
		for (int t = 0; t < options.number_of_threads; ++t) {
			auto root = roots[t].get();
			games_played += root->visits;
			for (auto child = root->children.cbegin(); child != root->children.cend(); ++child) {
				visits[(*child)->move] += (*child)->visits;
				wins[(*child)->move] += (*child)->wins;
			}
		}

		// 寻找最高分的结点
		double best_score = -1;
		int best_move = int();
		for (auto itr : visits) {
			auto move = itr.first;
			double v = itr.second;
			double w = wins[move];
			// Expected success rate assuming a uniform prior (Beta(1, 1)).
			// https://en.wikipedia.org/wiki/Beta_distribution
			double expected_success_rate = (w + 1) / (v + 2);
			if (expected_success_rate > best_score) {
				best_move = move;
				best_score = expected_success_rate;
			}

			// 输出UCT分数
			if (options.verbose) {
				_cprintf("MOVE: %d  (%d %%visits) (%d %%wins) \n", itr.first,
					int(100.0 * v / double(games_played) + 0.5),
					int(100.0 * w / v + 0.5));
			}
		}

		// 输出最优的UCT分数
		if (options.verbose) {
			auto best_wins = wins[best_move];
			auto best_visits = visits[best_move];
			_cprintf("---- \n Best: %d  (%d  visits) (%d  wins) \n",
				best_move,
				int(100.0 * best_visits / double(games_played)),
				int(100.0 * best_wins / best_visits));
		}

		return best_move;
	}

}

