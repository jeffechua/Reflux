#pragma once
#include <vector>

namespace Reflux::Engine::Simulation {

	class BaseEdge;

	class Node {

	public:
		std::vector<BaseEdge*> incoming;
		std::vector<BaseEdge*> outgoing;
		Node();

	};

}