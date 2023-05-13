#pragma once
#include "BaseUnit.h"
#include "Design.h"

namespace Reflux::Engine::Design {

	class CompositeUnit : public BaseUnit {

	public:
		std::unordered_set<BaseUnit*> units;
		std::unordered_set<Junction*> junctions;

		CompositeUnit(UnitId id, Design& design);
		template<typename T, typename... TConstructorArgs>
		T& create(TConstructorArgs... args);
		Junction& create_junction();
		std::vector<Junction*> get_push_boundary_violations(const std::unordered_set<Junction*>& junctionsToPush, const std::unordered_set<BaseUnit*>& unitsToPush) const;
		std::vector<BaseUnit*> get_pop_boundary_violations(const std::unordered_set<Junction*>& junctionsToPop, const std::unordered_set<BaseUnit*>& unitsToPop) const;
		void push(const std::unordered_set<Junction*>& junctionsToPush, const std::unordered_set<BaseUnit*>& unitsToPush);
		void pop(const std::unordered_set<Junction*>& junctionsToPop, const std::unordered_set<BaseUnit*>& unitsToPop);
		void notify_new_unbound_internal_port(Port& port);
		void notify_removed_unbound_internal_port(Port& port);
		void notify_internal_port_switch(Port& oldPort, Port& newPort);

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
		std::unordered_map<Port*, std::unique_ptr<Port>> exports;
		std::unordered_map<Port*, Port*> exportsReverseLookup;
		Port& addExport(Port& internalPort, std::unique_ptr<Port>&& exportedPort);
		std::unique_ptr<Port> removeExport(Port& internalPort);

	};

	// IMPLEMENTATION

	template<typename T, typename... TConstructorArgs>
	T& CompositeUnit::create(TConstructorArgs... args) {
		T& newUnit = design->create<T>(args...);
		newUnit.parent = this;
		units.insert(&newUnit);
		for (Port& port : newUnit.ports) {
			notify_new_unbound_internal_port(port);
		}
		return newUnit;
	}

}