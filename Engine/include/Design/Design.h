#pragma once
#include <memory>
#include <unordered_map>
#include "BaseUnit.h"
#include "../Simulation/Graph.h"

namespace Reflux::Engine::Design {

	class CompositeUnit;

	namespace Factory {
		struct CreationContext;
	}

	class Design {

		friend Factory::CreationContext;

	public:

		std::string name;
		CompositeUnit* root;
		Design(std::string name = "MyDesign");
		void destroy(BaseUnit& unit);
		void destroy(Junction& junction);
		bool contains_unit(UnitId id);
		bool contains_junction(JunctionId id);
		BaseUnit& get_unit(UnitId id);
		Junction& get_junction(JunctionId id);
		Simulation::Graph build();
		bool validate(std::ostream& output) const;

	private:
		UnitId nextUnitId;
		JunctionId nextJunctionId;
		std::unordered_map<UnitId, std::unique_ptr<BaseUnit>> units;
		std::unordered_map<JunctionId, std::unique_ptr<Junction>> junctions;
		template<typename T, typename... TConstructorArgs>
		T& make_unit(TConstructorArgs... args);
		Junction& make_junction();
		void recursively_unregister(BaseUnit* unit);

	};

	// IMPLEMENTATION

	template<typename T, typename... TConstructorArgs>
	T& Design::make_unit(TConstructorArgs... args) {
		T* ptr = new T(nextUnitId++, *this, args...);
		units[ptr->id] = std::unique_ptr<BaseUnit>(ptr);
		return *ptr;
	}

}