#include "EngineTests/pch.h"
#include <algorithm>
#include <cstdlib>
#include <CppUnitTest.h>
#include "Engine/include/Simulation/GraphBuilder.h"
#include "Engine/include/Simulation/ConstantEdge.h"
#include "../../include/Utils/graph_factory.h"
#include "../../include/Utils/graph_comparison.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Reflux::Engine::Simulation;

namespace Reflux::Engine::TestUtils {

	TEST_CLASS(GraphComparisonTests) {
public:

	TEST_METHOD(TestOneNode) {
		Assert::IsTrue(equals(createOneNode(false), createOneNode(false)));
	}

	TEST_METHOD(TestOneEdge) {
		Assert::IsTrue(equals(createOneEdge(false), createOneEdge(false)));
	}

	TEST_METHOD(TestOneNodeVsOneEdge) {
		Assert::IsFalse(equals(createOneNode(false), createOneEdge(false)));
	}

	TEST_METHOD(TestTwoEdges) {
		Assert::IsTrue(equals(createTwoEdges(false), createTwoEdges(false)));
	}

	TEST_METHOD(TestTwoEdgesConverging) {
		Assert::IsTrue(equals(createTwoEdgesConverging(false), createTwoEdgesConverging(false)));
	}

	TEST_METHOD(TestTwoEdgesVsTwoEdgesConverging) {
		Assert::IsFalse(equals(createTwoEdges(false), createTwoEdgesConverging(false)));
	}

	TEST_METHOD(TestTriangle) {
		Assert::IsTrue(equals(createTriangle(false), createTriangle(false)));
	}

	TEST_METHOD(TestTriangleVsThreeEdges) {
		Assert::IsFalse(equals(createTwoEdges(false), createThreeEdges(false)));
	}

	TEST_METHOD(TestSymmetricTree) {
		Assert::IsTrue(equals(createSymmetricTree(false), createSymmetricTree(false)));
	}

	TEST_METHOD(TestAsymmetricTree) {
		Assert::IsTrue(equals(createAsymmetricTree(false), createAsymmetricTree(false)));
	}

	TEST_METHOD(TestSymmetricVsAsymmetricTree) {
		Assert::IsFalse(equals(createSymmetricTree(false), createAsymmetricTree(false)));
	}

	TEST_METHOD(TestComplex) {
		Assert::IsTrue(equals(createComplex1(false), createComplex1(false)));
	}

	TEST_METHOD(TestComplex1VsComplex2) {
		Assert::IsFalse(equals(createComplex1(false), createComplex2(false)));
	}

	TEST_METHOD(TestComplexRandomized) {
		Assert::IsTrue(equals(createComplex1(true), createComplex1(true)));
	}

	TEST_METHOD(TestComplexRandomized1VsRandomized2) {
		Assert::IsFalse(equals(createComplex1(true, 1), createComplex1(true, 2)));
	}

	TEST_METHOD(TestComplexVsComplexRandomized) {
		Assert::IsFalse(equals(createComplex1(false), createComplex1(true)));
	}

	};

}