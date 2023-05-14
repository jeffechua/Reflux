#include "Engine/pch.h"
#include <format>
#include <iostream>
#include <algorithm>
#include "../../include/Design/Factory.h"
#include "../../include/Design/CompositeUnit.h"
#include "../../include/Design/Design.h"

namespace Reflux::Engine::Design {

	// PUBLIC

	CompositeUnit::CompositeUnit(UnitId id, Design& design) : BaseUnit(id, design), exports_{}, exportsReverseLookup_{} {}

	const std::unordered_set<BaseUnit*>& CompositeUnit::get_units() const {
		return units_;
	}
	const std::unordered_set<Junction*>& CompositeUnit::get_junctions() const {
		return junctions_;
	}

	void CompositeUnit::push(const std::unordered_set<BaseUnit*>& unitsToPush) {
		if (parent == nullptr) {
			throw std::runtime_error("Cannot push to root unit.");
		}
		struct JunctionPushInfo {
			Port* portFromMe = nullptr;
			std::vector<Port*> portsFromUnitsToPush{};
		};
		std::unordered_map<Junction*, JunctionPushInfo> touchedJunctionsInfo{};
		for (BaseUnit* unit : unitsToPush) {
			// Claim the unit
			if (unit->parent != parent) {
				throw std::runtime_error(std::format("%s is not a sibling of \"%s\"", unit->name(), name()).c_str());
			}
			unit->parent->units_.erase(unit);
			units_.emplace(unit);
			unit->parent = this;
			// Scan ports for touched junctions
			for (Port& port : unit->ports) {
				// implicitly create JunctionInfo if not already present
				touchedJunctionsInfo[port.junction].portsFromUnitsToPush.push_back(&port);
			}
		}
		// Scan own ports for touching touched junctions
		for (Port& port : ports) {
			if (touchedJunctionsInfo.contains(port.junction)) {
				Port*& portFromMe = touchedJunctionsInfo.at(port.junction).portFromMe;
				if (portFromMe) {
					throw std::runtime_error(std::format("Junction %d is bound to a \"%s\" of multiple times", port.junction->id, name()).c_str());
				} else {
					portFromMe = &port;
				}
			}
		}
		// Handle touched junctions
		for (const auto& [junction, info] : touchedJunctionsInfo) {
			if (junction->parent != parent) {
				throw std::runtime_error(std::format("Junction %d is not a child of \"%s\"", junction->id, name()).c_str());
			}
			bool isBoundToMe = info.portFromMe;
			bool isBoundToUnpushed = junction->ports.size() > info.portsFromUnitsToPush.size() + (isBoundToMe ? 1 : 0);
			if (isBoundToMe) {
				// Migrate to existing case
				Junction& internalJunction = *exportsReverseLookup_.at(info.portFromMe);
				for (Port* port : info.portsFromUnitsToPush) {
					port->unbind();
					port->bind(internalJunction);
				}
				if (!isBoundToUnpushed && !junction->isExported) {
					info.portFromMe->unbind();
					Factory::destroy(*junction);
					remove_export_raw_(internalJunction);
				}
			} else if (!isBoundToUnpushed && !junction->isExported){
				// Steal case
				junction->parent->junctions_.erase(junction);
				junctions_.emplace(junction);
				junction->parent = this;
			} else {
				// Create and migrate case
				Junction& internalJunction = Factory::in(*this).create_junction();
				for (Port* port : info.portsFromUnitsToPush) {
					port->unbind();
					port->bind(internalJunction);
				}
				Port& exportedPort = add_export_raw_(internalJunction);
				exportedPort.bind(*junction);
			}
		}
	}

	void CompositeUnit::pop(const std::unordered_set<BaseUnit*>& unitsToPop) {
		if (parent == nullptr) {
			throw std::runtime_error("Cannot pop from root unit.");
		}
		struct JunctionPopInfo {
			std::vector<Port*> portsFromUnitsToPop{};
		};
		std::unordered_map<Junction*, JunctionPopInfo> touchedJunctionsInfo{};
		for (BaseUnit* unit : unitsToPop) {
			// Claim the unit
			if (unit->parent != this) {
				throw std::runtime_error(std::format("%s is not a child of \"%s\"", unit->name(), name()).c_str());
			}
			units_.erase(unit);
			parent->units_.emplace(unit);
			unit->parent = parent;
			// Scan ports for touched junctions
			for (Port& port : unit->ports) {
				// implicitly create JunctionInfo if not already present
				touchedJunctionsInfo[port.junction].portsFromUnitsToPop.push_back(&port);
			}
		}
		// Handle touched junctions
		for (const auto& [junction, info] : touchedJunctionsInfo) {
			if (junction->parent != this) {
				throw std::runtime_error(std::format("Junction %d is not a child of \"%s\"", junction->id, name()).c_str());
			}
			bool isExported = junction->isExported;
			bool isBoundToUnpopped = junction->ports.size() != info.portsFromUnitsToPop.size();
			if (isExported) {
				// Migrate to existing case
				Port& exportedPort = *exports_.at(junction);
				Junction& externalJunction = *exportedPort.junction;
				for (Port* port : info.portsFromUnitsToPop) {
					port->unbind();
					port->bind(externalJunction);
				}
				if (!isBoundToUnpopped) {
					exportedPort.unbind();
					remove_export_raw_(*junction);
					Factory::destroy(*junction);
				}
			} else if(!isBoundToUnpopped) {
				// Give away case
				junctions_.erase(junction);
				parent->junctions_.emplace(junction);
				junction->parent = parent;
			} else {
				// Create export case
				Junction& externalJunction = Factory::in(*parent).create_junction();
				for (Port* port : info.portsFromUnitsToPop) {
					port->unbind();
					port->bind(externalJunction);
				}
				Port& exportedPort = add_export_raw_(*junction);
				exportedPort.bind(externalJunction);
			}
		}
	}

	std::string CompositeUnit::name() const {
		return std::format("Composite Unit %d/%d/%d (%d)", units_.size(), junctions_.size(), exports_.size(), id);
	}

	int CompositeUnit::node_count() const {
		int count = static_cast<int>(junctions_.size() - ports.size());
		for (auto unit = units_.begin(); unit != units_.end(); unit++) {
			count += (*unit)->node_count();
		}
		return count;
	}

	int CompositeUnit::edge_count() const {
		int count = 0;
		for (auto unit = units_.begin(); unit != units_.end(); unit++) {
			count += (*unit)->edge_count();
		}
		return count;
	}

	void CompositeUnit::render(Simulation::GraphBuilder& graphBuilder, const std::vector<Simulation::Node*>& bindings) const {
		std::unordered_map<const Junction*, Simulation::Node*> internalNodeLookup;
		std::unordered_map<const Junction*, Simulation::Node*> exportedNodeLookup{};
		// Don't use ports iterators as its iterator is non-const to enable returning a non-const Port&. :/
		for (auto i = 0; i < port_count_(); i++) {
			Junction* junction = std::next(exports_.begin(), i)->first;
			exportedNodeLookup[junction] = bindings[i];
		}
		for (Junction* junction : junctions_) {
			if (!junction->isExported) {
				internalNodeLookup[junction] = &graphBuilder.add_node();
			}
		}
		for (BaseUnit* unit : units_) {
			std::vector<Simulation::Node*> unitBindings;
			unitBindings.reserve(unit->ports.size());
			for (Port& port : unit->ports) {
				Simulation::Node* boundNode;
				if (port.junction->isExported) {
					boundNode = exportedNodeLookup.at(port.junction);
				} else {
					boundNode = internalNodeLookup.at(port.junction);
				}
				unitBindings.push_back(boundNode);
			}
			unit->render(graphBuilder, unitBindings);
		}
	}

	bool CompositeUnit::validate(std::ostream& output) const {
		bool pass = true;
		for (BaseUnit* unit : units_) {
			for (Port& port : unit->ports) {
				if (!port.is_bound()) {
					output << ("Unbound port.") << std::endl;
					pass = false;
				}
				if (!junctions_.contains(port.junction)) {
					output << ("Internal port bound to external junction.") << std::endl;
					pass = false;
				}
			}
			pass &= unit->validate(output);
		}
		for (Junction* junction : junctions_) {
			for (Port* port : junction->ports) {
				if (!units_.contains(&port->unit)) {
					output << ("External unit bound to internal junction.") << std::endl;
					pass = false;
				}
			}
			if (junction->ports.size() == 0) {
				output << ("Junction has no connected units.") << std::endl;
				pass = false;
			}
			if (junction->isExported != exports_.contains(junction)) {
				output << ("Junction isExported flag inconsistent with actual export registry.") << std::endl;
				pass = false;
			}
		}
		if (exports_.size() != exportsReverseLookup_.size()) {
			output << ("exportsReverseLookup different length from exports") << std::endl;
			pass = false;
		}
		for (auto exp = exports_.begin(); exp != exports_.end(); exp++) {
			if (!exportsReverseLookup_.contains(exp->second.get()) || exportsReverseLookup_.at(exp->second.get()) != exp->first) {
				output << ("exportsReverseLookup inconsistent with exports.") << std::endl;
				pass = false;
			}
		}
		return pass;
	}

	Port& CompositeUnit::get_export(Junction& junction) {
		return *exports_.at(&junction);
	}

	Junction& CompositeUnit::add_export(Junction& junction) {
		Port& exportedPort = add_export_raw_(junction);
		// TODO: make this work for exporting from design root
		if (!parent) {
			throw std::runtime_error("Cannot add export to root unit.");
		}
		Junction& exportedJunction = Factory::in(*parent).create_junction();
		exportedPort.bind(exportedJunction);
		return exportedJunction;
	}

	void CompositeUnit::remove_export(Junction& junction) {
		Port* exportedPort = exports_.at(&junction).get();
		Junction& exportedJunction = exportedPort->unbind();
		if (exportedJunction.ports.size() == 0) {
			Factory::destroy(exportedJunction);
		}
		remove_export_raw_(junction);
	}

	// PRIVATE

	size_t CompositeUnit::port_count_() const {
		return static_cast<int>(exports_.size());
	}

	Port& CompositeUnit::get_port_(size_t i) {
		return *std::next(exports_.begin(), i)->second;
	}

	// Cannot take const internalPort as reverseLookup needs to be able to return a non-const internal port
	Port& CompositeUnit::add_export_raw_(Junction& junction) {
		auto emplaceResult = exports_.emplace(&junction, std::make_unique<Port>(*this, false));
		if (!emplaceResult.second) {
			throw std::runtime_error("Junction is already exported.");
		}
		Port* exportedPort = emplaceResult.first->second.get();
		if (!exportsReverseLookup_.emplace(exportedPort, &junction).second) {
			throw std::runtime_error("exportedPort is already registered.");
		}
		junction.isExported = true;
		return *exportedPort;
	}

	std::unique_ptr<Port> CompositeUnit::remove_export_raw_(Junction& junction) {
		if (!exports_.contains(&junction)) {
			throw std::runtime_error("Junction is not exported.");
		}
		std::unique_ptr<Port> exportedPort = std::move(exports_.at(&junction));
		exports_.erase(&junction);
		if (!exportsReverseLookup_.erase(exportedPort.get())) {
			throw std::runtime_error("Exported port was already removed.");
		}
		junction.isExported = false;
		return exportedPort;
	}

}