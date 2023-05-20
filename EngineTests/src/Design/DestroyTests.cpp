#include "EngineTests/pch.h"
#include <CppUnitTest.h>
#include "Engine/include/Design/Design.h"
#include "Engine/include/Design/CompositeUnit.h"
#include "Engine/include/Design/ConstantUnit.h"
#include "Engine/include/Design/Factory.h"
#include "Engine/include/Simulation/Graph.h"
#include "../../include/Utils/design_comparison.h"
#include "../../include/Utils/graph_comparison.h"
#include "../../include/Utils/pointer_comparison.h"
#include "../../include/Utils/graph_factory.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Reflux::Engine::Design::Tests {

	TEST_CLASS(DestroyTests) {

public:

	TEST_METHOD(DestroyEdge) {
		Design design{};
		ConstantUnit& edge = Factory::in(*design.root).create_unit<ConstantUnit>(0, 0);
		TestUtils::validate_and_assert_composite_unit_counts(*design.root, 0, 1, 2);
		Factory::destroy(edge);
		TestUtils::validate_and_assert_composite_unit_counts(*design.root, 0, 0, 0);
	}

	TEST_METHOD(DestroyOneOfTwoEdges) {
		Design design{};
		ConstantUnit& edge0 = Factory::in(*design.root).create_unit<ConstantUnit>(0, 0);
		ConstantUnit& edge1 = Factory::in(*design.root).create_unit<ConstantUnit>(0, 0);
		Junction::merge(*edge0.ports[1].junction, *edge1.ports[0].junction);
		TestUtils::validate_and_assert_composite_unit_counts(*design.root, 0, 2, 3);
		Factory::destroy(edge0);
		TestUtils::validate_and_assert_composite_unit_counts(*design.root, 0, 1, 2);
	}

	TEST_METHOD(DestroyNestedOneOfTwoEdges) {
		Design design{};
		ConstantUnit& edge0 = Factory::in(*design.root).create_unit<ConstantUnit>(0, 0);
		ConstantUnit& edge1 = Factory::in(*design.root).create_unit<ConstantUnit>(0, 0);
		Junction::merge(*edge0.ports[1].junction, *edge1.ports[0].junction);
		CompositeUnit& group = Factory::in(*design.root).create_unit<CompositeUnit>();
		group.push({ &edge0 });
		TestUtils::validate_and_assert_composite_unit_counts(*design.root, 0, 2, 2);
		TestUtils::validate_and_assert_composite_unit_counts(group, 1, 1, 2);
		Factory::destroy(edge0);
		TestUtils::validate_and_assert_composite_unit_counts(*design.root, 0, 2, 2);
		TestUtils::validate_and_assert_composite_unit_counts(group, 0, 0, 0);
	}

	TEST_METHOD(DestroyNestedRootOneOfTwoEdges) {
		Design design{};
		ConstantUnit& edge0 = Factory::in(*design.root).create_unit<ConstantUnit>(0, 0);
		ConstantUnit& edge1 = Factory::in(*design.root).create_unit<ConstantUnit>(0, 0);
		Junction::merge(*edge0.ports[1].junction, *edge1.ports[0].junction);
		CompositeUnit& group = Factory::in(*design.root).create_unit<CompositeUnit>();
		group.push({ &edge0 });
		TestUtils::validate_and_assert_composite_unit_counts(*design.root, 0, 2, 2);
		TestUtils::validate_and_assert_composite_unit_counts(group, 1, 1, 2);
		Factory::destroy(group);
		TestUtils::validate_and_assert_composite_unit_counts(*design.root, 0, 1, 2);
		Assert::AreEqual(static_cast<size_t>(2), design.units.size());
		Assert::AreEqual(static_cast<size_t>(2), design.junctions.size());
	}

	};

}