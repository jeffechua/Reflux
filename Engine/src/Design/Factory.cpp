#include "Engine/pch.h"
#include <stdexcept>
#include "../../include/Design/Factory.h"

namespace Reflux::Engine::Design {

	Junction& Factory::CreationContext::create_junction() {
		Junction& newJunction = design.make_junction();
		newJunction.parent = &container;
		container.junctions_.insert(&newJunction);
		return newJunction;
	}

	Factory::CreationContext Factory::in(CompositeUnit& container) {
		if (!container.design) {
			throw std::runtime_error("Cannot create in a CompositeUnit not in a Design.");
		}
		return CreationContext(*container.design, container);
	}

	void Factory::destroy(BaseUnit& unit) {
		if (&unit == unit.design->root) {
			throw std::runtime_error("Cannot destroy root unit.");
		}
		// Don't need to clone since unbinding a unit port doesn't mutate its ports list
		for (Port& port : unit.ports) {
			if (port.is_bound()) {
				Junction& junction = port.unbind();
				if (junction.ports.size() == 0) {
					destroy(junction);
				}
			}
		}
		unit.parent->units_.erase(&unit);
		unit.design->recursively_unregister(&unit);
	}

	void Factory::destroy(Junction& junction) {
		if (junction.ports.size() > 0) {
			throw std::runtime_error("Cannot destroy junction with connected units.");
		}
		if (junction.isExported) {
			junction.parent->remove_export(junction);
		}
		junction.parent->junctions_.erase(&junction);
		junction.parent->design->junctions_.erase(junction.id);
	}

}