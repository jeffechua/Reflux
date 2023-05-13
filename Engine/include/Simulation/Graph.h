#pragma once
#include <vector>
#include <memory>
#include "BaseEdge.h"

namespace Reflux::Engine::Simulation {

	class Graph {

	public:
		std::vector<std::unique_ptr<Node>> nodes;
		std::vector<std::unique_ptr<BaseEdge>> edges;

		Graph(std::vector<std::unique_ptr<Node>>&& nodes, std::vector<std::unique_ptr<BaseEdge>>&& edges);
		Graph(const Graph&) = delete;
		Graph(Graph&&) = default;
		Graph& operator=(const Graph&) = delete;
		Graph& operator=(Graph&&) = default;
	};

}