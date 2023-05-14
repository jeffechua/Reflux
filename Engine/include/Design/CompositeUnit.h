#pragma once
#include "BaseUnit.h"
#include "Design.h"

namespace Reflux::Engine::Design {

	class CompositeUnit : public BaseUnit {

	public:
		std::unordered_set<BaseUnit*> units;
		std::unordered_set<Junction*> junctions;

		CompositeUnit(UnitId id, Design& design);
		void push(const std::unordered_set<BaseUnit*>& units);
		void pop(const std::unordered_set<BaseUnit*>& units);
		Port& get_export(Junction& junction);
		Junction& add_export(Junction& junction);
		void remove_export(Junction& junction);

		// BaseUnit implementations
		std::string name() const override;
		int node_count() const override;
		int edge_count() const override;
		void render(Simulation::GraphBuilder& graphBuilder, const std::vector<Simulation::Node*>& bindings) const override;
		bool validate(std::ostream& output) const override;

	private:
		// BaseUnit implementations
		size_t port_count() const override;
		Port& get_port(size_t i) override;
		// New members
		std::unordered_map<Junction*, std::unique_ptr<Port>> exports;
		std::unordered_map<Port*, Junction*> exportsReverseLookup;
		Port& add_export_raw(Junction& junction);
		std::unique_ptr<Port> remove_export_raw(Junction& junction);

	};

}