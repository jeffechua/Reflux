#pragma once

namespace Microsoft::VisualStudio::CppUnitTestFramework {

	template<>
	static std::wstring ToString<Reflux::Engine::Simulation::Node>(Reflux::Engine::Simulation::Node* node) {
		return std::to_wstring(reinterpret_cast<std::uintptr_t>(node));
	}

	template<>
	static std::wstring ToString<Reflux::Engine::Design::Port>(Reflux::Engine::Design::Port* port) {
		return std::to_wstring(reinterpret_cast<std::uintptr_t>(port));
	}

	template<>
	static std::wstring ToString<Reflux::Engine::Design::Junction>(Reflux::Engine::Design::Junction* junction) {
		return std::to_wstring(reinterpret_cast<std::uintptr_t>(junction));
	}

	template<>
	static std::wstring ToString<Reflux::Engine::Design::BaseUnit>(Reflux::Engine::Design::BaseUnit* unit) {
		return std::to_wstring(reinterpret_cast<std::uintptr_t>(unit));
	}

}