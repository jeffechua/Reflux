#pragma once
#include "BaseUnit.h"
#include "Design.h"

namespace Reflux::Engine::Design {

	class Factory;

	class CompositeUnit : public BaseUnit {

		friend Factory;

	public:

		CompositeUnit(UnitId id, Design& design);
		const std::unordered_set<BaseUnit*>& get_units() const;
		const std::unordered_set<Junction*>& get_junctions() const;
		void push(const std::unordered_set<BaseUnit*>& units_);
		void pop(const std::unordered_set<BaseUnit*>& units_);
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
		size_t port_count_() const override;
		Port& get_port_(size_t i) override;
		// New members
		std::unordered_set<BaseUnit*> units_;
		std::unordered_set<Junction*> junctions_;
		std::unordered_map<Junction*, std::unique_ptr<Port>> exports_;
		std::unordered_map<Port*, Junction*> exportsReverseLookup_;
		Port& add_export_raw_(Junction& junction);
		std::unique_ptr<Port> remove_export_raw_(Junction& junction);

	};

}