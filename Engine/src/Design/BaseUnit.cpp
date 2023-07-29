#include "Engine/pch.h"
#include "../../include/Design/BaseUnit.h"
#include "../../include/Design/CompositeUnit.h"

namespace Reflux::Engine::Design {

	BaseUnit::BaseUnit(UnitId id_, Design& design) : id_(id_), design(&design), parent(nullptr), ports(*this), localPosition_({0, 0}), internalRect_({{0, 0}, {0, 0}}) {}

	// BASEUNIT::PORT

	BaseUnit::Ports::Ports(BaseUnit& unit) : unit(unit) {}

	size_t BaseUnit::Ports::size() const {
		return unit.port_count_();
	}

	Port& BaseUnit::Ports::operator[](size_t index) {
		return unit.get_port_(index);
	}

	BaseUnit::Ports::Iterator BaseUnit::Ports::begin() const {
		return Iterator(unit, 0);
	}

	BaseUnit::Ports::Iterator BaseUnit::Ports::end() const {
		return Iterator(unit, unit.port_count_());
	}

	// BASEUNIT::PORT::ITERATOR

	BaseUnit::Ports::Iterator::Iterator(BaseUnit& unit, size_t index) : unit(unit), index(index) {}

	BaseUnit::Ports::Iterator& BaseUnit::Ports::Iterator::operator++() {
		++index;
		return *this;
	}

	BaseUnit::Ports::Iterator BaseUnit::Ports::Iterator::operator++(int) {
		Iterator tmp(*this);
		++index;
		return tmp;
	}

	BaseUnit::Ports::Iterator& BaseUnit::Ports::Iterator::operator--() {
		--index;
		return *this;
	}

	BaseUnit::Ports::Iterator BaseUnit::Ports::Iterator::operator--(int) {
		Iterator tmp(*this);
		--index;
		return tmp;
	}

	BaseUnit::Ports::Iterator BaseUnit::Ports::Iterator::operator+(difference_type n) const {
		return Iterator(unit, index + n);
	}

	BaseUnit::Ports::Iterator BaseUnit::Ports::Iterator::operator-(difference_type n) const {
		return Iterator(unit, index - n);
	}

	BaseUnit::Ports::Iterator::difference_type BaseUnit::Ports::Iterator::operator-(const Iterator& other) const {
		return index - other.index;
	}

	BaseUnit::Ports::Iterator& BaseUnit::Ports::Iterator::operator+=(difference_type n) {
		index += n;
		return *this;
	}

	BaseUnit::Ports::Iterator& BaseUnit::Ports::Iterator::operator-=(difference_type n) {
		index -= n;
		return *this;
	}

	bool BaseUnit::Ports::Iterator::operator==(const Iterator& other) const {
		return index == other.index && &unit == &other.unit;
	}

	bool BaseUnit::Ports::Iterator::operator!=(const Iterator& other) const {
		return !(*this == other);
	}

	bool BaseUnit::Ports::Iterator::operator<(const Iterator& other) const {
		return index < other.index;
	}

	bool BaseUnit::Ports::Iterator::operator>(const Iterator& other) const {
		return index > other.index;
	}

	bool BaseUnit::Ports::Iterator::operator<=(const Iterator& other) const {
		return index <= other.index;
	}

	bool BaseUnit::Ports::Iterator::operator>=(const Iterator& other) const {
		return index >= other.index;
	}

	BaseUnit::Ports::Iterator::reference BaseUnit::Ports::Iterator::operator[](difference_type n) const {
		return unit.get_port_(index + n);
	}

	BaseUnit::Ports::Iterator::reference BaseUnit::Ports::Iterator::operator*() const {
		return unit.get_port_(index);
	}

	BaseUnit::Ports::Iterator::pointer BaseUnit::Ports::Iterator::operator->() const {
		return &unit.get_port_(index);
	}

	void BaseUnit::set_local_position(Geometry::vector2<int> position) {
		localPosition_ = position;
		if (parent) {
			parent->child_geometry_update(*this);
		}
	}

	void BaseUnit::set_world_position(Geometry::vector2<int> position) {
		if (parent) {
			set_local_position(position - parent->get_world_position());
		} else {
			set_local_position(position);
		}
	}

	Geometry::vector2<int> BaseUnit::get_local_position() {
		return localPosition_;
	}

	Geometry::vector2<int> BaseUnit::get_world_position() {
		if (parent) {
			return localPosition_ + parent->get_world_position();
		} else {
			return localPosition_;
		}
	}


	Geometry::rect<int> BaseUnit::get_local_rect() {
		return internalRect_ + get_local_position();
	}

	Geometry::rect<int> BaseUnit::get_world_rect() {
		return internalRect_ + get_world_position();
	}

} 