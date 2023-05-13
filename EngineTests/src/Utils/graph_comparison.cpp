#include "EngineTests/pch.h"
#include "../../include/Utils/graph_comparison.h"
#include "../../include/Utils/mapstack.h"

using namespace Reflux::Engine::Simulation;

namespace Reflux::Engine::TestUtils {

	bool equals(BaseEdge* e1, BaseEdge* e2) {
		return e1->R() == e2->R() && e1->e() == e2->e();
	}

	bool equals(Node& n1, Node& n2, mapstack<Node*, Node*>& explored) {
		if (explored.contains(&n2)) {
			return &n1 == explored[&n2];
		}
		if (n1.incoming.size() != n2.incoming.size() || n1.outgoing.size() != n2.outgoing.size()) {
			return false;
		}
		size_t baseline = explored.size();
		explored.emplace(&n2, &n1);
		std::vector<bool> n2EdgesMatched(n2.incoming.size(), false);
		for (auto e1 : n1.incoming) {
			bool n1EdgeMatched = false;
			for (int i2 = 0; i2 < n2.incoming.size(); i2++) {
				auto e2 = n2.incoming[i2];
				// (C)ounter(p)arty of the edges
				// Note: the equals(e1cp, e2cp) may mutate the explored mapstack.
				if (!n2EdgesMatched[i2] && equals(e1, e2) && equals(*e1->from, *e2->from, explored)) {
					n1EdgeMatched = true;
					n2EdgesMatched[i2] = true;
					break;
				}
			}
			if (!n1EdgeMatched) {
				explored.pop_to(baseline);
				return false;
			}
		}
		n2EdgesMatched = std::vector<bool>(n2.outgoing.size(), false);
		for (auto e1 : n1.outgoing) {
			bool n1EdgeMatched = false;
			for (int i2 = 0; i2 < n2.outgoing.size(); i2++) {
				auto e2 = n2.outgoing[i2];
				// Note: the equals(e1cp, e2cp) may mutate the explored mapstack.
				if (!n2EdgesMatched[i2] && equals(e1, e2) && equals(*e1->to, *e2->to, explored)) {
					n1EdgeMatched = true;
					n2EdgesMatched[i2] = true;
					break;
				}
			}
			if (!n1EdgeMatched) {
				explored.pop_to(baseline);
				return false;
			}
		}
		return true;
	}

	bool equals(const Graph& g1, const Graph& g2) {
		if (g1.nodes.size() != g2.nodes.size() || g1.edges.size() != g2.edges.size()) {
			return false;
		}

		if (g1.nodes.size() == 0) {
			return true;
		}

		// Key: g1 node
		// Value: corresponding g2 node
		mapstack<Node*, Node*> explored(g1.nodes.size());
		for (auto& n1 : g1.nodes) {
			for (auto& n2 : g2.nodes) {
				if (equals(*n1, *n2, explored)) {
					return true;
				}
			}
		}
		return false;
	}

}