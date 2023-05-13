#include "Engine/pch.h"
#include "../../include/Design/CompositeUnit.h"
#include "../../include/Design/Design.h"
#include <format>
#include <iostream>

namespace Reflux::Engine::Design {

	// PUBLIC

	CompositeUnit::CompositeUnit(UnitId id, Design& design) : BaseUnit(id, design), exports{}, exportsReverseLookup{} {}

	Junction& CompositeUnit::create_junction() {
		Junction& newJunction = design->create_junction();
		newJunction.parent = this;
		junctions.insert(&newJunction);
		return newJunction;
	}

	// Removing units can remove internal-port/external-junc bindings or create external-port/internal-junc bindings
	// Removing juncs can create internal-port/external-junc bindings or remove external-port/internal-junc bindings
	// Adding   units can create internal-port/external-junc bindings or remove external-port/internal-junc bindings
	// Adding   juncs can remove internal-port/external-junc bindings or create external-port/internal-junc bindings
	// Int-port/ext-junc is resolvable by exporting the port and rebinding the exported port to the external junc.
	// Ext-port/int-junc is not resolvable as we can't export junctions.
	// Therefore, adding junctions or removing units can create violations.
	std::vector<Junction*> CompositeUnit::get_push_boundary_violations(const std::unordered_set<Junction*>& junctionsToPush, const std::unordered_set<BaseUnit*>& unitsToPush) const {
		std::vector<Junction*> violators{};
		// Adding units can create internal-port/external-junc bindings or remove external-port/internal-junc bin
		for (Junction* junction : junctionsToPush) {
			for (Port* port : junction->ports) {
				// Pushing a junction which is bound to an external unit creates a violation
				// So the junction must either by bound to me, or to one of the other pushed units
				if (!(exportsReverseLookup.contains(port) || unitsToPush.contains(&port->unit))) {
					violators.push_back(junction);
					break;
				}
			}
		}
		return violators;
	}

	std::vector<BaseUnit*> CompositeUnit::get_pop_boundary_violations(const std::unordered_set<Junction*>& junctionsToPop, const std::unordered_set<BaseUnit*>& unitsToPop) const {
		std::vector<BaseUnit*> violators{};
		for (BaseUnit* unit : unitsToPop) {
			for (Port& port : unit->ports) {
				// Popping a unit which is bound to an internal junction creates a violation
				if (junctions.contains(port.junction) && !junctionsToPop.contains(port.junction)) {
					violators.push_back(unit);
					break;
				}
			}
		}
		return violators;
	}

	void CompositeUnit::push(const std::unordered_set<Junction*>& junctionsToPush, const std::unordered_set<BaseUnit*>& unitsToPush) {
		if (get_push_boundary_violations(junctionsToPush, unitsToPush).size() > 0) {
			throw std::runtime_error("Group is not pushable; it creates junctions connected to external units.");
		}
		// Add junctions
		for (Junction* junction : junctionsToPush) {
			if (junction->parent != parent) {
				throw std::runtime_error(std::format("Junction %d is not a sibling of \"%s\"", junction->id, name()).c_str());
			}
			junctions.emplace(junction);
			if (junction->parent) {
				junction->parent->junctions.erase(junction);
			}
			junction->parent = this;
			// Clone ports as we mutate junction->ports by rebinding
			std::vector<Port*> ports{ junction->ports.begin(), junction->ports.end() };
			for (Port* port : ports) {
				// A pushed junction may be bound to the following types of port:
				//  1. One of the newly added units: no change
				//  2. One of my exported ports: need to unexport the port.
				// It CANNOT be bound to an old internal port; that's a boundary violation.
				if (&port->unit == this) {
					Port* internalPort = exportsReverseLookup.at(port);
					Port* exportedPort = port;
					internalPort->bind(exportedPort->unbind(false), false);
					removeExport(*internalPort);
				}
			}
		}
		// Add units
		for (BaseUnit* unit : unitsToPush) {
			if (unit->parent != parent) {
				throw std::runtime_error(std::format("%s is not a sibling of \"%s\"", unit->name(), name()).c_str());
			}
			units.emplace(unit);
			if (unit->parent) {
				unit->parent->units.erase(unit);
			}
			unit->parent = this;
			// Do not need to clone ports as unit ports don't change on rebinding.
			for (Port& port : unit->ports) {
				// A pushed unit's port can only be bound to:
				//  1. One of the newly added junctions: no change.
				//  2. An external junction: need to export port and rebind.
				//  3. Nothing: it's exported by the parent CompositeUnit: need to be export but not rebind.
				// It CANNOT be bound to an old internal junction; that's a boundary violation.
				if (!junctions.contains(port.junction)) {
					Port& exportedPort = addExport(port, std::make_unique<Port>(*this, port.outgoing));
					if (port.is_bound()) {
						exportedPort.bind(port.unbind(false), false);
					} else if(parent) {
						parent->notify_internal_port_switch(port, exportedPort);
					}
				}
			}
		}
	}

	void CompositeUnit::pop(const std::unordered_set<Junction*>& junctionsToPop, const std::unordered_set<BaseUnit*>& unitsToPop) {
		if (get_pop_boundary_violations(junctionsToPop, unitsToPop).size() > 0) {
			throw std::runtime_error("Group is not pushable; it creates junctions connected to external units.");
		}
		// Remove junctions
		for (Junction* junction : junctionsToPop) {
			if (junction->parent != this) {
				throw std::runtime_error(std::format("Junction %d is not a child of \"%s\"", junction->id, name()).c_str());
			}
			junctions.erase(junction);
			if (parent) {
				parent->junctions.emplace(junction);
			}
			junction->parent = parent;
			// Clone ports as we mutate junction->ports by rebinding
			std::vector<Port*> ports{ junction->ports.begin(), junction->ports.end() };
			for (Port* port : ports) {
				// A popped junction may be bound to the following types of port:
				//  1. An internal port in the pop group: no change.
				//  2. An internal port not in the pop group: need to export the port.
				// It CANNOT be bound to an external port; that's a boundary violation.
				if (!unitsToPop.contains(&port->unit)) {
					Port& internalPort = *port;
					Port& exportedPort = addExport(*port, std::make_unique<Port>(*this, internalPort.outgoing));
					exportedPort.bind(internalPort.unbind(false), false);
				}
			}
		}
		// Remove units
		for (BaseUnit* unit : unitsToPop) {
			if (unit->parent != this) {
				throw std::runtime_error(std::format("%s is not a child of \"%s\"", unit->name(), name()).c_str());
			}
			units.erase(unit);
			if (parent) {
				parent->units.emplace(unit);
			}
			unit->parent = parent;
			// Do not need to clone ports as unit ports don't change on rebinding.
			for (Port& port : unit->ports) {
				// A removed unit's port can only be bound to:
				//  1. One of the popped junctions: no change.
				//  2. An external junction, via an export: need to unexport the port and rebind.
				//  3. Nothing, via an export: need to unexport but not rebind.
				// It CANNOT be bound to a non-popped internal junction; that's a boundary violation.
				if (!port.is_bound()) { // (i.e. is exported)
					Port& exportedPort = *exports.at(&port);
					if (exportedPort.is_bound()) {
						port.bind(exportedPort.unbind(false), false);
					} else if (parent) {
						parent->notify_internal_port_switch(exportedPort, port);
					}
					removeExport(port);
				}
			}
		}
	}

	void CompositeUnit::notify_new_unbound_internal_port(Port& port) {
		Port& exportedPort = addExport(port, std::make_unique<Port>(*this, port.outgoing));
		if (parent) {
			parent->notify_new_unbound_internal_port(exportedPort);
		}
	}

	void CompositeUnit::notify_removed_unbound_internal_port(Port& port) {
		if (exports.at(&port)->is_bound()) {
			exports.at(&port)->unbind(false);
		} else if (parent) {
			parent->notify_removed_unbound_internal_port(*exports.at(&port));
		}
		removeExport(port);
	}

	void CompositeUnit::notify_internal_port_switch(Port& oldPort, Port& newPort) {
		addExport(newPort, removeExport(oldPort));
	}

	std::string CompositeUnit::name() const {
		return std::format("Composite Unit %d/%d/%d (%d)", units.size(), junctions.size(), exports.size(), id);
	}

	int CompositeUnit::node_count() const {
		int count = static_cast<int>(junctions.size());
		for (auto unit = units.begin(); unit != units.end(); unit++) {
			count += (*unit)->node_count();
		}
		return count;
	}

	int CompositeUnit::edge_count() const {
		int count = 0;
		for (auto unit = units.begin(); unit != units.end(); unit++) {
			count += (*unit)->edge_count();
		}
		return count;
	}

	void CompositeUnit::render(Simulation::GraphBuilder& graphBuilder, const std::vector<Simulation::Node*>& bindings) const {
		std::unordered_map<const Junction*, Simulation::Node*> nodeLookup;
		std::unordered_map<const Port*, Simulation::Node*> externalBindingLookup{};
		// Don't use ports iterators as its iterator is non-const to enable returning a non-const Port&. :/
		for (auto i = 0; i < port_count(); i++) {
			Port* port = std::next(exports.begin(), i)->first;
			externalBindingLookup[port] = bindings[i];
		}
		for (Junction* junction : junctions) {
			nodeLookup[junction] = &graphBuilder.add_node();
		}
		for (BaseUnit* unit : units) {
			std::vector<Simulation::Node*> unitBindings;
			unitBindings.reserve(unit->ports.size());
			for (Port& port : unit->ports) {
				Simulation::Node* boundNode;
				// If port is unbound that means it's exported
				if (port.is_bound()) {
					boundNode = nodeLookup.at(port.junction);
				} else {
					boundNode = externalBindingLookup.at(&port);
				}
				unitBindings.push_back(boundNode);
			}
			unit->render(graphBuilder, unitBindings);
		}
	}

	bool CompositeUnit::validate(std::ostream& output) const {
		bool pass = true;
		for (BaseUnit* unit : units) {
			for (Port& port : unit->ports) {
				if (port.is_bound()) {
					if (!junctions.contains(port.junction)) {
						output << ("Internal port bound to external junction.");
						pass = false;
					}
				} else {
					if (!exports.contains(&port)) {
						output << ("Unbound internal port not exported.");
						pass = false;
					}
				}
			}
			pass &= unit->validate(output);
		}
		for (Junction* junction : junctions) {
			for (Port* port : junction->ports) {
				if (!units.contains(&port->unit)) {
					output << ("External unit bound to internal junction.");
					pass = false;
				}
			}
		}
		if (exports.size() != exportsReverseLookup.size()) {
			output << ("exportsReverseLookup different length from exports");
			pass = false;
		}
		for (auto exp = exports.begin(); exp != exports.end(); exp++) {
			if (!exportsReverseLookup.contains(exp->second.get()) || exportsReverseLookup.at(exp->second.get()) != exp->first) {
				output << ("exportsReverseLookup inconsistent with exports.");
				pass = false;
			}
		}
		return pass;
	}

	// PRIVATE

	size_t CompositeUnit::port_count() const {
		return static_cast<int>(exports.size());
	}

	Port& CompositeUnit::get_port(size_t i) {
		return *std::next(exports.begin(), i)->second;
	}

	// Cannot take const internalPort as reverseLookup needs to be able to return a non-const internal port
	Port& CompositeUnit::addExport(Port& internalPort, std::unique_ptr<Port>&& exportedPort) {
		Port* exportPtr = exportedPort.get();
		if (!exports.emplace(&internalPort, std::move(exportedPort)).second) {
			throw std::runtime_error("internalPort is already exported.");
		}
		if (!exportsReverseLookup.emplace(exportPtr, &internalPort).second) {
			throw std::runtime_error("exportedPort is already registered.");
		}
		return *exportPtr;
	}

	std::unique_ptr<Port> CompositeUnit::removeExport(Port& internalPort) {
		if (!exports.contains(&internalPort)) {
			throw std::runtime_error("Port is not exported.");
		}
		std::unique_ptr<Port> exportedPort = std::move(exports.at(&internalPort));
		exports.erase(&internalPort);
		if (!exportsReverseLookup.erase(exportedPort.get())) {
			throw std::runtime_error("Exported port was already removed.");
		}
		return exportedPort;
	}

}