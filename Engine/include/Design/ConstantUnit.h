#pragma once
#include "BaseUnit.h"
#include "../Simulation/ConstantEdge.h"
#include <array>

namespace Reflux::Engine::Design {

	class ConstantUnit : public BaseUnit {

	public:
		enum Type {
			RESISTOR,
			CELL,
			RESISTIVE_CELL
		};
		Type type;
		double R;
		double e;
		ConstantUnit(UnitId id, Design& design, Type type, double R, double e);
		std::string name() const override;
		int node_count() const override;
		int edge_count() const override;
		void render(Simulation::GraphBuilder& graphBuilder, const std::vector<Simulation::Node*>& bindings) const override;
		bool validate(std::ostream& output) const override;

	private:
		std::array<Port, 2> portsArray;
		size_t port_count() const override;
		Port& get_port(size_t i) override;

	};

}