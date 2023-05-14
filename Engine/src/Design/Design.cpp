#include "Engine/pch.h"
#include <stdexcept>
#include <iostream>
#include "../../include/Design/Design.h"
#include "../../include/Design/CompositeUnit.h"

namespace Reflux::Engine::Design {

	// PUBLIC

	Design::Design(std::string name) : name(name), nextUnitId(0), nextJunctionId(0), units_(), junctions_() {
		root = new CompositeUnit(nextUnitId++, *this);
		units_[0] = std::unique_ptr<BaseUnit>(root);
	}

	const std::unordered_map<UnitId, std::unique_ptr<BaseUnit>>& Design::get_units() const {
		return units_;
	}
	const std::unordered_map<UnitId, std::unique_ptr<Junction>>& Design::get_junctions() const {
		return junctions_;
	}

	bool Design::contains_unit(UnitId id) {
		return units_.contains(id);
	}

	bool Design::contains_junction(JunctionId id) {
		return junctions_.contains(id);
	}

	BaseUnit& Design::get_unit(UnitId id) {
		return *units_.at(id);
	}

	Junction& Design::get_junction(JunctionId id) {
		return *junctions_.at(id);
	}

	Simulation::Graph Design::build() {
		if (root->ports.size() > 0) {
			throw std::runtime_error("Tried to build design with open ports.");
		}
		Simulation::GraphBuilder graphBuilder(root->node_count(), root->edge_count());
		root->render(graphBuilder, std::vector<Simulation::Node*>{});
		return graphBuilder.build();
	}

	bool Design::validate(std::ostream& output) const {
		bool pass = true;
		for (const auto& [_, junction] : junctions_) {
			if (junction->parent == nullptr) {
				output << ("Parentless junction.") << std::endl;
				pass = false;
			}
		}
		for (const auto& [_, unit] : units_) {
			if (unit->parent == nullptr && unit.get() != root) {
				output << ("Parentless unit.") << std::endl;
				pass = false;
			}
		}
		pass |= root->validate(output);
		return pass;
	}

	// PRIVATE

	Junction& Design::make_junction() {
		Junction* ptr = new Junction(nextJunctionId++);
		junctions_.emplace(ptr->id, std::unique_ptr<Junction>(ptr));
		return *ptr;
	}

	void Design::recursively_unregister(BaseUnit* unit) {
		if (CompositeUnit* compositeUnit = dynamic_cast<CompositeUnit*>(unit)) {
			for (Junction* junction : compositeUnit->get_junctions()) {
				junctions_.erase(junction->id);
			}
			for (BaseUnit* subunit : compositeUnit->get_units()) {
				recursively_unregister(subunit);
			}
		}
		// C28182 warning if I don't gate this because it doesn't understand dynamic_cast
		if (unit) {
			units_.erase(unit->id);
		}
	}

}