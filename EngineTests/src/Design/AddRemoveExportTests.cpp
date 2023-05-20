#include "EngineTests/pch.h"
#include <iostream>
#include <CppUnitTest.h>
#include "Engine/include/Design/Design.h"
#include "Engine/include/Design/ConstantUnit.h"
#include "Engine/include/Design/Factory.h"
#include "../../include/Utils/design_comparison.h"
#include "../../include/Utils/graph_comparison.h"
#include "../../include/Utils/pointer_comparison.h"
#include "../../include/Utils/graph_factory.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Reflux::Engine::Design::Tests {

	TEST_CLASS(AddRemoveExportTests) {

public:

	TEST_METHOD(AddExportTest) {
		Design design{};
		CompositeUnit& group = Factory::in(*design.root).create_unit<CompositeUnit>();
		ConstantUnit& edge = Factory::in(group).create_unit<ConstantUnit>(5, 5);
		group.add_export(*edge.ports[0].junction);
		Assert::AreEqual(static_cast<size_t>(3), design.junctions.size());
		Assert::AreEqual(2, design.root->node_count());
	}

	TEST_METHOD(RemoveExportTest) {
		Design design{};
		ConstantUnit& edge0 = Factory::in(*design.root).create_unit<ConstantUnit>(5, 5);
		ConstantUnit& edge1 = Factory::in(*design.root).create_unit<ConstantUnit>(5, 5);
		Junction::merge(*edge0.ports[1].junction, *edge1.ports[0].junction);
		CompositeUnit& group = Factory::in(*design.root).create_unit<CompositeUnit>();
		group.push({ &edge0 });
		Assert::AreEqual(static_cast<size_t>(4), design.junctions.size());
		Assert::AreEqual(3, design.root->node_count());
		group.remove_export(*edge0.ports[1].junction);
		Assert::AreEqual(static_cast<size_t>(4), design.junctions.size());
		Assert::AreEqual(4, design.root->node_count());
	}

	TEST_METHOD(RemoveNestedExportTest) {
		Design design{};
		ConstantUnit& edge0 = Factory::in(*design.root).create_unit<ConstantUnit>(5, 5);
		ConstantUnit& edge1 = Factory::in(*design.root).create_unit<ConstantUnit>(5, 5);
		Junction::merge(*edge0.ports[1].junction, *edge1.ports[0].junction);
		CompositeUnit& group0 = Factory::in(*design.root).create_unit<CompositeUnit>();
		group0.push({ &edge0 });
		CompositeUnit& group1 = Factory::in(group0).create_unit<CompositeUnit>();
		group1.push({ &edge0 });
		Assert::AreEqual(static_cast<size_t>(5), design.junctions.size());
		Assert::AreEqual(3, design.root->node_count());
		Assert::AreEqual(static_cast<size_t>(1), group0.ports.size());
		Assert::AreEqual(static_cast<size_t>(1), group1.ports.size());
		group1.remove_export(*edge0.ports[1].junction);
		Assert::AreEqual(static_cast<size_t>(4), design.junctions.size());
		Assert::AreEqual(4, design.root->node_count());
		Assert::AreEqual(static_cast<size_t>(0), group0.ports.size());
		Assert::AreEqual(static_cast<size_t>(0), group1.ports.size());
	}

	};

}