#include "Engine/pch.h"
#include "../../include/Design/BaseUnit.h"

namespace Reflux::Engine::Design {
	
	BaseUnit::BaseUnit(UnitId id, Design& design) : id(id), design(&design), parent(nullptr), ports(*this) {}
    
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

}