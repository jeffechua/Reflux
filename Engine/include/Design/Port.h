#pragma once
#include "Junction.h"

namespace Reflux::Engine::Design {

	class BaseUnit;

	struct Port {

		Port(const Port&) = delete;
		Port(Port&&) = default;
		Port& operator=(const Port&) = delete;
		Port& operator=(Port&&) = default;

		BaseUnit& unit;
		Junction* junction;
		bool outgoing;
		Port(BaseUnit& unit, bool outgoing);
		bool is_bound() const;
		Junction& detach();
		void bind(Junction& to);
		Junction& unbind();

	};

}