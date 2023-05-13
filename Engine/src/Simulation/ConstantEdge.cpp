#include "Engine/pch.h"
#include "../../include/Simulation/ConstantEdge.h"

namespace Reflux::Engine::Simulation {

	ConstantEdge::ConstantEdge(double R, double e) : R_(R), e_(e) {}

	double ConstantEdge::R() const {
		return R_;
	}

	double ConstantEdge::e() const {
		return e_;
	}

}