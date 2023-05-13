#include "Engine/pch.h"
#include <stdexcept>
#include "../../include/Simulation/BaseEdge.h"

namespace Reflux::Engine::Simulation {

	bool BaseEdge::is_bound() const {
		return this->from != nullptr;
	}

	BaseEdge& BaseEdge::bind(Node& from, Node& to) {
		if (is_bound()) {
			throw std::runtime_error("Edge is already bound");
		}
		from.outgoing.push_back(this);
		to.incoming.push_back(this);
		this->from = &from;
		this->to = &to;
		return *this;
	}

}