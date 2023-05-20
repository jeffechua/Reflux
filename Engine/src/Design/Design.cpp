#include "Engine/pch.h"
#include <stdexcept>
#include <iostream>
#include "../../include/Design/Design.h"
#include "../../include/Design/CompositeUnit.h"

namespace Reflux::Engine::Design {

	// PUBLIC

	Design::Design(std::string name) : name(name), nextUnitId(0), nextJunctionId(0), units(), junctions() {
		root = new CompositeUnit(nextUnitId++, *this);
		units[0] = std::unique_ptr<BaseUnit>(root);
	}

	bool Design::contains_unit(UnitId id_) {
		return units.contains(id_);
	}

	bool Design::contains_junction(JunctionId id_) {
		return junctions.contains(id_);
	}

	BaseUnit& Design::get_unit(UnitId id_) {
		return *units.at(id_);
	}

	Junction& Design::get_junction(JunctionId id_) {
		return *junctions.at(id_);
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
		for (const auto& [_, junction] : junctions) {
			if (junction->parent == nullptr) {
				output << ("Parentless junction.") << std::endl;
				pass = false;
			}
		}
		for (const auto& [_, unit] : units) {
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
		junctions.emplace(ptr->id_, std::unique_ptr<Junction>(ptr));
		return *ptr;
	}

	void Design::recursively_unregister(BaseUnit* unit) {
		if (CompositeUnit* compositeUnit = dynamic_cast<CompositeUnit*>(unit)) {
			for (Junction* junction : compositeUnit->junctions) {
				junctions.erase(junction->id_);
			}
			for (BaseUnit* subunit : compositeUnit->units) {
				recursively_unregister(subunit);
			}
		}
		// C28182 warning if I don't gate this because it doesn't understand dynamic_cast
		if (unit) {
			units.erase(unit->id_);
		}
	}

}