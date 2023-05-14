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
	TEST_CLASS(CompositesPushPullTests) {
public:

	TEST_METHOD(TestPushEdge) {
		//            .___.
		// .___.  =>   
		//
		Design design{};
		CompositeUnit& group = Factory::in(*design.root).create_unit<CompositeUnit>();
		// Build design
		ConstantUnit& edge = Factory::in(*design.root).create_unit<ConstantUnit>(1, 2);
		TestUtils::validate_and_assert_composite_unit_counts(*design.root, 0, 2, 2);
		std::vector<Junction*> ends{ edge.ports[0].junction, edge.ports[1].junction };
		Graph graph0 = design.build();
		group.push({ &edge });
		TestUtils::validate_and_assert_composite_unit_counts(*design.root, 0, 1, 0);
		TestUtils::validate_and_assert_composite_unit_counts(group, 0, 1, 2);
		TestUtils::assert_ports_are_bound_to_ordered(edge.ports, ends);
		Graph graph1 = design.build();
		Assert::IsTrue(TestUtils::equals(graph0, graph1));
	}

	TEST_METHOD(TestPushTwoEdges) {
		//                .___.___.
		// .___.___.  =>   
		//
		Design design{};
		CompositeUnit& group = Factory::in(*design.root).create_unit<CompositeUnit>();
		// Build design
		ConstantUnit& edge0 = Factory::in(*design.root).create_unit<ConstantUnit>(1, 3);
		ConstantUnit& edge1 = Factory::in(*design.root).create_unit<ConstantUnit>(2, 4);
		Junction::merge(*edge0.ports[1].junction, *edge1.ports[0].junction);
		TestUtils::validate_and_assert_composite_unit_counts(*design.root, 0, 3, 3);
		Graph graph0 = design.build();
		group.push({ &edge0, &edge1 });
		TestUtils::validate_and_assert_composite_unit_counts(*design.root, 0, 1, 0);
		TestUtils::validate_and_assert_composite_unit_counts(group, 0, 2, 3);
		Graph graph1 = design.build();
		Assert::IsTrue(TestUtils::equals(graph0, graph1));
	}

	TEST_METHOD(TestPushTwoEdgesStaggered) {
		//                .___.          .___.___.
		// .___.___.  =>      .___.  =>
		//
		Design design{};
		CompositeUnit& group = Factory::in(*design.root).create_unit<CompositeUnit>();
		// Build design
		ConstantUnit& edge0 = Factory::in(*design.root).create_unit<ConstantUnit>(1, 3);
		ConstantUnit& edge1 = Factory::in(*design.root).create_unit<ConstantUnit>(2, 4);
		Junction::merge(*edge0.ports[1].junction, *edge1.ports[0].junction);
		TestUtils::validate_and_assert_composite_unit_counts(*design.root, 0, 3, 3);
		Graph graph0 = design.build();
		group.push({ &edge0 });
		TestUtils::validate_and_assert_composite_unit_counts(*design.root, 0, 2, 2);
		TestUtils::validate_and_assert_composite_unit_counts(group, 1, 1, 2);
		Graph graph1 = design.build();
		Assert::IsTrue(TestUtils::equals(graph0, graph1));
		group.push({ &edge1 });
		TestUtils::validate_and_assert_composite_unit_counts(*design.root, 0, 1, 0);
		TestUtils::validate_and_assert_composite_unit_counts(group, 0, 2, 3);
		Graph graph2 = design.build();
		Assert::IsTrue(TestUtils::equals(graph0, graph2));
	}


	TEST_METHOD(TestPushTreeStaggered) {
		//                   .            .           .
		//                 ./       .___./      .___./
		//       .                                   \.
		// .___./  =>  .___.    =>           =>
		//      \.          \.           \.
		Design design{};
		CompositeUnit& group = Factory::in(*design.root).create_unit<CompositeUnit>();
		// Build design
		ConstantUnit& edge0 = Factory::in(*design.root).create_unit<ConstantUnit>(1, 4);
		ConstantUnit& edge1 = Factory::in(*design.root).create_unit<ConstantUnit>(2, 5);
		ConstantUnit& edge2 = Factory::in(*design.root).create_unit<ConstantUnit>(3, 6);
		Junction::merge(*edge0.ports[1].junction, *edge1.ports[0].junction);
		Junction::merge(*edge0.ports[1].junction, *edge2.ports[0].junction);
		TestUtils::validate_and_assert_composite_unit_counts(*design.root, 0, 4, 4);
		Graph graph0 = design.build();
		group.push({ &edge0 });
		TestUtils::validate_and_assert_composite_unit_counts(*design.root, 0, 3, 3);
		TestUtils::validate_and_assert_composite_unit_counts(group, 1, 1, 2);
		Graph graph1 = design.build();
		Assert::IsTrue(TestUtils::equals(graph0, graph1));
		group.push({ &edge1 });
		TestUtils::validate_and_assert_composite_unit_counts(*design.root, 0, 2, 2);
		TestUtils::validate_and_assert_composite_unit_counts(group, 1, 2, 3);
		Graph graph2 = design.build();
		Assert::IsTrue(TestUtils::equals(graph0, graph2));
		group.push({ &edge2 });
		TestUtils::validate_and_assert_composite_unit_counts(*design.root, 0, 1, 0);
		TestUtils::validate_and_assert_composite_unit_counts(group, 0, 3, 4);
		Graph graph3 = design.build();
		Assert::IsTrue(TestUtils::equals(graph0, graph3));
	}
	
	TEST_METHOD(TestQuintuplePushPullRoundTrip) {
		Design design{};
		ConstantUnit& edge0 = Factory::in(*design.root).create_unit<ConstantUnit>(1, 4);
		ConstantUnit& edge1 = Factory::in(*design.root).create_unit<ConstantUnit>(2, 5);
		ConstantUnit& edge2 = Factory::in(*design.root).create_unit<ConstantUnit>(3, 6);
		Junction::merge(*edge0.ports[1].junction, *edge1.ports[0].junction);
		Junction::merge(*edge0.ports[1].junction, *edge2.ports[0].junction);
		CompositeUnit* current = design.root;
		TestUtils::validate_and_assert_composite_unit_counts(*design.root, 0, 3, 4);
		Graph graph = design.build();
		CompositeUnit* groups[5];
		groups[0] = design.root;
		for (int i = 1; i < 5; i++) {
			groups[i] = &Factory::in(*groups[i-1]).create_unit<CompositeUnit>();
		}
		for (int j = 0; j < 5; j++) {
			int nestedGroupCount = j == 4 ? 0 : 1;
			if (j == 0) {
				TestUtils::validate_and_assert_composite_unit_counts(*groups[j], 0, 3 + nestedGroupCount, 4);
			} else {
				TestUtils::validate_and_assert_composite_unit_counts(*groups[j], 0, nestedGroupCount, 0);
			}
		}
		for (int i = 1; i < 5; i++) {
			groups[i]->push({&edge0});
			for (int j = 0; j < 5; j++) {
				int nestedGroupCount = j == 4 ? 0 : 1;
				if (j == 0) {
					TestUtils::validate_and_assert_composite_unit_counts(*groups[j], 0, 2 + nestedGroupCount, 3);
				} else if (j < i) {
					TestUtils::validate_and_assert_composite_unit_counts(*groups[j], 1, 0 + nestedGroupCount, 1);
				} else if (j == i) {
					TestUtils::validate_and_assert_composite_unit_counts(*groups[j], 1, 1 + nestedGroupCount, 2);
				} else if (j < 4) {
					TestUtils::validate_and_assert_composite_unit_counts(*groups[j], 0, 0 + nestedGroupCount, 0);
				} else {
					TestUtils::validate_and_assert_composite_unit_counts(*groups[j], 0, 0 + nestedGroupCount, 0);
				}
			}
			Assert::IsTrue(TestUtils::equals(graph, design.build()));
		}
		for (int i = 1; i < 5; i++) {
			groups[i]->push({ &edge1 });
			for (int j = 0; j < 5; j++) {
				int nestedGroupCount = j == 4 ? 0 : 1;
				if (j == i && i == 4) {
					TestUtils::validate_and_assert_composite_unit_counts(*groups[j], 1, 2, 3);
				} else {
					if (j == 0) {
						TestUtils::validate_and_assert_composite_unit_counts(*groups[j], 0, 1 + nestedGroupCount, 2);
					} else if (j < i) {
						TestUtils::validate_and_assert_composite_unit_counts(*groups[j], 1, 0 + nestedGroupCount, 1);
					} else if (j == i) {
						TestUtils::validate_and_assert_composite_unit_counts(*groups[j], 1, 1 + nestedGroupCount, 2);
					} else if (j < 4) {
						TestUtils::validate_and_assert_composite_unit_counts(*groups[j], 1, 0 + nestedGroupCount, 1);
					} else {
						TestUtils::validate_and_assert_composite_unit_counts(*groups[j], 1, 1 + nestedGroupCount, 2);
					}
				}
			}
			Assert::IsTrue(TestUtils::equals(graph, design.build()));
		}
		for (int i = 1; i < 5; i++) {
			groups[i]->push({ &edge2 });
			for (int j = 0; j < 5; j++) {
				int nestedGroupCount = j == 4 ? 0 : 1;
				if (j == i && i == 4) {
					TestUtils::validate_and_assert_composite_unit_counts(*groups[j], 0, 3, 4);
				} else {
					if (j == 0) {
						TestUtils::validate_and_assert_composite_unit_counts(*groups[j], 0, 0 + nestedGroupCount, 0);
					} else if (j < i) {
						TestUtils::validate_and_assert_composite_unit_counts(*groups[j], 0, 0 + nestedGroupCount, 0);
					} else if (j == i) {
						TestUtils::validate_and_assert_composite_unit_counts(*groups[j], 0, 1 + nestedGroupCount, 2);
					} else if (j < 4) {
						TestUtils::validate_and_assert_composite_unit_counts(*groups[j], 1, 0 + nestedGroupCount, 1);
					} else {
						TestUtils::validate_and_assert_composite_unit_counts(*groups[j], 1, 2 + nestedGroupCount, 3);
					}
				}
			}
			Assert::IsTrue(TestUtils::equals(graph, design.build()));
		}
		for (int i = 4; i > 0; i--) {
			groups[i]->pop({ &edge0 });
			for (int j = 0; j < 5; j++) {
				int nestedGroupCount = j == 4 ? 0 : 1;
				if (j == i-1 && i == 1) {
					TestUtils::validate_and_assert_composite_unit_counts(*groups[j], 0, 1 + nestedGroupCount, 2);
				} else {
					if (j == 0) {
						TestUtils::validate_and_assert_composite_unit_counts(*groups[j], 0, 0 + nestedGroupCount, 0);
					} else if (j < i - 1) {
						TestUtils::validate_and_assert_composite_unit_counts(*groups[j], 0, 0 + nestedGroupCount, 0);
					} else if (j == i - 1) {
						TestUtils::validate_and_assert_composite_unit_counts(*groups[j], 0, 1 + nestedGroupCount, 2);
					} else if (j < 4) {
						TestUtils::validate_and_assert_composite_unit_counts(*groups[j], 1, 0 + nestedGroupCount, 1);
					} else {
						TestUtils::validate_and_assert_composite_unit_counts(*groups[j], 1, 2 + nestedGroupCount, 3);
					}
				}
			}
			Assert::IsTrue(TestUtils::equals(graph, design.build()));
		}
		for (int i = 4; i > 0; i--) {
			groups[i]->pop({ &edge1 });
			for (int j = 0; j < 5; j++) {
				int nestedGroupCount = j == 4 ? 0 : 1;
				if (j == i-1 && i == 1) {
					TestUtils::validate_and_assert_composite_unit_counts(*groups[j], 0, 2 + nestedGroupCount, 3);
				} else {
					if (j == 0) {
						TestUtils::validate_and_assert_composite_unit_counts(*groups[j], 0, 1 + nestedGroupCount, 2);
					} else if (j < i - 1) {
						TestUtils::validate_and_assert_composite_unit_counts(*groups[j], 1, 0 + nestedGroupCount, 1);
					} else if (j == i - 1) {
						TestUtils::validate_and_assert_composite_unit_counts(*groups[j], 1, 1 + nestedGroupCount, 2);
					} else if (j < 4) {
						TestUtils::validate_and_assert_composite_unit_counts(*groups[j], 1, 0 + nestedGroupCount, 1);
					} else {
						TestUtils::validate_and_assert_composite_unit_counts(*groups[j], 1, 1 + nestedGroupCount, 2);
					}
				}
			}
			Assert::IsTrue(TestUtils::equals(graph, design.build()));
		}
		for (int i = 4; i > 0; i--) {
			groups[i]->pop({ &edge2 });
			for (int j = 0; j < 5; j++) {
				int nestedGroupCount = j == 4 ? 0 : 1;
				if (j == i-1 && i == 1) {
					TestUtils::validate_and_assert_composite_unit_counts(*groups[j], 0,  3 + nestedGroupCount, 4);
				} else {
					if (j == 0) {
						TestUtils::validate_and_assert_composite_unit_counts(*groups[j], 0, 2 + nestedGroupCount, 3);
					} else if (j < i - 1) {
						TestUtils::validate_and_assert_composite_unit_counts(*groups[j], 1, 0 + nestedGroupCount, 1);
					} else if (j == i - 1) {
						TestUtils::validate_and_assert_composite_unit_counts(*groups[j], 1, 1 + nestedGroupCount, 2);
					} else if (j < 4) {
						TestUtils::validate_and_assert_composite_unit_counts(*groups[j], 0, 0 + nestedGroupCount, 0);
					} else {
						TestUtils::validate_and_assert_composite_unit_counts(*groups[j],0, 0 + nestedGroupCount, 0);
					}
				}
			}
			Assert::IsTrue(TestUtils::equals(graph, design.build()));
		}
	}

	};

}