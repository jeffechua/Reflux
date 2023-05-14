#include "EngineTests/pch.h"
#include <iostream>
#include <CppUnitTest.h>
#include "../../include/Utils/design_comparison.h"

using namespace Reflux::Engine::Simulation;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Reflux::Engine::TestUtils {

	void validate_and_assert_composite_unit_counts(CompositeUnit& unit, int portCount, int unitCount, int junctionCount) {
		std::stringstream stream;
		bool valid = unit.validate(stream);
;		Logger::WriteMessage(stream.str().c_str());
		Assert::IsTrue(valid);
		Assert::AreEqual(static_cast<size_t>(portCount), unit.ports.size());
		Assert::AreEqual(static_cast<size_t>(unitCount), unit.get_units().size());
		Assert::AreEqual(static_cast<size_t>(junctionCount), unit.get_junctions().size());
	}

	void assert_ports_are_bound_to_unordered(BaseUnit::Ports ports, std::unordered_set<Junction*> junctions_) {
		std::unordered_set<Junction*> portJunctions{};
		for (Port& port : ports) {
			portJunctions.emplace(port.junction);
		}
		Assert::IsTrue(portJunctions == junctions_);
	}

	void assert_ports_are_bound_to_ordered(BaseUnit::Ports ports, std::vector<Junction*> junctions_) {
		std::vector<Junction*> portJunctions{};
		portJunctions.reserve(junctions_.size());
		for (Port& port : ports) {
			portJunctions.push_back(port.junction);
		}
		Assert::IsTrue(portJunctions == junctions_);
	}

}