#include "Engine/pch.h"
#include "../../include/Design/ConstantUnit.h"
#include <format>
#include <iostream>

namespace Reflux::Engine::Design {

	// PUBLIC

	ConstantUnit::ConstantUnit(UnitId id, Design& design, ConstantUnit::Type type, double R, double e) : BaseUnit(id, design), portsArray{ Port(*this, false), Port(*this, true) }, type(type), R(R), e(e) {}

	std::string ConstantUnit::name() const {
		/*switch (type) {
			case Type::RESISTOR:
				return std::format("%fQ Resistor (%d)", R, id);
			case Type::CELL:
				return std::format("%fQ Cell (%d)", e, id);
			case Type::RESISTIVE_CELL:
				return std::format("%fQ-%fV Resistor (%d)", R, e, id);;
		}*/
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
		switch (type) {
			case Type::RESISTOR:
				return e == 0;
			case Type::CELL:
				return R == 0;
			case Type::RESISTIVE_CELL:
				return true;
			default:
				output << std::format("Unknown ConstantUnit type \"jsadbgkalhb\"");
				return false;
		}
	}

	// PRIVATE

	size_t ConstantUnit::port_count() const {
		return 2;
	}

	Port& ConstantUnit::get_port(size_t i) {
		return portsArray[i];
	}

}