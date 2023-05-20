#pragma once
#include "Design.h"
#include "CompositeUnit.h"

namespace Reflux::Engine::Design {

	class Factory {

	private:

		struct CreationContext {
			Design& design;
			CompositeUnit& container;
			Junction& create_junction();
			template<typename TUnit, typename...TConstructorArgs>
			TUnit& create_unit(TConstructorArgs... args) {
				TUnit& newUnit = design.make_unit<TUnit>(args...);
				newUnit.parent = &container;
				container.units.insert(&newUnit);
				for (Port& port : newUnit.ports) {
					Junction& junction = create_junction();
					port.bind(junction);
				}
				return newUnit;
			}
		};

	public:

		static CreationContext in(CompositeUnit& container);
		static void destroy(BaseUnit& unit);
		static void destroy(Junction& junction);

	};

}