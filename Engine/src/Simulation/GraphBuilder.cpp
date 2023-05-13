#include "Engine/pch.h"
#include "../../include/Simulation/GraphBuilder.h"

namespace Reflux::Engine::Simulation {

	GraphBuilder::GraphBuilder() : nodes(), edges() {}

	GraphBuilder::GraphBuilder(size_t nodeCount, size_t edgeCount) : nodes(), edges() {
		nodes.reserve(nodeCount);
		edges.reserve(edgeCount);
	}

	Node& GraphBuilder::add_node() {
		nodes.push_back(std::make_unique<Node>());
		return *nodes.back();
	}

	Graph GraphBuilder::build() {
		return Graph(std::move(nodes), std::move(edges));
		//return Graph(
		//	{
		//		std::make_move_iterator(nodes.begin()),
		//		std::make_move_iterator(nodes.end())
		//	},
		//	{
		//		std::make_move_iterator(edges.begin()),
		//		std::make_move_iterator(edges.end())
		//	}
		//);
	}

}