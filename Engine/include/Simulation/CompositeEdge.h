#pragma once
#include "BaseEdge.h"

namespace Reflux::Engine::Simulation {

	class CompositeEdge : public BaseEdge {

	public:
		CompositeEdge(std::vector<BaseEdge*> constituents);
		BaseEdge& bind(Node& from, Node& to) override;
		double R() const override;
		double e() const override;

	private:
		const std::vector<BaseEdge*> constituents;
		std::vector<bool> inverse;

	};

}