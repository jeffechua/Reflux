#include "EngineTests/pch.h"
#include <iostream>
#include <CppUnitTest.h>
#include "Engine/include/Design/Design.h"
#include "Engine/include/Design/ConstantUnit.h"
#include "../../include/Utils/design_comparison.h"
#include "../../include/Utils/graph_comparison.h"
#include "../../include/Utils/pointer_comparison.h"
#include "../../include/Utils/graph_factory.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Reflux::Engine::Design::Tests {
	TEST_CLASS(CompositesPushPullTests) {
public:

	TEST_METHOD(TestPushSingleNodeToRoot) {
		Design design{};
		TestPushSingleNodeImpl(design, design, *design.root);
	}

	TEST_METHOD(TestPushSingleNodeToGroup) {
		Design design{};
		CompositeUnit& group = design.root->create<CompositeUnit>();
		TestPushSingleNodeImpl(design, *design.root, group);
	}

	template<typename DesignOrCompositeUnit> 
	void TestPushSingleNodeImpl(Design& design, DesignOrCompositeUnit& outer, CompositeUnit& inner) {
		// Build design
		Junction& junction = outer.create_junction();
		TestUtils::validate_and_assert_composite_unit_counts(inner, 0, 0, 0);
		inner.push({ &junction }, {});
		TestUtils::validate_and_assert_composite_unit_counts(inner, 0, 0, 1);
		// Compare to graph
		Assert::IsTrue(TestUtils::equals(TestUtils::createOneNode(false), design.build()));
	}

	TEST_METHOD(TestPopSingleNodeFromRoot) {
		Design design{};
		TestPopSingleNodeImpl(design, design, *design.root);
	}

	TEST_METHOD(TestPopSingleNodeFromGroup) {
		Design design{};
		CompositeUnit& group = design.root->create<CompositeUnit>();
		TestPopSingleNodeImpl(design, *design.root, group);
	}

	template<typename DesignOrCompositeUnit>
	void TestPopSingleNodeImpl(Design& design, DesignOrCompositeUnit& outer, CompositeUnit& inner) {
		// Build design
		Junction& junction = inner.create_junction();
		TestUtils::validate_and_assert_composite_unit_counts(inner, 0, 0, 1);
		inner.pop({ &junction }, {});
		TestUtils::validate_and_assert_composite_unit_counts(inner, 0, 0, 0);
	}

	TEST_METHOD(TestPushUnboundEdgeToRoot) {
		Design design{};
		TestPushUnboundEdgeImpl(design, design, *design.root);
	}

	TEST_METHOD(TestPushUnboundEdgeToGroup) {
		Design design{};
		CompositeUnit& group = design.root->create<CompositeUnit>();
		TestPushUnboundEdgeImpl(design, *design.root, group);
	}

	template<typename DesignOrCompositeUnit>
	void TestPushUnboundEdgeImpl(Design& design, DesignOrCompositeUnit& outer, CompositeUnit& inner) {
		// Build design
		ConstantUnit& edge = outer.create<ConstantUnit>(ConstantUnit::CELL, 0, 0);
		Junction& j0 = outer.create_junction();
		Junction& j1 = outer.create_junction();
		TestUtils::validate_and_assert_composite_unit_counts(inner, 0, 0, 0);
		inner.push({}, { &edge });
		TestUtils::validate_and_assert_composite_unit_counts(inner, 2, 1, 0);
		// Can't compare to graph since design is not buildable
	}

	TEST_METHOD(TestPopUnboundEdgeFromRoot) {
		Design design{};
		TestPopUnboundEdgeImpl(design, design, *design.root);
	}

	TEST_METHOD(TestPopUnboundEdgeFromGroup) {
		Design design{};
		CompositeUnit& group = design.root->create<CompositeUnit>();
		TestPopUnboundEdgeImpl(design, *design.root, group);
	}

	template<typename DesignOrCompositeUnit>
	void TestPopUnboundEdgeImpl(Design & design, DesignOrCompositeUnit & outer, CompositeUnit & inner) {
		// Build design
		ConstantUnit& edge = inner.create<ConstantUnit>(ConstantUnit::CELL, 0, 0);
		TestUtils::validate_and_assert_composite_unit_counts(inner, 2, 1, 0);
		inner.pop({}, { &edge });
		TestUtils::validate_and_assert_composite_unit_counts(inner, 0, 0, 0);
	}

	TEST_METHOD(TestPushUnboundEdgeAndNodesToRoot) {
		Design design{};
		TestPushUnboundEdgeAndNodesImpl(design, design, *design.root);
	}

	TEST_METHOD(TestPushUnboundEdgeAndNodesToGroup) {
		Design design{};
		CompositeUnit& group = design.root->create<CompositeUnit>();
		TestPushUnboundEdgeAndNodesImpl(design, *design.root, group);
	}

	template<typename DesignOrCompositeUnit>
	void TestPushUnboundEdgeAndNodesImpl(Design& design, DesignOrCompositeUnit& outer, CompositeUnit& inner) {
		// Build design
		ConstantUnit& edge = outer.create<ConstantUnit>(ConstantUnit::CELL, 0, 0);
		Junction& j0 = outer.create_junction();
		Junction& j1 = outer.create_junction();
		TestUtils::validate_and_assert_composite_unit_counts(inner, 0, 0, 0);
		inner.push({ &j0, &j1 }, { &edge });
		TestUtils::validate_and_assert_composite_unit_counts(inner, 2, 1, 2);
		// Can't compare to graph since design is not buildable
	}

	TEST_METHOD(TestPopUnboundEdgeAndNodesFromRoot) {
		Design design{};
		TestPopUnboundEdgeAndNodesImpl(design, design, *design.root);
	}

	TEST_METHOD(TestPopUnboundEdgeAndNodesFromGroup) {
		Design design{};
		CompositeUnit& group = design.root->create<CompositeUnit>();
		TestPopUnboundEdgeAndNodesImpl(design, *design.root, group);
	}

	template<typename DesignOrCompositeUnit>
	void TestPopUnboundEdgeAndNodesImpl(Design& design, DesignOrCompositeUnit& outer, CompositeUnit& inner) {
		// Build design
		ConstantUnit& edge = inner.create<ConstantUnit>(ConstantUnit::CELL, 0, 0);
		Junction& j0 = inner.create_junction();
		Junction& j1 = inner.create_junction();
		TestUtils::validate_and_assert_composite_unit_counts(inner, 2, 1, 2);
		inner.pop({ &j0, &j1 }, { &edge });
		TestUtils::validate_and_assert_composite_unit_counts(inner, 0, 0, 0);
	}

	TEST_METHOD(TestPushBoundEdgeStaggeredToRoot) {
		Design design{};
		TestPushBoundEdgeStaggeredImpl(design, design, *design.root);
	}

	TEST_METHOD(TestPushBoundEdgeStaggeredToGroup) {
		Design design{};
		CompositeUnit& group = design.root->create<CompositeUnit>();
		TestPushBoundEdgeStaggeredImpl(design, *design.root, group);
	}

	template<typename DesignOrCompositeUnit>
	void TestPushBoundEdgeStaggeredImpl(Design& design, DesignOrCompositeUnit& outer, CompositeUnit& inner) {
		// Build design
		ConstantUnit& edge = outer.create<ConstantUnit>(ConstantUnit::CELL, 0, 0);
		Junction& j0 = outer.create_junction();
		Junction& j1 = outer.create_junction();
		edge.ports[0].bind(j0);
		edge.ports[1].bind(j1);
		TestUtils::validate_and_assert_composite_unit_counts(inner, 0, 0, 0);
		TestUtils::assert_ports_are_bound_to_ordered(edge.ports, { &j0, &j1 });
		inner.push({}, { &edge });
		TestUtils::validate_and_assert_composite_unit_counts(inner, 2, 1, 0);
		TestUtils::assert_ports_are_bound_to_unordered(inner.ports, { &j0, &j1 });
		TestUtils::assert_ports_are_bound_to_ordered(edge.ports, { nullptr, nullptr });
		inner.push({ &j0 }, {});
		TestUtils::validate_and_assert_composite_unit_counts(inner, 1, 1, 1);
		TestUtils::assert_ports_are_bound_to_unordered(inner.ports, { &j1 });
		TestUtils::assert_ports_are_bound_to_ordered(edge.ports, { &j0, nullptr });
		inner.push({ &j1 }, {});
		TestUtils::validate_and_assert_composite_unit_counts(inner, 0, 1, 2);
		TestUtils::assert_ports_are_bound_to_ordered(edge.ports, { &j0, &j1 });
		// Compare to graph
		Assert::IsTrue(TestUtils::equals(TestUtils::createOneEdge(false), design.build()));
	}

	TEST_METHOD(TestPopBoundEdgeStaggeredFromRoot) {
		Design design{};
		TestPopBoundEdgeStaggeredImpl(design, design, *design.root);
	}

	TEST_METHOD(TestPopBoundEdgeStaggeredFromGroup) {
		Design design{};
		CompositeUnit& group = design.root->create<CompositeUnit>();
		TestPopBoundEdgeStaggeredImpl(design, *design.root, group);
	}

	template<typename DesignOrCompositeUnit>
	void TestPopBoundEdgeStaggeredImpl(Design& design, DesignOrCompositeUnit& outer, CompositeUnit& inner) {
		// Build design
		ConstantUnit& edge = inner.create<ConstantUnit>(ConstantUnit::CELL, 0, 0);
		Junction& j0 = inner.create_junction();
		Junction& j1 = inner.create_junction();
		edge.ports[0].bind(j0);
		edge.ports[1].bind(j1);
		TestUtils::validate_and_assert_composite_unit_counts(inner, 0, 1, 2);
		TestUtils::assert_ports_are_bound_to_ordered(edge.ports, { &j0, &j1 });
		inner.pop({ &j0 }, {});
		TestUtils::validate_and_assert_composite_unit_counts(inner, 1, 1, 1);
		TestUtils::assert_ports_are_bound_to_unordered(inner.ports, { &j0 });
		TestUtils::assert_ports_are_bound_to_ordered(edge.ports, { nullptr, &j1 });
		inner.pop({ &j1 }, {});
		TestUtils::validate_and_assert_composite_unit_counts(inner, 2, 1, 0);
		TestUtils::assert_ports_are_bound_to_unordered(inner.ports, { &j0, &j1 });
		TestUtils::assert_ports_are_bound_to_ordered(edge.ports, { nullptr, nullptr });
		inner.pop({ }, { &edge });
		TestUtils::validate_and_assert_composite_unit_counts(inner, 0, 0, 0);
		TestUtils::assert_ports_are_bound_to_ordered(edge.ports, { &j0, &j1 });
	}

	TEST_METHOD(TestPushBoundEdgeAsGroupToRoot) {
		Design design{};
		TestPushBoundEdgeAsGroupImpl(design, design, *design.root);
	}

	TEST_METHOD(TestPushBoundEdgeAsGroupToGroup) {
		Design design{};
		CompositeUnit& group = design.root->create<CompositeUnit>();
		TestPushBoundEdgeAsGroupImpl(design, *design.root, group);
	}

	template<typename DesignOrCompositeUnit>
	void TestPushBoundEdgeAsGroupImpl(Design& design, DesignOrCompositeUnit& outer, CompositeUnit& inner) {
		// Build design
		ConstantUnit& edge = outer.create<ConstantUnit>(ConstantUnit::CELL, 0, 0);
		Junction& j0 = outer.create_junction();
		Junction& j1 = outer.create_junction();
		edge.ports[0].bind(j0);
		edge.ports[1].bind(j1);
		TestUtils::validate_and_assert_composite_unit_counts(inner, 0, 0, 0);
		inner.push({ &j0, &j1 }, { &edge });
		TestUtils::validate_and_assert_composite_unit_counts(inner, 0, 1, 2);
		TestUtils::assert_ports_are_bound_to_ordered(edge.ports, { &j0, &j1 });
		// Compare to graph
		Assert::IsTrue(TestUtils::equals(TestUtils::createOneEdge(false), design.build()));
	}

	TEST_METHOD(TestPopBoundEdgeAsGroupFromRoot) {
		Design design{};
		TestPopBoundEdgeAsGroupImpl(design, design, *design.root);
	}

	TEST_METHOD(TestPopBoundEdgeAsGroupFromGroup) {
		Design design{};
		CompositeUnit& group = design.root->create<CompositeUnit>();
		TestPopBoundEdgeAsGroupImpl(design, *design.root, group);
	}

	template<typename DesignOrCompositeUnit>
	void TestPopBoundEdgeAsGroupImpl(Design& design, DesignOrCompositeUnit& outer, CompositeUnit& inner) {
		// Build design
		ConstantUnit& edge = inner.create<ConstantUnit>(ConstantUnit::CELL, 0, 0);
		Junction& j0 = inner.create_junction();
		Junction& j1 = inner.create_junction();
		edge.ports[0].bind(j0);
		edge.ports[1].bind(j1);
		TestUtils::validate_and_assert_composite_unit_counts(inner, 0, 1, 2);
		inner.pop({ &j0, &j1 }, { &edge });
		TestUtils::validate_and_assert_composite_unit_counts(inner, 0, 0, 0);
		TestUtils::assert_ports_are_bound_to_ordered(edge.ports, { &j0, &j1 });
	}

	TEST_METHOD(TestPushExternallyBoundNodeToRootThrows) {
		Design design{};
		TestPushExternallyBoundNodeThrowsImpl(design, design, *design.root);
	}

	TEST_METHOD(TestPushExternallyBoundNodeToGroupThrows) {
		Design design{};
		CompositeUnit& group = design.root->create<CompositeUnit>();
		TestPushExternallyBoundNodeThrowsImpl(design, *design.root, group);
	}

	template<typename DesignOrCompositeUnit>
	void TestPushExternallyBoundNodeThrowsImpl(Design& design, DesignOrCompositeUnit& outer, CompositeUnit& inner) {
		ConstantUnit& edge = outer.create<ConstantUnit>(ConstantUnit::CELL, 0, 0);
		Junction& junction = outer.create_junction();
		edge.ports[0].bind(junction);
		std::vector<Junction*> pushBoundaryViolations = inner.get_push_boundary_violations({ &junction }, {});
		Assert::AreEqual(static_cast<size_t>(1), pushBoundaryViolations.size());
		Assert::AreEqual(&junction, pushBoundaryViolations[0]);
	}

	TEST_METHOD(TestPopInternallyBoundEdgeFromRootThrows) {
		Design design{};
		TestPopInternallyBoundEdgeThrowsImpl(design, design, *design.root);
	}

	TEST_METHOD(TestPopInternallyBoundEdgeFromGroupThrows) {
		Design design{};
		CompositeUnit& group = design.root->create<CompositeUnit>();
		TestPopInternallyBoundEdgeThrowsImpl(design, *design.root, group);
	}

	template<typename DesignOrCompositeUnit>
	void TestPopInternallyBoundEdgeThrowsImpl(Design& design, DesignOrCompositeUnit& outer, CompositeUnit& inner) {
		ConstantUnit& edge = inner.create<ConstantUnit>(ConstantUnit::CELL, 0, 0);
		Junction& junction = inner.create_junction();
		edge.ports[0].bind(junction);
		std::vector<BaseUnit*> popBoundaryViolations = inner.get_pop_boundary_violations({}, { &edge });
		Assert::AreEqual(static_cast<size_t>(1), popBoundaryViolations.size());
		Assert::AreEqual((BaseUnit*)&edge, popBoundaryViolations[0]);
	}

	TEST_METHOD(TestQuintuplePushPullRoundTrip) {
		Design design{};
		ConstantUnit& edge = design.root->create<ConstantUnit>(ConstantUnit::CELL, 0, 0);
		Junction& j0 = design.root->create_junction();
		Junction& j1 = design.root->create_junction();
		edge.ports[0].bind(j0);
		edge.ports[1].bind(j1);
		CompositeUnit* current = design.root;
		Graph graph = design.build();
		for (int i = 0; i < 5; i++) {
			current = &current->create<CompositeUnit>();
			current->push({ &j0 }, { &edge });
			TestUtils::validate_and_assert_composite_unit_counts(*current, 1, 1, 1);
			TestUtils::assert_ports_are_bound_to_ordered(edge.ports, { &j0, nullptr });
			TestUtils::assert_ports_are_bound_to_ordered(current->ports, { i == 0 ? &j1 : nullptr });
			TestUtils::validate_and_assert_composite_unit_counts(*design.root, 0, 1, 1);
			CompositeUnit* ancestor = current->parent;
			while (ancestor != design.root) {
				TestUtils::validate_and_assert_composite_unit_counts(*ancestor, 1, 1, 0);
				Junction* boundJunction = ancestor->parent == design.root ? &j1 : nullptr;
				TestUtils::assert_ports_are_bound_to_ordered(ancestor->ports, { boundJunction });
				ancestor = ancestor->parent;
			}
			Assert::IsTrue(TestUtils::equals(graph, design.build()));
		}
		current = design.root;
		for (int i = 0; i < 5; i++) {
			current = dynamic_cast<CompositeUnit*>(*current->units.begin());
			TestUtils::validate_and_assert_composite_unit_counts(*current, 1, 1, i == 4 ? 1 : 0);
			TestUtils::assert_ports_are_bound_to_ordered(current->ports, { &j1 });
			current->push({ &j1 }, {});
			TestUtils::validate_and_assert_composite_unit_counts(*current, 0, 1, i == 4 ? 2 : 1);
			CompositeUnit* ancestor = current->parent;
			while (ancestor) {
				TestUtils::validate_and_assert_composite_unit_counts(*ancestor, 0, 1, 0);
				ancestor = ancestor->parent;
			}
			Assert::IsTrue(TestUtils::equals(graph, design.build()));
		}
		CompositeUnit* top = current;
		while (current != design.root) {
			TestUtils::validate_and_assert_composite_unit_counts(*current, 0, 1, current == top ? 2 : 1);
			current->pop({ &j0 }, {});
			CompositeUnit* descendant = top;
			while (descendant != current->parent) {
				TestUtils::validate_and_assert_composite_unit_counts(*descendant, 1, 1, descendant == top ? 1 : 0);
				TestUtils::assert_ports_are_bound_to_ordered(current->ports, { &j0 });
				descendant = descendant->parent;
			}
			while (descendant != nullptr) {
				TestUtils::validate_and_assert_composite_unit_counts(*descendant, 0, 1, descendant == current->parent ? 1 : 0);
				descendant = descendant->parent;
			}
			current = current->parent;
			Assert::IsTrue(TestUtils::equals(graph, design.build()));
		}
		current = top;
		while (current != design.root) {
			TestUtils::validate_and_assert_composite_unit_counts(*current, 1, current == top ? 1 : 2, 1);
			current->pop({ &j1 }, { &edge });
			CompositeUnit* descendant = top;
			while (descendant != current->parent) {
				TestUtils::validate_and_assert_composite_unit_counts(*descendant, 0, descendant == top ? 0 : 1, 0);
				descendant = descendant->parent;
			}
			if (descendant != design.root) {
				TestUtils::validate_and_assert_composite_unit_counts(*descendant, 1, 2, 1);
				descendant = descendant->parent;
			}
			while (descendant != design.root) {
				TestUtils::validate_and_assert_composite_unit_counts(*descendant, 1, 1, 0);
				descendant = descendant->parent;
			}
			if (current->parent == design.root) {
				TestUtils::validate_and_assert_composite_unit_counts(*design.root, 0, 2, 2);
			} else {
				TestUtils::validate_and_assert_composite_unit_counts(*design.root, 0, 1, 1);
			}
			current = current->parent;
			Assert::IsTrue(TestUtils::equals(graph, design.build()));
		}
	}

	};

}