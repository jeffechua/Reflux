#include "Engine/pch.h"
#include "../../include/Simulation/CompositeEdge.h"

namespace Reflux::Engine::Simulation {

	CompositeEdge::CompositeEdge(std::vector<BaseEdge*> constituents) : constituents(constituents) {
		inverse = std::vector<bool>(constituents.size());
		if (constituents.size() == 0) return;
		// assume first constituent is correctly inverted at construction
		// correct on binding if necessary
	}

	BaseEdge& CompositeEdge::bind(Node& from, Node& to) {
		BaseEdge::bind(from, to);
		Node* head = &from;
		for (int i = 0; i < constituents.size(); i++) {
			inverse[i] = constituents[i]->from != head;
			head = inverse[i] ? constituents[0]->from : constituents[0]->to;
		}
		return *this;
	}

	double CompositeEdge::R() const {
		double sum = 0;
		for (int i = 0; i < constituents.size(); i++) {
			sum += constituents[i]->R();
		}
		return sum;
	}

	double CompositeEdge::e() const {
		double sum = 0;
		for (int i = 0; i < constituents.size(); i++) {
			sum += constituents[i]->e() * (inverse[i] ? -1 : 1);
		}
		return sum;
	}

}