#include "EngineTests/pch.h"
#include <CppUnitTest.h>
#include "Engine/include/Design/Design.h"
#include "Engine/include/Design/CompositeUnit.h"
#include "Engine/include/Design/ConstantUnit.h"
#include "Engine/include/Simulation/Graph.h"
#include "../../include/Utils/design_comparison.h"
#include "../../include/Utils/graph_comparison.h"
#include "../../include/Utils/pointer_comparison.h"
#include "../../include/Utils/graph_factory.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Reflux::Engine::Design::Tests {
	TEST_CLASS(DestroyTests) {
public:

	TEST_METHOD(DestroyEdgeStaggered) {
		Design design{};
		ConstantUnit& edge = design.root->create<ConstantUnit>(ConstantUnit::CELL, 0, 0);
		Junction& j0 = design.root->create_junction();
		Junction& j1 = design.root->create_junction();
		edge.ports[0].bind(j0);
		edge.ports[1].bind(j1);
		design.destroy(j0);
		Assert::IsFalse(edge.ports[0].is_bound());
		TestUtils::validate_and_assert_composite_unit_counts(*design.root, 1, 1, 1);
		design.destroy(edge);
		Assert::AreEqual(static_cast<size_t>(0), j1.ports.size());
		TestUtils::validate_and_assert_composite_unit_counts(*design.root, 0, 0, 1);
		design.destroy(j1);
		TestUtils::validate_and_assert_composite_unit_counts(*design.root, 0, 0, 0);
	}

	TEST_METHOD(DestroyQuintupleBaggedEdgeStaggered) {
		Design design{};
		CompositeUnit* innermost = design.root;
		for (int i = 0; i < 5; i++) {
			innermost = &innermost->create<CompositeUnit>();
		}
		ConstantUnit& edge = innermost->create<ConstantUnit>(ConstantUnit::CELL, 0, 0);
		Junction& j0 = innermost->create_junction();
		Junction& j1 = innermost->create_junction();
		edge.ports[0].bind(j0);
		edge.ports[1].bind(j1);
		design.destroy(j0);
		Assert::IsFalse(edge.ports[0].is_bound());
		TestUtils::validate_and_assert_composite_unit_counts(*innermost, 1, 1, 1);
		CompositeUnit* current = innermost->parent;
		while (current) {
			TestUtils::validate_and_assert_composite_unit_counts(*current, 1, 1, 0);
			current = current->parent;
		}
		design.destroy(edge);
		Assert::AreEqual(static_cast<size_t>(0), j1.ports.size());
		TestUtils::validate_and_assert_composite_unit_counts(*innermost, 0, 0, 1);
		current = innermost->parent;
		while (current) {
			TestUtils::validate_and_assert_composite_unit_counts(*current, 0, 1, 0);
			current = current->parent;
		}
		design.destroy(j1);
		TestUtils::validate_and_assert_composite_unit_counts(*innermost, 0, 0, 0);
		current = innermost->parent;
		while (current) {
			TestUtils::validate_and_assert_composite_unit_counts(*current, 0, 1, 0);
			current = current->parent;
		}
	}

	TEST_METHOD(DestroyQuintupleBaggedEdgeFromMiddle) {
		Design design{};
		CompositeUnit* innermost = design.root;
		CompositeUnit* middle = nullptr;
		for (int i = 0; i < 5; i++) {
			innermost = &innermost->create<CompositeUnit>();
			if (i == 2) {
				middle = innermost;
			}
		}
		ConstantUnit& edge = innermost->create<ConstantUnit>(ConstantUnit::CELL, 0, 0);
		Junction& j0 = innermost->create_junction();
		Junction& j1 = innermost->create_junction();
		edge.ports[0].bind(j0);
		edge.ports[1].bind(j1);
		CompositeUnit* current = middle->parent;
		design.destroy(*middle);
		TestUtils::validate_and_assert_composite_unit_counts(*current, 0, 0, 0);
		current = current->parent;
		while (current != nullptr) {
			TestUtils::validate_and_assert_composite_unit_counts(*current, 0, 1, 0);
			current = current->parent;
		}
	}

	};

}