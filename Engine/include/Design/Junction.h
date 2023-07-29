#pragma once
#include <unordered_set>
#include "../Geometry/vector2.h"

namespace Reflux::Engine::Design {

	struct Port;
	class CompositeUnit;

	typedef unsigned int JunctionId;

	class Junction {

	public:
		const JunctionId id_;
		CompositeUnit* parent;
		bool isExported;
		std::unordered_set<Port*> ports;
		Junction(JunctionId id_);
		Geometry::vector2<int> get_local_position();
		Geometry::vector2<int> get_world_position();
		static Junction& merge(Junction& first, Junction& second);

	};

}