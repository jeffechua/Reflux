#include "Engine/pch.h"
#include <stdexcept>
#include "../../include/Design/Port.h"
#include "../../include/Design/CompositeUnit.h"

namespace Reflux::Engine::Design {

	Port::Port(BaseUnit& unit, bool outgoing) : unit(unit), junction(nullptr), outgoing(outgoing) {}

	bool Port::is_bound() const {
		return junction != nullptr;
	}

	void Port::bind(Junction& to, bool notify) {
		if (is_bound()) {
			throw new std::runtime_error("Port is already bound.");
		}
		if (to.parent != unit.parent) {
			throw new std::runtime_error("Port can only bind to siblings.");
		}
		junction = &to;
		to.ports.insert(this);
		if (notify && unit.parent) {
			unit.parent->notify_removed_unbound_internal_port(*this);
		}
	}

	Junction& Port::unbind(bool notify) {
		if (!is_bound()) {
			throw new std::runtime_error("Port is already unbound.");
		}
		junction->ports.erase(this);
		Junction& oldJunction = *junction;
		junction = nullptr;
		if (notify && unit.parent) {
			unit.parent->notify_new_unbound_internal_port(*this);
		}
		return oldJunction;
	}

}