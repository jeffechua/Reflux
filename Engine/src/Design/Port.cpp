#include "Engine/pch.h"
#include <stdexcept>
#include "../../include/Design/Port.h"
#include "../../include/Design/CompositeUnit.h"
#include "../../include/Design/Factory.h"

namespace Reflux::Engine::Design {

	Port::Port(BaseUnit& unit, bool outgoing) : unit(unit), junction(nullptr), outgoing(outgoing) {}

	void Port::set_internal_position(Geometry::vector2<int> position) {
		internalPosition_ = position;
		if (unit.parent) {
			unit.parent->child_geometry_update(unit);
		}
	}

	void Port::set_local_position(Geometry::vector2<int> position) {
		set_internal_position(position - unit.get_local_position());
	}

	void Port::set_world_position(Geometry::vector2<int> position) {
		set_internal_position(position - unit.get_world_position());
	}

	Geometry::vector2<int> Port::get_internal_position() {
		return internalPosition_;
	}

	Geometry::vector2<int> Port::get_local_position() {
		return internalPosition_ + unit.get_local_position();
	}

	Geometry::vector2<int> Port::get_world_position() {
		return internalPosition_ + unit.get_world_position();
	}

	bool Port::is_bound() const {
		return junction != nullptr;
	}

	Junction& Port::detach() {
		if (is_bound()) {
			unbind();
		}
		Junction& junction = Factory::in(*unit.parent).create_junction();
		bind(junction);
		return junction;
	}

	void Port::bind(Junction& to) {
		if (is_bound()) {
			throw new std::runtime_error("Port is already bound.");
		}
		if (to.parent != unit.parent) {
			throw new std::runtime_error("Port can only bind to siblings.");
		}
		junction = &to;
		to.ports.insert(this);
	}

	Junction& Port::unbind() {
		if (!is_bound()) {
			throw new std::runtime_error("Port is already unbound.");
		}
		junction->ports.erase(this);
		Junction& junctionRef = *junction;
		junction = nullptr;
		return junctionRef;
	}

}