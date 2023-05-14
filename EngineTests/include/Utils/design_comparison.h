#pragma once

#include "EngineTests/pch.h"
#include "Engine/include/Design/CompositeUnit.h"

using namespace Reflux::Engine::Design;

namespace Reflux::Engine::TestUtils {

	void validate_and_assert_composite_unit_counts(CompositeUnit& unit, int portCount, int unitCount, int junctionCount);

	void assert_ports_are_bound_to_unordered(BaseUnit::Ports ports, std::unordered_set<Junction*> junctions_);

	void assert_ports_are_bound_to_ordered(BaseUnit::Ports ports, std::vector<Junction*> junctions_);

}