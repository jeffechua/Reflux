#include "Engine/pch.h"
#include <stdexcept>
#include "../../include/Design/Factory.h"

namespace Reflux::Engine::Design::Factory {

	Junction& CreationContext::create_junction() {
		Junction& newJunction = design.make_junction();
		newJunction.parent = &container;
		container.junctions.insert(&newJunction);
		return newJunction;
	}

	CreationContext in(CompositeUnit& container) {
		if (!container.design) {
			throw std::runtime_error("Cannot create in a CompositeUnit not in a Design.");
		}
		return CreationContext(*container.design, container);
	}

}