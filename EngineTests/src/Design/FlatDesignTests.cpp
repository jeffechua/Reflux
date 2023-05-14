#include "EngineTests/pch.h"
#include <iostream>
#include <CppUnitTest.h>
#include "Engine/include/Design/Design.h"
#include "Engine/include/Design/CompositeUnit.h"
#include "Engine/include/Design/ConstantUnit.h"
#include "Engine/include/Design/Factory.h"
#include "Engine/include/Simulation/Graph.h"
#include "../../include/Utils/graph_comparison.h"
#include "../../include/Utils/pointer_comparison.h"
#include "../../include/Utils/graph_factory.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Reflux::Engine::Design::Tests {
	TEST_CLASS(FlatDesignTests) {
public:

	TEST_METHOD(TestOneEdge) {
		Design design{};
		ConstantUnit& unit = Factory::in(*design.root).create_unit<ConstantUnit>(0.5, 9.7);
		Assert::IsTrue(design.root->validate(std::cerr));
		Assert::AreEqual(static_cast<size_t>(0), design.root->ports.size());
		GraphBuilder builder(2, 1);
		Node& n0 = builder.add_node();
		Node& n1 = builder.add_node();
		ConstantEdge& edge = builder.add_edge<ConstantEdge>(0.5, 9.7);
		edge.bind(n0, n1);
		Assert::IsTrue(TestUtils::equals(builder.build(), design.build()));
	}

	TEST_METHOD(TestTwoEdges) {
		Design design{};
		ConstantUnit& unit0 = Factory::in(*design.root).create_unit<ConstantUnit>(0, 0);
		ConstantUnit& unit1 = Factory::in(*design.root).create_unit<ConstantUnit>(0, 0);
		Junction::merge(*unit0.ports[1].junction, *unit1.ports[0].junction);
		Assert::IsTrue(design.root->validate(std::cerr));
		Assert::AreEqual(static_cast<size_t>(0), design.root->ports.size());
		Assert::IsTrue(TestUtils::equals(TestUtils::createTwoEdges(false), design.build()));
	}

	TEST_METHOD(TestTwoEdgesConverging) {
		Design design{};
		ConstantUnit& unit0 = Factory::in(*design.root).create_unit<ConstantUnit>(0, 0);
		ConstantUnit& unit1 = Factory::in(*design.root).create_unit<ConstantUnit>(0, 0);
		Junction::merge(*unit0.ports[1].junction, *unit1.ports[1].junction);
		Assert::IsTrue(design.root->validate(std::cerr));
		Assert::AreEqual(static_cast<size_t>(0), design.root->ports.size());
		Assert::IsTrue(TestUtils::equals(TestUtils::createTwoEdgesConverging(false), design.build()));
	}

	TEST_METHOD(TestTriangle) {
		Design design{};
		ConstantUnit& unit0 = Factory::in(*design.root).create_unit<ConstantUnit>(0, 0);
		ConstantUnit& unit1 = Factory::in(*design.root).create_unit<ConstantUnit>(0, 0);
		ConstantUnit& unit2 = Factory::in(*design.root).create_unit<ConstantUnit>(0, 0);
		Junction::merge(*unit0.ports[1].junction, *unit1.ports[0].junction);
		Junction::merge(*unit1.ports[1].junction, *unit2.ports[0].junction);
		Junction::merge(*unit2.ports[1].junction, *unit0.ports[0].junction);
		Assert::IsTrue(design.root->validate(std::cerr));
		Assert::AreEqual(static_cast<size_t>(0), design.root->ports.size());
		Assert::IsTrue(TestUtils::equals(TestUtils::createTriangle(false), design.build()));
	}

	TEST_METHOD(TestSymmetricTree) {
		Design design{};
		ConstantUnit& unit0 = Factory::in(*design.root).create_unit<ConstantUnit>(0, 0);
		ConstantUnit& unit1 = Factory::in(*design.root).create_unit<ConstantUnit>(0, 0);
		ConstantUnit& unit2 = Factory::in(*design.root).create_unit<ConstantUnit>(0, 0);
		ConstantUnit& unit3 = Factory::in(*design.root).create_unit<ConstantUnit>(0, 0);
		Junction::merge(*unit0.ports[1].junction, *unit1.ports[0].junction);
		Junction::merge(*unit0.ports[1].junction, *unit2.ports[0].junction);
		Junction::merge(*unit0.ports[1].junction, *unit3.ports[0].junction);
		Assert::IsTrue(design.root->validate(std::cerr));
		Assert::AreEqual(static_cast<size_t>(0), design.root->ports.size());
		Assert::IsTrue(TestUtils::equals(TestUtils::createSymmetricTree(false), design.build()));
	}

	TEST_METHOD(TestAsymmetricTree) {
		Design design{};
		ConstantUnit& unit0 = Factory::in(*design.root).create_unit<ConstantUnit>(0, 0);
		ConstantUnit& unit1 = Factory::in(*design.root).create_unit<ConstantUnit>(0, 0);
		ConstantUnit& unit2 = Factory::in(*design.root).create_unit<ConstantUnit>(0, 0);
		ConstantUnit& unit3 = Factory::in(*design.root).create_unit<ConstantUnit>(0, 0);
		Junction::merge(*unit0.ports[1].junction, *unit1.ports[0].junction);
		Junction::merge(*unit1.ports[1].junction, *unit2.ports[0].junction);
		Junction::merge(*unit0.ports[1].junction, *unit3.ports[0].junction);
		Assert::IsTrue(design.root->validate(std::cerr));
		Assert::AreEqual(static_cast<size_t>(0), design.root->ports.size());
		Assert::IsTrue(TestUtils::equals(TestUtils::createAsymmetricTree(false), design.build()));
	}

	};

}