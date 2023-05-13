#pragma once
#include "Node.h"

namespace Reflux::Engine::Simulation {

	class BaseEdge {

	public:
		bool is_bound() const;
		virtual BaseEdge& bind(Node& from, Node& to);
		Node* from = nullptr;
		Node* to = nullptr;
		virtual double R() const = 0;
		virtual double e() const = 0;

	};

}