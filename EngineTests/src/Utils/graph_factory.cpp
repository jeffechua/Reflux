#include "EngineTests/pch.h"
#include "Engine/include/Simulation/GraphBuilder.h"
#include "Engine/include/Simulation/ConstantEdge.h"
#include "../../include/Utils/graph_factory.h"

using namespace Reflux::Engine::Simulation;

namespace Reflux::Engine::TestUtils {

	Graph createAndPopulateGraph(size_t nodeCount, size_t edgeCount, bool randomizeEdges, int seed) {
		GraphBuilder builder(nodeCount, edgeCount);
		std::srand(seed);
		for (size_t i = 0; i < nodeCount; i++) {
			builder.add_node();
		}
		for (size_t i = 0; i < edgeCount; i++) {
			builder.add_edge<ConstantEdge>(randomizeEdges ? std::rand() : 0, randomizeEdges ? std::rand() : 0);
		}
		return builder.build();
	}

	Graph createOneNode(bool randomizeEdges, int seed) {
		return createAndPopulateGraph(1, 0, randomizeEdges, seed);
	}

	Graph createOneEdge(bool randomizeEdges, int seed) {
		Graph graph = createAndPopulateGraph(2, 1, randomizeEdges, seed);
		graph.edges[0]->bind(*graph.nodes[0], *graph.nodes[1]);
		return graph;
	}

	Graph createTwoEdges(bool randomizeEdges, int seed) {
		Graph graph = createAndPopulateGraph(3, 2, randomizeEdges, seed);
		graph.edges[0]->bind(*graph.nodes[0], *graph.nodes[1]);
		graph.edges[1]->bind(*graph.nodes[1], *graph.nodes[2]);
		return graph;
	}

	Graph createThreeEdges(bool randomizeEdges, int seed) {
		Graph graph = createAndPopulateGraph(4, 3, randomizeEdges, seed);
		graph.edges[0]->bind(*graph.nodes[0], *graph.nodes[1]);
		graph.edges[1]->bind(*graph.nodes[1], *graph.nodes[2]);
		graph.edges[2]->bind(*graph.nodes[2], *graph.nodes[3]);
		return graph;
	}

	Graph createTwoEdgesConverging(bool randomizeEdges, int seed) {
		Graph graph = createAndPopulateGraph(3, 2, randomizeEdges, seed);
		graph.edges[0]->bind(*graph.nodes[0], *graph.nodes[1]);
		graph.edges[1]->bind(*graph.nodes[2], *graph.nodes[1]);
		return graph;
	}

	Graph createTriangle(bool randomizeEdges, int seed) {
		Graph graph = createAndPopulateGraph(3, 3, randomizeEdges, seed);
		graph.edges[0]->bind(*graph.nodes[0], *graph.nodes[1]);
		graph.edges[1]->bind(*graph.nodes[1], *graph.nodes[2]);
		graph.edges[2]->bind(*graph.nodes[2], *graph.nodes[0]);
		return graph;
	}

	Graph createSymmetricTree(bool randomizeEdges, int seed) {
		Graph graph = createAndPopulateGraph(5, 4, randomizeEdges, seed);
		graph.edges[0]->bind(*graph.nodes[0], *graph.nodes[1]);
		graph.edges[1]->bind(*graph.nodes[1], *graph.nodes[2]);
		graph.edges[2]->bind(*graph.nodes[1], *graph.nodes[3]);
		graph.edges[3]->bind(*graph.nodes[1], *graph.nodes[4]);
		return graph;
	}

	Graph createAsymmetricTree(bool randomizeEdges, int seed) {
		Graph graph = createAndPopulateGraph(5, 4, randomizeEdges, seed);
		graph.edges[0]->bind(*graph.nodes[0], *graph.nodes[1]);
		graph.edges[1]->bind(*graph.nodes[1], *graph.nodes[2]);
		graph.edges[2]->bind(*graph.nodes[2], *graph.nodes[3]);
		graph.edges[3]->bind(*graph.nodes[1], *graph.nodes[4]);
		return graph;
	}

	Graph createComplex1(bool randomizeEdges, int seed) {
		/*    9
			  |
			5 6-7-8
			| |/  |
			3-4   |
			| |   |
			0-1---2
		*/
		Graph graph = createAndPopulateGraph(10, 12, randomizeEdges, seed);
		size_t i = 0;
		graph.edges[i++]->bind(*graph.nodes[0], *graph.nodes[1]);
		graph.edges[i++]->bind(*graph.nodes[0], *graph.nodes[3]);
		graph.edges[i++]->bind(*graph.nodes[1], *graph.nodes[2]);
		graph.edges[i++]->bind(*graph.nodes[1], *graph.nodes[4]);
		graph.edges[i++]->bind(*graph.nodes[2], *graph.nodes[8]);
		graph.edges[i++]->bind(*graph.nodes[3], *graph.nodes[4]);
		graph.edges[i++]->bind(*graph.nodes[3], *graph.nodes[5]);
		graph.edges[i++]->bind(*graph.nodes[4], *graph.nodes[6]);
		graph.edges[i++]->bind(*graph.nodes[4], *graph.nodes[7]);
		graph.edges[i++]->bind(*graph.nodes[6], *graph.nodes[9]);
		graph.edges[i++]->bind(*graph.nodes[6], *graph.nodes[7]);
		graph.edges[i++]->bind(*graph.nodes[7], *graph.nodes[8]);
		return graph;
	}

	Graph createComplex2(bool randomizeEdges, int seed) {
		/*    9
			  |\
			5 6-7 8
			| |/  |
			3-4   |
			| |   |
			0-1---2
		*/
		Graph graph = createAndPopulateGraph(10, 12, randomizeEdges, seed);
		size_t i = 0;
		graph.edges[i++]->bind(*graph.nodes[0], *graph.nodes[1]);
		graph.edges[i++]->bind(*graph.nodes[0], *graph.nodes[3]);
		graph.edges[i++]->bind(*graph.nodes[1], *graph.nodes[2]);
		graph.edges[i++]->bind(*graph.nodes[1], *graph.nodes[4]);
		graph.edges[i++]->bind(*graph.nodes[2], *graph.nodes[8]);
		graph.edges[i++]->bind(*graph.nodes[3], *graph.nodes[4]);
		graph.edges[i++]->bind(*graph.nodes[3], *graph.nodes[5]);
		graph.edges[i++]->bind(*graph.nodes[4], *graph.nodes[6]);
		graph.edges[i++]->bind(*graph.nodes[4], *graph.nodes[7]);
		graph.edges[i++]->bind(*graph.nodes[6], *graph.nodes[9]);
		graph.edges[i++]->bind(*graph.nodes[6], *graph.nodes[7]);
		graph.edges[i++]->bind(*graph.nodes[7], *graph.nodes[9]);
		return graph;
	}

}