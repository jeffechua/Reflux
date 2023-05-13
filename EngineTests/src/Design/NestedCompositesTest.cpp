#include "EngineTests/pch.h"
#include <iostream>
#include <CppUnitTest.h>
#include "Engine/include/Design/Design.h"
#include "Engine/include/Design/CompositeUnit.h"
#include "Engine/include/Design/ConstantUnit.h"
#include "Engine/include/Simulation/Graph.h"
#include "../../include/Utils/graph_comparison.h"
#include "../../include/Utils/pointer_comparison.h"
#include "../../include/Utils/graph_factory.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Reflux::Engine::Design::Tests {
	TEST_CLASS(NestedCompositesTest) {
public:

	TEST_METHOD(TestSingleBaggedNode) {
		// Build design
		Design design{};
		CompositeUnit& bag0 = design.root->create<CompositeUnit>();
		Junction& junction = bag0.create_junction();
		Assert::IsTrue(design.root->validate(std::cerr));
		Assert::AreEqual(static_cast<size_t>(0), design.root->ports.size());
		Assert::AreEqual(static_cast<size_t>(0), bag0.ports.size());
		// Compare to graph
		Assert::IsTrue(TestUtils::equals(TestUtils::createOneNode(false), design.build()));
	}

	TEST_METHOD(TestSingleBaggedEdge) {
		// Build design
		Design design{};
		CompositeUnit& bag0 = design.root->create<CompositeUnit>();
		Junction& j0 = bag0.create_junction();
		Junction& j1 = bag0.create_junction();
		ConstantUnit& unit = bag0.create<ConstantUnit>(ConstantUnit::RESISTIVE_CELL, 0.5, 9.7);
		Assert::IsTrue(design.root->validate(std::cerr));
		Assert::AreEqual(static_cast<size_t>(2), design.root->ports.size());
		Assert::AreEqual(static_cast<size_t>(2), bag0.ports.size());
		unit.ports[0].bind(j0);
		unit.ports[1].bind(j1);
		Assert::IsTrue(design.root->validate(std::cerr));
		Assert::AreEqual(static_cast<size_t>(0), design.root->ports.size());
		Assert::AreEqual(static_cast<size_t>(0), bag0.ports.size());
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
		CompositeUnit& bag0 = design.root->create<CompositeUnit>();
		Junction& j0 = bag0.create_junction();
		Junction& j1 = bag0.create_junction();
		Junction& j2 = bag0.create_junction();
		Junction& j3 = bag0.create_junction();
		Junction& j4 = bag0.create_junction();
		ConstantUnit& unit0 = bag0.create<ConstantUnit>(ConstantUnit::RESISTIVE_CELL, 0, 0);
		unit0.ports[0].bind(j0);
		unit0.ports[1].bind(j1);
		ConstantUnit& unit1 = bag0.create<ConstantUnit>(ConstantUnit::RESISTIVE_CELL, 0, 0);
		unit1.ports[0].bind(j1);
		unit1.ports[1].bind(j2);
		ConstantUnit& unit2 = bag0.create<ConstantUnit>(ConstantUnit::RESISTIVE_CELL, 0, 0);
		unit2.ports[0].bind(j2);
		unit2.ports[1].bind(j3);
		ConstantUnit& unit3 = bag0.create<ConstantUnit>(ConstantUnit::RESISTIVE_CELL, 0, 0);
		unit3.ports[0].bind(j1);
		unit3.ports[1].bind(j4);
		Assert::IsTrue(design.root->validate(std::cerr));
		Assert::AreEqual(static_cast<size_t>(0), design.root->ports.size());
		Assert::AreEqual(static_cast<size_t>(0), bag0.ports.size());
		// Compare to graph
		Assert::IsTrue(TestUtils::equals(TestUtils::createAsymmetricTree(false), design.build()));
	}

	TEST_METHOD(TestQuintupleBaggedNode) {
		// Build design
		Design design{};
		CompositeUnit* current = design.root;
		for (int i = 0; i < 5; i++) {
			current = &current->create<CompositeUnit>();
		}
		Junction& junction = current->create_junction();
		Assert::IsTrue(design.root->validate(std::cerr));
		while (current != nullptr) {
			Assert::AreEqual(static_cast<size_t>(0), current->ports.size());
			current = current->parent;
		}
		// Compare to graph
		Assert::IsTrue(TestUtils::equals(TestUtils::createOneNode(false), design.build()));
	}

	TEST_METHOD(TestQuintupleBaggedEdge) {
		// Build design
		Design design{};
		CompositeUnit* current = design.root;
		for (int i = 0; i < 5; i++) {
			current = &current->create<CompositeUnit>();
		}
		Junction& j0 = current->create_junction();
		Junction& j1 = current->create_junction();
		ConstantUnit& unit = current->create<ConstantUnit>(ConstantUnit::RESISTIVE_CELL, 0.5, 9.7);
		unit.ports[0].bind(j0);
		unit.ports[1].bind(j1);
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
			current = &current->create<CompositeUnit>();
		}
		Junction& j0 = current->create_junction();
		Junction& j1 = current->create_junction();
		Junction& j2 = current->create_junction();
		Junction& j3 = current->create_junction();
		Junction& j4 = current->create_junction();
		ConstantUnit& unit0 = current->create<ConstantUnit>(ConstantUnit::RESISTIVE_CELL, 0, 0);
		unit0.ports[0].bind(j0);
		unit0.ports[1].bind(j1);
		ConstantUnit& unit1 = current->create<ConstantUnit>(ConstantUnit::RESISTIVE_CELL, 0, 0);
		unit1.ports[0].bind(j1);
		unit1.ports[1].bind(j2);
		ConstantUnit& unit2 = current->create<ConstantUnit>(ConstantUnit::RESISTIVE_CELL, 0, 0);
		unit2.ports[0].bind(j2);
		unit2.ports[1].bind(j3);
		ConstantUnit& unit3 = current->create<ConstantUnit>(ConstantUnit::RESISTIVE_CELL, 0, 0);
		unit3.ports[0].bind(j1);
		unit3.ports[1].bind(j4);
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