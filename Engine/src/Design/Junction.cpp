#include "Engine/pch.h"
#include <stdexcept>
#include "../../include/Design/Junction.h"
#include "../../include/Design/CompositeUnit.h"

namespace Reflux::Engine::Design {

	Junction::Junction(JunctionId id) : id(id), parent(nullptr), isExported(false) {}

	Junction& Junction::merge(Junction& first_in, Junction& second_in) {
		bool flip = (second_in.isExported && !first_in.isExported);
		Junction& first = flip ? second_in : first_in;
		Junction& second = flip ? first_in : second_in;
		CompositeUnit& parent = *first.parent;
		if (second.parent != first.parent) {
			throw std::runtime_error("Junctions do not have the same parent.");
		}
		// If second is exported, first is exported
		if (second.isExported) {
			Junction& firstExternalJunction = *parent.get_export(first).junction;
			Junction& secondExternalJunction = *parent.get_export(second).junction;
			for (Port* port : secondExternalJunction.ports) {
				port->unbind();
				port->bind(firstExternalJunction);
			}
			parent.remove_export(second);
		}
		std::vector<Port*> ports{ second.ports.begin(), second.ports.end() };
		for (Port* port : ports) {
			port->unbind();
			port->bind(first);
		}
		parent.design->destroy(second);
		return first;
	}

}