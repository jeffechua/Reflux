#include "Engine/pch.h"
#include <stdexcept>
#include "../../include/Design/Design.h"
#include "../../include/Design/CompositeUnit.h"

namespace Reflux::Engine::Design {

	// PUBLIC

	Design::Design(std::string name) : name(name), nextUnitId(0), nextJunctionId(0), units(), junctions() {
		root = new CompositeUnit(nextUnitId++, *this);
		units[0] = std::unique_ptr<BaseUnit>(root);
	}

	Junction& Design::create_junction() {
		Junction* ptr = new Junction(nextJunctionId++);
		junctions.emplace(ptr->id, std::unique_ptr<Junction>(ptr));
		return *ptr;
	}

	void Design::destroy(BaseUnit& unit) {
		if (&unit == root) {
			throw std::runtime_error("Cannot destroy root unit.");
		}
		// Don't need to clone since unbinding a unit port doesn't mutate its ports list
		for (Port& port : unit.ports) {
			if (port.is_bound()) {
				port.unbind(false);
			} else if (unit.parent) {
				unit.parent->notify_removed_unbound_internal_port(port);
			}
		}
		if (unit.parent) {
			unit.parent->units.erase(&unit);
		}
		recursively_unregister(&unit);
	}

	void Design::destroy(Junction& junction) {
		// Need to clone ports since unbinding junction ports mutates junction.ports
		std::vector<Port*> ports{ junction.ports.begin(), junction.ports.end() };
		for (Port* port : ports) {
			port->unbind();
		}
		if (junction.parent) {
			junction.parent->junctions.erase(&junction);
		}
		junctions.erase(junction.id);
	}

	bool Design::contains_unit(UnitId id) {
		return units.contains(id);
	}

	bool Design::contains_junction(JunctionId id) {
		return junctions.contains(id);
	}

	BaseUnit& Design::get_unit(UnitId id) {
		return *units.at(id);
	}

	Junction& Design::get_junction(JunctionId id) {
		return *junctions.at(id);
	}

	Simulation::Graph Design::build() {
		if (root->ports.size() > 0) {
			throw std::runtime_error("Tried to build design with open ports.");
		}
		Simulation::GraphBuilder graphBuilder(root->node_count(), root->edge_count());
		root->render(graphBuilder, std::vector<Simulation::Node*>{});
		return graphBuilder.build();
	}

	// PRIVATE

	void Design::recursively_unregister(BaseUnit* unit) {
		if (CompositeUnit* compositeUnit = dynamic_cast<CompositeUnit*>(unit)) {
			for (Junction* junction : compositeUnit->junctions) {
				junctions.erase(junction->id);
			}
			for (BaseUnit* subunit : compositeUnit->units) {
				recursively_unregister(subunit);
			}
		}
		// C28182 warning if I don't gate this because it doesn't understand dynamic_cast
		if (unit) {
			units.erase(unit->id);
		}
	}

}