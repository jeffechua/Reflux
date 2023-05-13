#include "Engine/pch.h"
#include "../../include/Design/Junction.h"

namespace Reflux::Engine::Design {

	Junction::Junction(JunctionId id) : id(id), parent(nullptr) {}

}