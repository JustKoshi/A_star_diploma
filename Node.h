#pragma once

#include <cmath>
#include <algorithm>
#include <stdexcept>

enum class heuristicType { manhattan, euclidean, chebyshev, dijkstra};


struct Node {
	int x, y;
	float g, h;
	Node* parent;
	heuristicType heuristic;
	Node(int x, int y, float g, float h, Node* parent,heuristicType heuristic) : x(x), y(y), g(g), h(h), parent(parent), heuristic(heuristic) {}
	Node(int x, int y) : x(x), y(y), g(0), h(0), parent(nullptr), heuristic(heuristicType::manhattan) {}
	Node() : x(0), y(0), g(0), h(0), parent(nullptr), heuristic(heuristicType::manhattan) {}
	
	float getfcost() const {
		return g + h;
	}

	static float heuristic_def(int x1, int y1, int x2, int y2, heuristicType type) {
		switch (type) {
		case heuristicType::manhattan:
			return std::abs(x1 - x2) + std::abs(y1 - y2);
		case heuristicType::euclidean:
			return std::round(std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2)));
		case heuristicType::chebyshev:
			return std::max(std::abs(x1 - x2), std::abs(y1 - y2));
		case heuristicType::dijkstra:
			return 0;
		default:
			throw std::invalid_argument("Invalid heuristic type");
		}
	}

	bool operator==(const Node& other) const {
		return x == other.x && y == other.y;
	}
};

struct NodeCompare {
	bool operator()(const Node* a, const Node* b) const {
		return a->getfcost() > b->getfcost();
	}
};