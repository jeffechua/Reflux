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
	TEST_CLASS(NestedCompositesTest) {
public:

	TEST_METHOD(TestSingleBaggedEdge) {
		// Build design
		Design design{};
		CompositeUnit& bag0 = Factory::in(*design.root).create_unit<CompositeUnit>();
		ConstantUnit& unit = Factory::in(bag0).create_unit<ConstantUnit>(0.5, 9.7);
		// Build graph
		GraphBuilder builder(2, 1);
		Node& n0 = builder.add_node();
		Node& n1 = builder.add_node();
		ConstantEdge& edge = builder.add_edge<ConstantEdge>(0.5, 9.7);
		edge.bind(n0, n1);
		// Compare
		Assert::IsTrue(TestUtils::equals(builder.build(), design.build()));
	}

	TEST_METHOD(TestSingleBaggedAsymmetricTree) {
		// Build design
		Design design{};
		CompositeUnit& bag0 = Factory::in(*design.root).create_unit<CompositeUnit>();
		ConstantUnit& unit0 = Factory::in(bag0).create_unit<ConstantUnit>(0, 0);
		ConstantUnit& unit1 = Factory::in(bag0).create_unit<ConstantUnit>(0, 0);
		ConstantUnit& unit2 = Factory::in(bag0).create_unit<ConstantUnit>(0, 0);
		ConstantUnit& unit3 = Factory::in(bag0).create_unit<ConstantUnit>(0, 0);
		Junction::merge(*unit0.ports[1].junction, *unit1.ports[0].junction);
		Junction::merge(*unit1.ports[1].junction, *unit2.ports[0].junction);
		Junction::merge(*unit0.ports[1].junction, *unit3.ports[0].junction);
		Assert::IsTrue(design.root->validate(std::cerr));
		Assert::AreEqual(static_cast<size_t>(0), design.root->ports.size());
		Assert::AreEqual(static_cast<size_t>(0), bag0.ports.size());
		// Compare to graph
		Assert::IsTrue(TestUtils::equals(TestUtils::createAsymmetricTree(false), design.build()));
	}

	TEST_METHOD(TestQuintupleBaggedEdge) {
		// Build design
		Design design{};
		CompositeUnit* current = design.root;
		for (int i = 0; i < 5; i++) {
			current = &Factory::in(*current).create_unit<CompositeUnit>();
		}
		ConstantUnit& unit = Factory::in(*current).create_unit<ConstantUnit>(0.5, 9.7);
		Assert::IsTrue(design.root->validate(std::cerr));
		while (current != nullptr) {
			Assert::AreEqual(static_cast<size_t>(0), current->ports.size());
			current = current->parent;
		}
		// Build graph
		GraphBuilder builder(2, 1);
		Node& n0 = builder.add_node();
		Node& n1 = builder.add_node();
		ConstantEdge& edge = builder.add_edge<ConstantEdge>(0.5, 9.7);
		edge.bind(n0, n1);
		// Compare
		Assert::IsTrue(TestUtils::equals(builder.build(), design.build()));
	}

	TEST_METHOD(TestQuintupleBaggedAsymmetricTree) {
		// Build design
		Design design{};
		CompositeUnit* current = design.root;
		for (int i = 0; i < 5; i++) {
			current = &Factory::in(*current).create_unit<CompositeUnit>();
		}
		ConstantUnit& unit0 = Factory::in(*current).create_unit<ConstantUnit>(0, 0);
		ConstantUnit& unit1 = Factory::in(*current).create_unit<ConstantUnit>(0, 0);
		ConstantUnit& unit2 = Factory::in(*current).create_unit<ConstantUnit>(0, 0);
		ConstantUnit& unit3 = Factory::in(*current).create_unit<ConstantUnit>(0, 0);
		Junction::merge(*unit0.ports[1].junction, *unit1.ports[0].junction);
		Junction::merge(*unit1.ports[1].junction, *unit2.ports[0].junction);
		Junction::merge(*unit0.ports[1].junction, *unit3.ports[0].junction);
		Assert::IsTrue(design.root->validate(std::cerr));
		while (current != nullptr) {
			Assert::AreEqual(static_cast<size_t>(0), current->ports.size());
			current = current->parent;
		}
		// Compare to graph
		Assert::IsTrue(TestUtils::equals(TestUtils::createAsymmetricTree(false), design.build()));
	}

	};

}