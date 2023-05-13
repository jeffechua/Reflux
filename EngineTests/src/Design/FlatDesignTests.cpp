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
	TEST_CLASS(FlatDesignTests) {
public:

	TEST_METHOD(TestOneJunction) {
		Design design{};
		Junction& junction = design.root->create_junction();
		Assert::AreEqual(static_cast<size_t>(0), design.root->ports.size());
		Assert::IsTrue(design.root->validate(std::cerr));
		Assert::IsTrue(TestUtils::equals(TestUtils::createOneNode(false), design.build()));
	}

	TEST_METHOD(TestOneConstantEdge) {
		Design design{};
		Junction& j0 = design.root->create_junction();
		Junction& j1 = design.root->create_junction();
		ConstantUnit& unit = design.root->create<ConstantUnit>(ConstantUnit::RESISTIVE_CELL, 0.5, 9.7);
		unit.ports[0].bind(j0);
		unit.ports[1].bind(j1);
		Assert::IsTrue(design.root->validate(std::cerr));
		Assert::AreEqual(static_cast<size_t>(0), design.root->ports.size());
		GraphBuilder builder(2, 1);
		Node& n0 = builder.add_node();
		Node& n1 = builder.add_node();
		ConstantEdge& edge = builder.add_edge<ConstantEdge>(0.5, 9.7);
		edge.bind(n0, n1);
		Assert::IsTrue(TestUtils::equals(builder.build(), design.build()));
	}

	TEST_METHOD(TestOneUnboundEdge) {
		Design design{};
		ConstantUnit& unit = design.root->create<ConstantUnit>(ConstantUnit::RESISTIVE_CELL, 0.5, 9.7);
		Assert::IsTrue(design.root->validate(std::cerr));
		Assert::AreEqual(static_cast<size_t>(2), design.root->ports.size());
		Port* ports[2];
		int i = 0;
		for (Port& port : design.root->ports) {
			ports[i] = &port;
			i++;
		}
		Assert::AreNotEqual(ports[0], ports[1]);
		Assert::ExpectException<std::runtime_error>([&]() { design.build(); }, L"Tried to build design with open ports.");
	}

	TEST_METHOD(TestTwoEdges) {
		Design design{};
		Junction& j0 = design.root->create_junction();
		Junction& j1 = design.root->create_junction();
		Junction& j2 = design.root->create_junction();
		ConstantUnit& unit0 = design.root->create<ConstantUnit>(ConstantUnit::RESISTIVE_CELL, 0, 0);
		unit0.ports[0].bind(j0);
		unit0.ports[1].bind(j1);
		ConstantUnit& unit1 = design.root->create<ConstantUnit>(ConstantUnit::RESISTIVE_CELL, 0, 0);
		unit1.ports[0].bind(j1);
		unit1.ports[1].bind(j2);
		Assert::IsTrue(design.root->validate(std::cerr));
		Assert::AreEqual(static_cast<size_t>(0), design.root->ports.size());
		Assert::IsTrue(TestUtils::equals(TestUtils::createTwoEdges(false), design.build()));
	}

	TEST_METHOD(TestTwoEdgesConverging) {
		Design design{};
		Junction& j0 = design.root->create_junction();
		Junction& j1 = design.root->create_junction();
		Junction& j2 = design.root->create_junction();
		ConstantUnit& unit0 = design.root->create<ConstantUnit>(ConstantUnit::RESISTIVE_CELL, 0, 0);
		unit0.ports[0].bind(j0);
		unit0.ports[1].bind(j1);
		ConstantUnit& unit1 = design.root->create<ConstantUnit>(ConstantUnit::RESISTIVE_CELL, 0, 0);
		unit1.ports[0].bind(j2);
		unit1.ports[1].bind(j1);
		Assert::IsTrue(design.root->validate(std::cerr));
		Assert::AreEqual(static_cast<size_t>(0), design.root->ports.size());
		Assert::IsTrue(TestUtils::equals(TestUtils::createTwoEdgesConverging(false), design.build()));
	}

	TEST_METHOD(TestTriangle) {
		Design design{};
		Junction& j0 = design.root->create_junction();
		Junction& j1 = design.root->create_junction();
		Junction& j2 = design.root->create_junction();
		ConstantUnit& unit0 = design.root->create<ConstantUnit>(ConstantUnit::RESISTIVE_CELL, 0, 0);
		unit0.ports[0].bind(j0);
		unit0.ports[1].bind(j1);
		ConstantUnit& unit1 = design.root->create<ConstantUnit>(ConstantUnit::RESISTIVE_CELL, 0, 0);
		unit1.ports[0].bind(j1);
		unit1.ports[1].bind(j2);
		ConstantUnit& unit2 = design.root->create<ConstantUnit>(ConstantUnit::RESISTIVE_CELL, 0, 0);
		unit2.ports[0].bind(j2);
		unit2.ports[1].bind(j0);
		Assert::IsTrue(design.root->validate(std::cerr));
		Assert::AreEqual(static_cast<size_t>(0), design.root->ports.size());
		Assert::IsTrue(TestUtils::equals(TestUtils::createTriangle(false), design.build()));
	}

	TEST_METHOD(TestSymmetricTree) {
		Design design{};
		Junction& j0 = design.root->create_junction();
		Junction& j1 = design.root->create_junction();
		Junction& j2 = design.root->create_junction();
		Junction& j3 = design.root->create_junction();
		Junction& j4 = design.root->create_junction();
		ConstantUnit& unit0 = design.root->create<ConstantUnit>(ConstantUnit::RESISTIVE_CELL, 0, 0);
		unit0.ports[0].bind(j0);
		unit0.ports[1].bind(j1);
		ConstantUnit& unit1 = design.root->create<ConstantUnit>(ConstantUnit::RESISTIVE_CELL, 0, 0);
		unit1.ports[0].bind(j1);
		unit1.ports[1].bind(j2);
		ConstantUnit& unit2 = design.root->create<ConstantUnit>(ConstantUnit::RESISTIVE_CELL, 0, 0);
		unit2.ports[0].bind(j1);
		unit2.ports[1].bind(j3);
		ConstantUnit& unit3 = design.root->create<ConstantUnit>(ConstantUnit::RESISTIVE_CELL, 0, 0);
		unit3.ports[0].bind(j1);
		unit3.ports[1].bind(j4);
		Assert::IsTrue(design.root->validate(std::cerr));
		Assert::AreEqual(static_cast<size_t>(0), design.root->ports.size());
		Assert::IsTrue(TestUtils::equals(TestUtils::createSymmetricTree(false), design.build()));
	}

	TEST_METHOD(TestAsymmetricTree) {
		Design design{};
		Junction& j0 = design.root->create_junction();
		Junction& j1 = design.root->create_junction();
		Junction& j2 = design.root->create_junction();
		Junction& j3 = design.root->create_junction();
		Junction& j4 = design.root->create_junction();
		ConstantUnit& unit0 = design.root->create<ConstantUnit>(ConstantUnit::RESISTIVE_CELL, 0, 0);
		unit0.ports[0].bind(j0);
		unit0.ports[1].bind(j1);
		ConstantUnit& unit1 = design.root->create<ConstantUnit>(ConstantUnit::RESISTIVE_CELL, 0, 0);
		unit1.ports[0].bind(j1);
		unit1.ports[1].bind(j2);
		ConstantUnit& unit2 = design.root->create<ConstantUnit>(ConstantUnit::RESISTIVE_CELL, 0, 0);
		unit2.ports[0].bind(j2);
		unit2.ports[1].bind(j3);
		ConstantUnit& unit3 = design.root->create<ConstantUnit>(ConstantUnit::RESISTIVE_CELL, 0, 0);
		unit3.ports[0].bind(j1);
		unit3.ports[1].bind(j4);
		Assert::IsTrue(design.root->validate(std::cerr));
		Assert::AreEqual(static_cast<size_t>(0), design.root->ports.size());
		Assert::IsTrue(TestUtils::equals(TestUtils::createAsymmetricTree(false), design.build()));
	}

	};

}