#pragma once
#include <vector>
#include <string>
#include "Port.h"
#include "../Geometry/rect.h"
#include "../Simulation/GraphBuilder.h"

namespace Reflux::Engine::Design {

	class Design;

	typedef unsigned int UnitId;

	class BaseUnit {

		friend CompositeUnit;

	public:

		class Ports {
		public:
			class Iterator;
			Ports(BaseUnit& unit);
			size_t size() const;
			Port& operator[](size_t index);
			Iterator begin() const;
			Iterator end() const;
		private:
			BaseUnit& unit;
		};

		UnitId id_;
		CompositeUnit* parent;
		Design* design;
		Ports ports;
		BaseUnit(UnitId id_, Design& design);
		
		void set_local_position(Geometry::vector2<int> position);
		void set_world_position(Geometry::vector2<int> position);
		Geometry::vector2<int> get_local_position();
		Geometry::vector2<int> get_world_position();
		Geometry::rect<int> get_local_rect();
		Geometry::rect<int> get_world_rect();
		virtual bool contains(Geometry::rect<int> rect, bool rectIsLocal = false, bool countTouch = true);
		virtual bool contained_by(Geometry::rect<int> rect, bool rectIsLocal = false, bool countTouch = true);
		virtual bool contains(Geometry::vector2<int> position, bool positionIsLocal = false, bool countTouch = true);
		virtual bool overlaps(Geometry::rect<int> rect, bool rectIsLocal = false, bool countTouch = true);

		virtual std::string name() const = 0;
		virtual int node_count() const = 0;
		virtual int edge_count() const = 0;
		virtual bool validate(std::ostream& output) const = 0;
		/// <summary>
		/// </summary>
		/// <param name="nodes">Node vector to push `nodeCount()` nodes to.</param>
		/// <param name="edges">Edge vector to push `edgeCount()` edges to.</param>
		/// <param name="bindings">Node vector of size `portCount` to which ports are to be bound, appending to `incoming` if `port.outgoing`, else to `outgoing`.</param>
		virtual void render(Simulation::GraphBuilder& graphBuilder, const std::vector<Simulation::Node*>& bindings) const = 0;

	protected:
		Geometry::vector2<int> localPosition_;
		Geometry::rect<int> internalRect_;
		virtual size_t port_count_() const = 0;
		virtual Port& get_port_(size_t i) = 0;

	};

	class BaseUnit::Ports::Iterator {

	public:
		using value_type = Port;
		using difference_type = std::ptrdiff_t;
		using pointer = Port*;
		using reference = Port&;
		using iterator_category = std::random_access_iterator_tag;

		Iterator(BaseUnit& unit, size_t index);
		Iterator& operator++();
		Iterator operator++(int);
		Iterator& operator--();
		Iterator operator--(int);
		Iterator operator+(difference_type n) const;
		Iterator operator-(difference_type n) const;
		difference_type operator-(const Iterator& other) const;
		Iterator& operator+=(difference_type n);
		Iterator& operator-=(difference_type n);
		bool operator==(const Iterator& other) const;
		bool operator!=(const Iterator& other) const;
		bool operator<(const Iterator& other) const;
		bool operator>(const Iterator& other) const;
		bool operator<=(const Iterator& other) const;
		bool operator>=(const Iterator& other) const;
		reference operator[](difference_type n) const;
		reference operator*() const;
		pointer operator->() const;

	private:
		BaseUnit& unit;
		size_t index;

	};

}