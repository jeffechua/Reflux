#pragma once
#include "Engine/include/Simulation/Graph.h"

using namespace Reflux::Engine::Simulation;

namespace Reflux::Engine::TestUtils {

	Graph createAndPopulateGraph(size_t nodeCount, size_t edgeCount, bool randomizeEdges, int seed = 0);

	Graph createOneNode(bool randomizeEdges, int seed = 0);

	Graph createOneEdge(bool randomizeEdges, int seed = 0);

	Graph createTwoEdges(bool randomizeEdges, int seed = 0);

	Graph createThreeEdges(bool randomizeEdges, int seed = 0);

	Graph createTwoEdgesConverging(bool randomizeEdges, int seed = 0);

	Graph createTriangle(bool randomizeEdges, int seed = 0);

	Graph createSymmetricTree(bool randomizeEdges, int seed = 0);

	Graph createAsymmetricTree(bool randomizeEdges, int seed = 0);

	Graph createComplex1(bool randomizeEdges, int seed = 0);

	Graph createComplex2(bool randomizeEdges, int seed = 0);

}