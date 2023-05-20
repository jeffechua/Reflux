#pragma once
#include <unordered_set>

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
		static Junction& merge(Junction& first, Junction& second);
	};

}