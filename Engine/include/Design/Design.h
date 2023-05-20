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
		std::unordered_map<UnitId, std::unique_ptr<BaseUnit>> units;
		std::unordered_map<JunctionId, std::unique_ptr<Junction>> junctions;
		Design(std::string name = "MyDesign");
		bool contains_unit(UnitId id_);
		bool contains_junction(JunctionId id_);
		BaseUnit& get_unit(UnitId id_);
		Junction& get_junction(JunctionId id_);
		Simulation::Graph build();
		bool validate(std::ostream& output) const;

	private:
		UnitId nextUnitId;
		JunctionId nextJunctionId;
		template<typename T, typename... TConstructorArgs>
		T& make_unit(TConstructorArgs... args);
		Junction& make_junction();
		void recursively_unregister(BaseUnit* unit);

	};

	// IMPLEMENTATION

	template<typename T, typename... TConstructorArgs>
	T& Design::make_unit(TConstructorArgs... args) {
		T* ptr = new T(nextUnitId++, *this, args...);
		units[ptr->id_] = std::unique_ptr<BaseUnit>(ptr);
		return *ptr;
	}

}