#pragma once
#include "Junction.h"

namespace Reflux::Engine::Design {

	class BaseUnit;

	struct Port {

		Port(const Port&) = delete;
		Port(Port&&) = default;
		Port& operator=(const Port&) = delete;
		Port& operator=(Port&&) = default;

		BaseUnit& unit;
		Junction* junction;
		bool outgoing;
		Port(BaseUnit& unit, bool outgoing);
		void set_internal_position(Geometry::vector2<int> position);
		void set_local_position(Geometry::vector2<int> position);
		void set_world_position(Geometry::vector2<int> position);
		Geometry::vector2<int> get_internal_position();
		Geometry::vector2<int> get_local_position();
		Geometry::vector2<int> get_world_position();
		bool is_bound() const;
		Junction& detach();
		void bind(Junction& to);
		Junction& unbind();

	private:
		Geometry::vector2<int> internalPosition_;

	};

}