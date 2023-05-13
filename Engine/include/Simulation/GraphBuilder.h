#pragma once
//#include <forward_list>
#include "Graph.h"

namespace Reflux::Engine::Simulation {

	class GraphBuilder {

	public:
		GraphBuilder();
		GraphBuilder(size_t nodeCount, size_t edgeCount);
		GraphBuilder(const GraphBuilder&) = delete;
		GraphBuilder(GraphBuilder&&) = default;
		GraphBuilder& operator=(const GraphBuilder&) = delete;
		GraphBuilder& operator=(GraphBuilder&&) = default;

		Node& add_node();
		template<typename T, typename... TConstructorArgs>
		T& add_edge(TConstructorArgs... args);
		Graph build();

	private:
		std::vector<std::unique_ptr<Node>> nodes;
		std::vector<std::unique_ptr<BaseEdge>> edges;
		//std::forward_list<std::unique_ptr<Node>> nodes;
		//std::forward_list<std::unique_ptr<BaseEdge>> edges;

	};

	template<typename T, typename... TConstructorArgs>
	T& GraphBuilder::add_edge(TConstructorArgs... args) {
		T* ptr = new T(args...);
		edges.push_back(std::unique_ptr<BaseEdge>(ptr));
		return *ptr;
	}

}