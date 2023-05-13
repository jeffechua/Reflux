#pragma once
#include <unordered_set>

namespace Reflux::Engine::Design {

	struct Port;
	class CompositeUnit;

	typedef unsigned int JunctionId;

	class Junction {

	public:
		const JunctionId id;
		CompositeUnit* parent;
		std::unordered_set<Port*> ports;
		Junction(JunctionId id);

	};

}