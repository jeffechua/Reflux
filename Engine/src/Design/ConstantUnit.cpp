#include "Engine/pch.h"
#include "../../include/Design/ConstantUnit.h"
#include <format>
#include <iostream>

namespace Reflux::Engine::Design {

	// PUBLIC

	ConstantUnit::ConstantUnit(UnitId id, Design& design, double R, double e) : BaseUnit(id, design), portsArray{ Port(*this, false), Port(*this, true) }, R(R), e(e) {}

	std::string ConstantUnit::name() const {
		return std::to_string(id);
	}

	int ConstantUnit::node_count() const {
		return 0;
	}

	int ConstantUnit::edge_count() const {
		return 1;
	}

	void ConstantUnit::render(Simulation::GraphBuilder& graphBuilder, const std::vector<Simulation::Node*>& bindings) const {
		graphBuilder.add_edge<Simulation::ConstantEdge>(R, e).bind(*bindings[0], *bindings[1]);
	}

	bool ConstantUnit::validate(std::ostream& output) const {
		return true;
	}

	// PRIVATE

	size_t ConstantUnit::port_count_() const {
		return 2;
	}

	Port& ConstantUnit::get_port_(size_t i) {
		return portsArray[i];
	}

}