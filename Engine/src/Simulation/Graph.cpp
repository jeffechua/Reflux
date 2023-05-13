#include "Engine/pch.h"
#include "../../include/Simulation/Graph.h"

namespace Reflux::Engine::Simulation {

	Graph::Graph(std::vector<std::unique_ptr<Node>>&& nodes, std::vector<std::unique_ptr<BaseEdge>>&& edges) : nodes(std::move(nodes)), edges(std::move(edges)) {

	}

}