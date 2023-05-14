#pragma once
#include <memory>
#include <unordered_map>
#include "BaseUnit.h"
#include "../Simulation/Graph.h"

namespace Reflux::Engine::Design {

	class CompositeUnit;

	class Factory;

	class Design {

		friend Factory;

	public:

		std::string name;
		CompositeUnit* root;
		Design(std::string name = "MyDesign");
		const std::unordered_map<UnitId, std::unique_ptr<BaseUnit>>& get_units() const;
		const std::unordered_map<UnitId, std::unique_ptr<Junction>>& get_junctions() const;
		bool contains_unit(UnitId id);
		bool contains_junction(JunctionId id);
		BaseUnit& get_unit(UnitId id);
		Junction& get_junction(JunctionId id);
		Simulation::Graph build();
		bool validate(std::ostream& output) const;

	private:
		UnitId nextUnitId;
		JunctionId nextJunctionId;
		std::unordered_map<UnitId, std::unique_ptr<BaseUnit>> units_;
		std::unordered_map<JunctionId, std::unique_ptr<Junction>> junctions_;
		template<typename T, typename... TConstructorArgs>
		T& make_unit(TConstructorArgs... args);
		Junction& make_junction();
		void recursively_unregister(BaseUnit* unit);

	};

	// IMPLEMENTATION

	template<typename T, typename... TConstructorArgs>
	T& Design::make_unit(TConstructorArgs... args) {
		T* ptr = new T(nextUnitId++, *this, args...);
		units_[ptr->id] = std::unique_ptr<BaseUnit>(ptr);
		return *ptr;
	}

}