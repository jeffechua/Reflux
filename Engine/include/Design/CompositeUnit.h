#pragma once
#include "BaseUnit.h"
#include "Design.h"
#include "../Geometry/aabb.h"

namespace Reflux::Engine::Design {

	class Factory;

	class CompositeUnit : public BaseUnit {

		friend Factory;

	public:

		CompositeUnit(UnitId id_, Design& design);
		std::unordered_set<BaseUnit*> units;
		std::unordered_set<Junction*> junctions;
		void push(const std::unordered_set<BaseUnit*>& units);
		void pop(const std::unordered_set<BaseUnit*>& units);
		Port& get_export(Junction& junction);
		Junction& add_export(Junction& junction);
		void remove_export(Junction& junction);
		void child_geometry_update(BaseUnit& unit);

		// BaseUnit implementations
		std::string name() const override;
		int node_count() const override;
		int edge_count() const override;
		void render(Simulation::GraphBuilder& graphBuilder, const std::vector<Simulation::Node*>& bindings) const override;
		bool validate(std::ostream& output) const override;

	private:
		Geometry::tree<void*, int> tree_;
		// BaseUnit implementations
		size_t port_count_() const override;
		Port& get_port_(size_t i) override;
		// New members
		std::unordered_map<Junction*, std::unique_ptr<Port>> exports_;
		std::unordered_map<Port*, Junction*> exportsReverseLookup_;
		Port& add_export_raw_(Junction& junction);
		std::unique_ptr<Port> remove_export_raw_(Junction& junction);
		void register_raw_(BaseUnit& unit);
		void register_raw_(Junction& junction);
		void unregister_raw_(BaseUnit& unit);
		void unregister_raw_(Junction& junction);
		void recalculate_own_rect();
	};

}