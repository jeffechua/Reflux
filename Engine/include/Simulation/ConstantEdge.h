#pragma once
#include "BaseEdge.h"

namespace Reflux::Engine::Simulation {

	class ConstantEdge : public BaseEdge {

	public:
		ConstantEdge(double R, double e);
		double R() const override;
		double e() const override;

	private:
		const double R_;
		const double e_;

	};

}